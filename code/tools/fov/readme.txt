Function: 
- Read viewpoints file and triangulation file. Then extract the viewpoints (vp) associated to one triangulation point. 
- For each vp, one static Google map will be downloaded. Then the Field of view of this vp will be learned and drawn onto the map. The triangulation will also be shown in red dot. 
- If the red dot is within FoV, the vp_pos number of this triangulation point will be added by one. 
- Finally, the vp_pos results will be written into a file. 

How to use: 
- $ python fov_main.py [vp_file] [t_res_file] [result_dir]

Workflow: 
- fov_main -> parse -> download_map & gps_dist -> detect_fov
- common.py is the class that used to store data of a t_p or vp
- detect_fov.py will call cpp code in detect_fov/

Note:
- parse has problem: vp_list will be appended together regardless of their t_p... This part is still in debugging
- detect_fov not completed: can only draw the two edge of FoV
