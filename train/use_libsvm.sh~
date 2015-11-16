#!/bin/sh
# Author: Xiaochen
# Usage: Use this to compile the SVM training process

echo 'start'
# Scale
# ./libsvm/svm-scale -l <-1> -u <1> -s [input_file] > [output_file]
./libsvm/svm-scale -l -u -s $1 > $2

# Training
# ./libsvm/svm-train -s 0 -t 0(2) -c x (-g x) -w1 [pos_weight] -w-1 [neg_weight] [input_file] [output_model]

# Prediction
# ./libsvm/svm-predict <options> [test_file] [model_file] [output_file]

# Cross-validation:
# python libsvm/tools/grid.py -log2c (begin,end,step) (-log2g (begin,end,step)) -v (fold=5) -svmtrain [path_of_svm-train] <options of svm-train> [input_file]

echo 'done'
 
