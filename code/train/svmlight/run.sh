#!/bin/sh
# Author: Xiaochen
# Usage: Use this to run the SVM training process

# 1234: pos_img_dir, neg_img_dir, descriptor_dir, need_training

# meaning of between..:
# - before $4: win_wid, win_hei, blk_wid, blk_hei, bstr_wid, bstr_hei, cell_wid, cell_hei, 
# - after $4: svm_c, svm_pos_weight

# for subway: 120 36 12 12 6 6 6 6 $4 0.03125 7.0
# for stop_sign: 64 64 8 8 4 4 4 4 $4 0.03125 10.0

echo 'start'
./train $1 $2 $3 64 64 8 8 4 4 4 4 $4 0.03125 10.0
echo 'done'
