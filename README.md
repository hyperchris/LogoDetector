# LogoDetector
use the detector to find the in an image

##Update
**Available [logo_name]: 'subway', 'stopsign', 'chase', 'mobil'**
- fov_detect added
- Win_stride 1x
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

##Detect Field of View
This part can detect if the triangulation result is within the field of view from one view points. *Support code can be found in 'code/tools/fov_detect'*

### Input
- Command: $ python fov_detect.py [view_point_file] [triangulation_file] [res_dir]
- Sample of view_point_file and triangulation_file can be found in 'code/sample_data/'

### Output
- Result wil be stored in [res_dir]
- Each line in 'result.txt' means [location_number : number_of_points_within_FoV]
- Images are the visualized results.
