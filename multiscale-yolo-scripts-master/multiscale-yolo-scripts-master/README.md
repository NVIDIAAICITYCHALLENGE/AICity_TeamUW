# multiscale-yolo-scripts
The code is for IEEE Nvidia Smart city challenge, Aug 2017 (rank 4th in total)  
We apply half size sliding windonw to slide through origin image in 3x3 times. Then apply YOLO wih nms.  


## Pipeline
images -> cropped images -> Apply YOLO for detection -> convert format (json->txt) -> merge and nms

### 1, Crop images
*Prepare cropped images (x9) before detection.*  
``` produce_sliding_windonws.py``` is an example script to create sliding windonw cropped images for a single image
``` produce_sliding_windonws_under_folder.py``` is for all image under folder. (You need to modify the path)
### 2, Apply YOLO
*Run object detection.*  
go into submodule to run 
``` bash command.sh``` (You need to modify the path) It will create an ``` out/``` folder under your input folder.

### 3, Convert format
*Convert outputs from .json to .txt.*  
Example script is in ``` foramt_translate.py ```(You need to modify the path).

### 4, Merge and non-maximom suppression
*Recover the offsets and merge together (9 outputs), then nms*  
Example script is in ``` merge_nms_under_folder.py ```(You need to modify the path) is to do merge+nms in all output txt under a given folder.

### Other scirpts
 ``` reconstruct_txt.py``` is to add offsets for multiple txt yolo outputs and visualize the result.
 ``` visualize_txt_under_folder.py``` visualizes output by reading output txt and input images
 
### Example results
*Note: image rights belong to Nvidia!*  
*YOLO is for detecting objects those are rare in training dataset such as bus, bike, motocycle, the common objects such as car, suv are detected by SSD*
![resutl1](https://github.com/adwin5/multiscale-yolo-scripts/blob/master/example.jpg)
![resutl2](https://github.com/adwin5/multiscale-yolo-scripts/blob/master/example2.jpg)

