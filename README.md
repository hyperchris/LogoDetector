# LogoDetector
use the detector to find the  in an image

#Update
- Available [logo_name]: 'subway'
- Tricks: 
1. The upper 15% and lower 1/3 will be ignored in both neg image selection and testing process
2. The contrast of training and testing images is increased to 1.3x (to be done)
3. 

##How to use

### For single image
- Include 'obj_detect.py', use the 'get_result' function inside.
- Parameters for 'get_result': [img_path], [logo_name], ([store_img=1], [res_dir_path='res/'])
- Return value format: 	
1. RESULT: empty
2. RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]
- store_img = 1 will store the result in res_dir_path. If you want to make the code run faster, set it to 0

### For multiple images
- Use: $ python obj_detect.py [input_dir_path] [logo_name] [threshold] ([res_dir_path='res/'])
- All the result images will be stored in [res_dir_path], default value is 'res/'
