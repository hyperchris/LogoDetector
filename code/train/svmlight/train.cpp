/*
 * Function: Train HoG classifier with SVMLight
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <cctype>

using namespace std;
using namespace cv;

#define SVMLIGHT 1
#define LIBSVM 2

#define ALPHA 1.3 // factor that the contrast is magnified
#define BETA 0 // factor that the brightness is increased

//#define TRAINHOG_USEDSVM SVMLIGHT
#define TRAINHOG_USEDSVM SVMLIGHT

#if TRAINHOG_USEDSVM == SVMLIGHT
    #include "svmlight/svmlight.h"
    #define TRAINHOG_SVM_TO_TRAIN SVMlight
#endif

static int WSIZE_WIDTH = 120; // window size
static int WSIZE_HEIGHT = 36;

static int BSIZE_WIDTH = 12; // block size
static int BSIZE_HEIGHT = 12;

static int BSTRIDE_WIDTH = 6; // block stride
static int BSTRIDE_HEIGHT = 6;

static int CSIZE_WIDTH = 6; // cell size
static int CSIZE_HEIGHT = 6;

// Directory containing positive sample images
static string posSamplesDir = "pos/";
// Directory containing negative sample images
static string negSamplesDir = "neg/";
// Set the file to write the features to
static string featuresFile = "genfiles/features.dat";
// Set the file to write the SVM model to
static string svmModelFile = "genfiles/svmlightmodel.dat";
// Set the file to write the resulting detecting descriptor vector to
static string descriptorVectorFile = "genfiles/descriptorvector.dat";

// HOG parameters for training that for some reason are not included in the HOG class
static const Size trainingPadding = Size(0, 0);
static const Size winStride = Size(8, 8);

static string toLowerCase(const string& in) {
    string t;
    for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
        t += tolower(*i);
    }
    return t;
}

static void storeCursor(void) {
    printf("\033[s");
}

static void resetCursor(void) {
    printf("\033[u");
}

static void saveDescriptorVectorToFile(vector<float>& descriptorVector, vector<unsigned int>& _vectorIndices, string fileName) {
    printf("Saving descriptor vector to file '%s'\n", fileName.c_str());
    string separator = " "; // Use blank as default separator between single features
    fstream File;
    float percent;
    File.open(fileName.c_str(), ios::out);
    if (File.good() && File.is_open()) {
        printf("Saving %lu descriptor vector features:\t", descriptorVector.size());
        storeCursor();
        for (int feature = 0; feature < descriptorVector.size(); ++feature) {
            if ((feature % 10 == 0) || (feature == (descriptorVector.size()-1)) ) {
                percent = ((1 + feature) * 100 / descriptorVector.size());
                printf("%4u (%3.0f%%)", feature, percent);
                fflush(stdout);
                resetCursor();
            }
            File << descriptorVector.at(feature) << separator;
        }
        printf("\n");
        File << endl;
        File.flush();
        File.close();
    }
}

static void getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions) {
    printf("Opening directory %s\n", dirName.c_str());
    struct dirent* ep;
    size_t extensionLocation;
    DIR* dp = opendir(dirName.c_str());
    if (dp != NULL) {
        while ((ep = readdir(dp))) {
            if (ep->d_type & DT_DIR) {
                continue;
            }
            extensionLocation = string(ep->d_name).find_last_of("."); // Assume the last point marks beginning of extension like file.ext
            // Check if extension is matching the wanted ones
            string tempExt = toLowerCase(string(ep->d_name).substr(extensionLocation + 1));
            if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end()) {
                printf("Found matching data file '%s'\n", ep->d_name);
                fileNames.push_back((string) dirName + ep->d_name);
            } else {
                printf("Found file does not match required file type, skipping: '%s'\n", ep->d_name);
            }
        }
        (void) closedir(dp);
    } else {
        printf("Error opening directory '%s'!\n", dirName.c_str());
    }
    return;
}

static void calculateFeaturesFromInput(const string& imageFilename, vector<float>& featureVector, HOGDescriptor& hog) {
    Mat imageData = imread(imageFilename, 1);
    if (imageData.empty()) {
        featureVector.clear();
        printf("Error: HOG image '%s' is empty, features calculation skipped!\n", imageFilename.c_str());
        return;
    }
    
    // increase contrast
    Mat imageData_enhanced = imageData;
    for(int y = 0; y < imageData.rows; y++) { 
        for( int x = 0; x < imageData.cols; x++ ) { 
            for( int c = 0; c < 3; c++ ) {
                imageData_enhanced.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(ALPHA * (imageData.at<Vec3b>(y,x)[c]) + BETA);
             }
        }
    }

    // Check for mismatching dimensions
    if (imageData.cols != hog.winSize.width || imageData.rows != hog.winSize.height) {
        featureVector.clear();
        printf("Error: Image '%s' dimensions (%u x %u) do not match HOG window size (%u x %u)!\n", imageFilename.c_str(), imageData.cols, imageData.rows, hog.winSize.width, hog.winSize.height);
        return;
    }
    vector<Point> locations;
    Mat image;
    cvtColor(imageData_enhanced, image, CV_BGR2GRAY);
    // imshow("good", image);
    // waitKey(0);
    hog.compute(image, featureVector, winStride, trainingPadding, locations);
    imageData.release(); // Release the image again after features are extracted
}

static void showDetections(const vector<Rect>& found, Mat& imageData) {
    vector<Rect> found_filtered;
    size_t i, j;
    for (i = 0; i < found.size(); ++i) {
        Rect r = found[i];
        for (j = 0; j < found.size(); ++j)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    for (i = 0; i < found_filtered.size(); i++) {
        Rect r = found_filtered[i];
        rectangle(imageData, r.tl(), r.br(), Scalar(64, 255, 64), 3);
    }
}

static void detectTrainingSetTest(const HOGDescriptor& hog, const double hitThreshold, const vector<string>& posFileNames, const vector<string>& negFileNames) {
    unsigned int truePositives = 0;
    unsigned int trueNegatives = 0;
    unsigned int falsePositives = 0;
    unsigned int falseNegatives = 0;
    vector<Point> foundDetection;
    // Walk over positive training samples, generate images and detect
    for (vector<string>::const_iterator posTrainingIterator = posFileNames.begin(); posTrainingIterator != posFileNames.end(); ++posTrainingIterator) {
        const Mat imageData = imread(*posTrainingIterator, 0);
        hog.detect(imageData, foundDetection, hitThreshold, winStride, trainingPadding);
        if (foundDetection.size() > 0) {
            ++truePositives;
            falseNegatives += foundDetection.size() - 1;
        } else {
            ++falseNegatives;
        }
    }
    // Walk over negative training samples, generate images and detect
    for (vector<string>::const_iterator negTrainingIterator = negFileNames.begin(); negTrainingIterator != negFileNames.end(); ++negTrainingIterator) {
        const Mat imageData = imread(*negTrainingIterator, 0);
        hog.detect(imageData, foundDetection, hitThreshold, winStride, trainingPadding);
        if (foundDetection.size() > 0) {
            falsePositives += foundDetection.size();
        } else {
            ++trueNegatives;
        }        
    }
    
    printf("Results:\n\tTrue Positives: %u\n\tTrue Negatives: %u\n\tFalse Positives: %u\n\tFalse Negatives: %u\n", truePositives, trueNegatives, falsePositives, falseNegatives);
}

static void detectTest(const HOGDescriptor& hog, const double hitThreshold, Mat& imageData) {
    vector<Rect> found;
    Size padding(Size(0, 0));
    Size winStride(Size(CSIZE_WIDTH, CSIZE_HEIGHT));
    hog.detectMultiScale(imageData, found, hitThreshold, winStride, padding);
    showDetections(found, imageData);
}

static void detectImages(const HOGDescriptor& hog, const double hitThreshold, string root_dir, int img_num) {
    for (int i = 0; i < img_num; i++) {
        Mat testImage = imread(root_dir + "img-" + std::to_string(i) + ".jpg", 1);
        detectTest(hog, hitThreshold, testImage);
        imshow("HOG custom detection", testImage);
        waitKey(0);
    }
    return;
}

/**
 * ----------------------------------- main -----------------------------------
 */
int main(int argc, char** argv) {
    if (argc != 13) {
        cout << "ERROR: argc wrong: ./train [pos_path] [neg_path] [res_path] " << 
        "[w_wid] [w_hei] [bsize_wid] [bsize_hei] [bstride_wid] [bstride_hei] [c_wid] [c_hei] [need_training]" << endl;
        return -1;
    }

    // assign parameters:
    posSamplesDir = argv[1];
    negSamplesDir = argv[2];
    descriptorVectorFile = argv[3];

    WSIZE_WIDTH = atoi(argv[4]);
    WSIZE_HEIGHT = atoi(argv[5]);
    BSIZE_WIDTH = atoi(argv[6]);
    BSIZE_HEIGHT = atoi(argv[7]);
    BSTRIDE_WIDTH = atoi(argv[8]);
    BSTRIDE_HEIGHT = atoi(argv[9]);
    CSIZE_WIDTH = atoi(argv[10]);
    CSIZE_HEIGHT = atoi(argv[11]);

    int need_training = atoi(argv[12]);

    HOGDescriptor hog; // Use standard parameters here
    hog.winSize = Size(WSIZE_WIDTH, WSIZE_HEIGHT); // Default training images size as used in paper
    hog.cellSize = Size(CSIZE_WIDTH, CSIZE_HEIGHT); // cell size 
    hog.blockSize = Size(BSIZE_WIDTH, BSIZE_HEIGHT); // blocksize 
    hog.blockStride = Size(BSTRIDE_WIDTH, BSTRIDE_HEIGHT); // blockstride

    // Get the files to train from somewhere
    static vector<string> positiveTrainingImages;
    static vector<string> negativeTrainingImages;
    static vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

    getFilesInDirectory(posSamplesDir, positiveTrainingImages, validExtensions);
    getFilesInDirectory(negSamplesDir, negativeTrainingImages, validExtensions);
    /// Retrieve the descriptor vectors from the samples
    unsigned long overallSamples = positiveTrainingImages.size() + negativeTrainingImages.size();
    
    // Make sure there are actually samples to train
    if (overallSamples == 0) {
        printf("No training sample files found, nothing to do!\n");
        return EXIT_SUCCESS;
    }

    /// @WARNING: This is really important, some libraries (e.g. ROS) seems to set the system locale which takes decimal commata instead of points which causes the file input parsing to fail
    setlocale(LC_ALL, "C"); // Do not use the system locale
    setlocale(LC_NUMERIC,"C");
    setlocale(LC_ALL, "POSIX");

    printf("Reading files, generating HOG features and save them to file '%s':\n", featuresFile.c_str());
    float percent;
    fstream File;
    File.open(featuresFile.c_str(), ios::out);
    if (File.good() && File.is_open()){
        // Iterate over sample images
        printf("------------- start reading\n");
        for (unsigned long currentFile = 0; currentFile < overallSamples; ++currentFile) {
            storeCursor();
            vector<float> featureVector;
            // Get positive or negative sample image file path
            const string currentImageFile = (currentFile < positiveTrainingImages.size() ? positiveTrainingImages.at(currentFile) : negativeTrainingImages.at(currentFile - positiveTrainingImages.size()));
            // Output progress
            if ( (currentFile+1) % 10 == 0 || (currentFile+1) == overallSamples ) {
                percent = ((currentFile+1) * 100 / overallSamples);
                printf("%5lu (%3.0f%%):\tFile '%s'", (currentFile+1), percent, currentImageFile.c_str());
                fflush(stdout);
                resetCursor();
            }
            // Calculate feature vector from current image file
            calculateFeaturesFromInput(currentImageFile, featureVector, hog);
            if (!featureVector.empty()) {
                File << ((currentFile < positiveTrainingImages.size()) ? "+1" : "-1");
                // Save feature vector components
                for (unsigned int feature = 0; feature < featureVector.size(); ++feature) {
                    File << " " << (feature + 1) << ":" << featureVector.at(feature);
                }
                File << endl;
            }
        }
        printf("\n");
        File.flush();
        File.close();
    } else {
        printf("Error opening file '%s'!\n", featuresFile.c_str());
        return EXIT_FAILURE;
    }
    
    if (!need_training) {
        cout << "No training. Features generated." << endl;
        return EXIT_SUCCESS;
    }

    /// Read in and train the calculated feature vectors
    printf("Calling %s\n", TRAINHOG_SVM_TO_TRAIN::getInstance()->getSVMName());
    TRAINHOG_SVM_TO_TRAIN::getInstance()->read_problem(const_cast<char*> (featuresFile.c_str()));
    TRAINHOG_SVM_TO_TRAIN::getInstance()->train(); // Call the core libsvm training procedure
    printf("Training done, saving model file!\n");
    TRAINHOG_SVM_TO_TRAIN::getInstance()->saveModelToFile(svmModelFile);
    /* -------------------------- trained model saved ---------------------------------------- */

    printf("Generating representative single HOG feature vector using svmlight!\n");
    vector<float> descriptorVector;
    vector<unsigned int> descriptorVectorIndices;
    // Generate a single detecting feature vector (v1 | b) from the trained support vectors, for use e.g. with the HOG algorithm
    TRAINHOG_SVM_TO_TRAIN::getInstance()->getSingleDetectingVector(descriptorVector, descriptorVectorIndices);
    // And save the precious to file system
    saveDescriptorVectorToFile(descriptorVector, descriptorVectorIndices, descriptorVectorFile);
    
    // Detector detection tolerance threshold
    const double hitThreshold = TRAINHOG_SVM_TO_TRAIN::getInstance()->getThreshold();
    cout << "threshold " << hitThreshold << endl; 
    // Set our custom detecting vector
    hog.setSVMDetector(descriptorVector);
    
    printf("Testing training phase using training set as test set (just to check if training is ok - no detection quality conclusion with this!)\n");
    detectTrainingSetTest(hog, hitThreshold, positiveTrainingImages, negativeTrainingImages);

    printf("Training Finished\n");
    return EXIT_SUCCESS;
}
