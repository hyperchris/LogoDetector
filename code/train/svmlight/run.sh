#!/bin/sh
# Author: Xiaochen
# Usage: Use this to run the SVM training process

# 1234: pos_img_dir, neg_img_dir, descriptor_dir, need_training

echo 'start'
./train $1 $2 $3 120 36 12 12 6 6 6 6 $4
echo 'done'
 
