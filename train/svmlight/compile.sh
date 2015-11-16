#!/bin/sh
# Author: Xiaochen
# Usage: Use this to compile the SVM training process

echo 'start'
g++ -std=c++11 `pkg-config --cflags opencv` -c -g -MMD -MP -MF train.o.d -o train.o train.cpp
# g++ -std=c++11 `pkg-config --cflags opencv` -c -g -MMD -MP -MF detect.o.d -o detect.o detect.cpp
gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_learn.o.d -o svmlight/svm_learn.o svmlight/svm_learn.c
gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_hideo.o.d -o svmlight/svm_hideo.o svmlight/svm_hideo.c
gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_common.o.d -o svmlight/svm_common.o svmlight/svm_common.c
g++ `pkg-config --cflags opencv` -o train train.o svmlight/svm_learn.o svmlight/svm_hideo.o svmlight/svm_common.o `pkg-config --libs opencv`
# g++ `pkg-config --cflags opencv` -o detect detect.o `pkg-config --libs opencv`
echo 'done'
 