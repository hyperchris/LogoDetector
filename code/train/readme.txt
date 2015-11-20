INTRODUCTION:
Currently we train the classifier with SVMlight, HoG feature, and linear kernel. 
RBF kernel and libSVM don't outperform the current configuration.

TODO:
Interage the size parameters into a file

------------------ SVMlight ------------------
This part can generate features and train classifiers. No cross-validation

HOW TO USE: 
- Use 'compile.sh' to compile
- Use 'train' in SVMlight_train to generate the detector file, which will be used by the detection code.
- The feature file will be stored in 'genfiles/'. The descriptor file will be stored in [descriptorFile]
- NOTE: To change the c and positive_weight in training process, modify 'svmlight/svmlight.h'
- NOTE: if no_training = 1, then only feature file will be generated. No training process.

- Modify the size parameters in run.sh, then:
- Input: $ ./run.sh [posSamplesDir] [negSamplesDir] [featuresFile] [no_training]

------------------ libsvm ------------------
You need to use libSVM code to train and cross-validate

------------------ neg_img_generator.py ------------------
This code is used for generating tons of negative (background) images with specific size

HOW TO USE:
- NOTE: DO REMEMBER TO CHECK IF THE GENERATED IMAGES HAVE POSITIVE OBJECT!
- Input: $ python neg_img_generator.py [srcImgDir] [dstDir] [width] [height] [number_from_one_image] [max_width]