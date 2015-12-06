# LogoDetector
use the detector to find the in an image

##Update
**Available [logo_name]: 'subway', 'chase', 'mobil'**
- Make win_stride into 2x distance (4x faster. Higher precision.)
- Contrast: 1.2x

##Merge data
- Include 'merge' in your python code.
- Use **merge_image(src_dir, res_dir)** to do processing. 
- Result will be stored in [res_dir]

##Do detection
**Modify 'code/config.txt' to set the detection threshold and window**

### For single image
- Include 'obj_detect.py', use the 'get_result' function inside.
- Parameters for 'get_result': [img_path], [logo_name], ([store_img=1], [res_dir_path='res/'])
- Return value format: 	
1. RESULT: empty
2. RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]
- store_img = 1 will store the result in res_dir_path. If you want to make the code run faster, set it to 0

### For multiple images
- Use: $ python obj_detect.py [input_dir_path] [logo_name] [threshold] [res_dir_path='res/']
- All the result images will be stored in [res_dir_path], default value is 'res/'
- Recommended threshold between 1.1 and 1.3 (for subway logo)
