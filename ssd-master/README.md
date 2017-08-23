## Installation 
The source code is implemented using tensorflow under Track_1 folder. Dependencies include tensorflow, numpy, opencv and matplotlib. Please carefully read and follow the instructions at https://github.com/balancap/SSD-Tensorflow for any questions.

## Usage
Train: please follow the training step at https://github.com/balancap/SSD-Tensorflow with your own dataset.
Use trained SSD model on AIC dataset: the pre-trained model on AIC datasets is in the Track_1/checkpoints folder. To visualize and save the detection results on the pre-trained model, run detection_demo.ipynb. Modify "ckpt_filename", "path" and "output_dir" to your model directory, image directory and output directory, respectively.
Combine results with YOLO9000: A multi-scale testing of YOLO9000 is avaiable at https://github.com/adwin5/multiscale-yolo-scripts. Please follow the instructions for YOLO detector. Visualize the combined result from YOLO9000 and SSD into one txt detection result by running comb_result.py. You should change "txt_dir1", "txt_dir2" and "img_file" to your own directory, where "txt_dir1" is the txt output from SSD; "txt_dir2" is the txt output from YOLO9000 and "img_file" is the input image.

## Example results (ex_1,ex_2,ex_3 under Track_1 folder)
