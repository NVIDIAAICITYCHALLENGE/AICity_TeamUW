# Multiple-Kernel Based Vehicle Tracking Using 3D Deformable Model and Camera Self-Calibration for 2017 IEEE Smart World NVIDIA AI City Challenge
This repository contains our source code of Track 1 and Track 2 for the NVIDIA AI City Challenge.
The source code of Track 1 is built with options of HOG or YOLO v2 object detectors. 
The source code of Track 2 is developed based on the implementation by the honorable graduates from the Information Processing Lab, Dr. Kuan-Hui Lee, who is currently a research scientist at the Toyota Research Institute, and Dr. Chun-Te Chu, who is currently a software development engineer at Microsoft. 

Both Track 1 and Track 2 have been tested on the NVIDIA Jetson TX2 developer kit. Dependencies include CUDA, cuDNN, OpenCV library and g++ compiler.

## Introduction

### 2017 IEEE Smart World NVIDIA AI City Challenge

The AI City Challenge by IEEE Smart World and NVIDIA aim to bring together the following:

1. A traffic camera video dataset of reasonable quality.
2. A collaborative annotation effort among participating teams to label the dataset.
3. Powerful GPU computing infrastructure, from cloud to edge, to enable the training and inferencing of models for real-time detection and classification of objects.
4. A panel of subject matter experts to advise on AI City analytics and help with problems related to safety and congestion. 

The Challenge comprise of two tracks: 

1. Track 1 - AI City Foundation Track - Teams use the training set data and labels to develop algorithms and models for basic machine learning tasks, such as object detection, and classification.
2. Track 2 - AI City Applications Track - Teams leverage available datasets to propose and develop AI City applications geared towards solving salient problems related to safety and/or congestion in an urban environment. 

Detailed information of this challenge can be found [here](http://smart-city-conference.com/AICityChallenge/index.html).

Our team is selected as the winning team of Track 2 for the value and innovation of our proposed approach, 
along with the success of our demonstration. 

### Multi-scale Testing for Object Detection

In Track 1 of the NVIDIA AI City Challenge, to better detect far-away small objects, we use multi-scale testing strategy. For each testing frame, the image is divided into 9 sub-regions with 50% overlapping between each pair of neighboring sub-regions. Then the individual results from each sub-region are combined together according to non-maximum suppression. 

### Constrained Multiple-Kernel (CMK) Vehicle Tracking Based on Deformable 3D Model

Tracking of multiple objects is an important application in AI City geared towards solving salient problems related to safety and congestion in an urban environment. 
Frequent occlusion in traffic surveillance has been a major problem in this research field. 
To resolve this problem, the proposed approach regards each patch of the 3D vehicle model as a kernel, and tracks the kernels under certain constrains facilitated with the 3D geometry of the vehicle model. 
Meanwhile, a kernel density estimator is designed to well fit the 3D vehicle model during tracking. 
By elegant application of the constrained multiple-kernel tracking facilitated with the 3D vehicle model, 
the vehicles are able to be tracked efficiently and located precisely. 
The proposed approach achieves high effectiveness in the tracking and localization by taking advantage of the color similarity and shape fitness.

The fitness of deformable 3D vehicle model is described in a [TIP 2012 paper](http://www.nlpr.ia.ac.cn/2012papers/gjkw/gk14.pdf). 
The CMK vehicle tracking framework based on 3D vehicle model is presented by our research team in a [TCSVT 2014 paper](http://ieeexplore.ieee.org/abstract/document/6826495/).

## Code structure

### Track 1

Under the "./DET_MST_IPL/" folder

1. "./obj/" folder: Libraries of .cpp files for compilation
2. "./src/" folder: Header files
3. "./darknet/" folder: Edited version of Darknet for C++ interaction
4. "./data/" folder: Example
   - "cfg.json": Important configuration parameters in JSON format
   - "roi.jpg": Region of interest (ROI) image
   - "vdo.mp4": Input video source (need to provide)
   - "./det/" folder: Detection results in text file (need to create)

The output of object detection in text file follows the format below (following the standard of MOTChallenge). 

\<frame no.\>,-1,\<2D x(left)\>,\<2D y(top)\>,\<2D width\>,\<2D height\>,\<detection score\>,-1,-1,-1,\<object class\>

The user can also choose to output plotted frames in video/image. But the computation time will increase accordingly. 

### Track 2

Under the "./CMK_VEH_TRK_IPL/" folder
1. "./obj/" folder: Libraries of .cpp files for compilation
2. "./src/" folder: Header files
3. "./data/" folder: Example of camera ID 0
   - "cam_config.ini": Configuration parameters
   - "roi.jpeg": Region of interest (ROI) image
   - "background.png": Input background image to initialize background subtraction
   - "video.avi": Input video to process
   - "InExCamParam.txt": Camera parameters

The text file of camera parameters should include the camera’s intrinsic parameters and extrinsic parameters, and follow the format as follows:

fx     0    cx    0    fy    cy    0    0    1

R[0]  R[1]  R[2]  R[3]  R[4]  R[5]  R[6]  R[7]  R[8]

T[0]  T[1]  T[2]

P[0]  P[1]  P[2]  P[3]  P[4]  P[5]  P[6]  P[7]  P[8]  P[9]  P[10]  P[11]

The output 3D vehicle model(s) at each frame are saved in the "./camID/" folder as "carID_frameID.txt": L, W1, W2, H1, H2, H3, H4, X1, X2, X3, X4, Ta, Translation.x, Translation.y, Translation.z, Rotation.x, Rotation.y, Rotation.z, DeformableRange.ScaleLength, DeformableRange. ScaleWidth, DeformableRange.ScaleHeight. Please read the vehicle modeling paper referred to in our final paper/report to understand the concepts of these parameters. The command window output is saved in "log.txt". The output frames with 3D models/tracking results plotted are saved in the folder "./results/". The FES (Fitness Evaluation Score) for each initialized 3D vehicle model is saved in the folder "./fes_score/" as "carID_fes_scoreframeID.txt". 

## Installation

### Track 1

1. Install CUDA and cuDNN. 
2. Download and make the OpenCV library (**v3.0+**, tested with v3.3.0).
3. Compile the provided source code using g++ in Linux environment.
4. Under the “./darknet/” folder, make the YOLO detector with GPU and CUDNN enabled. 

### Track 2

1. Download and make the OpenCV library (**v2.4+**, tested with v2.4.3 and v2.4.13.3).
2. Compile the provided source code using g++ in Linux environment.

## Usage

### Track 1

1. Set the corresponding input/output paths in the configuration file if necessary. 
2. When running the program, a window called “current frame” will pop up first.
3. When the ROI image is not selected, an ROI image of the entire frame will be automatically created. If the user wants to select a specified ROI, set “selRoiFlg” to 1. The user can perform 3 choices: 1) press ‘p’ to proceed to the frame that s/he wants to select ROI in, 2) press ‘Esc’ and load the existing ROI image, 3) press ‘s’ to select ROI in the current frame. The process of selecting ROI is described as follows:  
   - A new window called “ROI selector” pops out. This should be the current frame that the user proceeds to.
   - Click on the image around the ROI. A blue circle stands for each click and a white line connect two adjacent clicks. Note that in order to make a valid ROI, the last click should be close to the first click, i.e., two blue circles should overlap.
   - After the selection of ROI is done, click “Enter”. A binary mask image that shows the mask for ROI is created. 
   - During ROI selection, if mis-clicking on wrong places, the user can press ‘r’. All the markers will be cleared, and s/he can start over.
4. When the detection results are not available, the user can choose to run HOG detector (for person only) or YOLO v2 detector online. For YOLO v2 detector, the user needs to set the threshold for detection score and the object classes of interest. When “detMltSclTstFlg” is set, each frame will be tested in multiple scales and the results are combined through non-maximum suppression, so that more small objects can be detected. As a requirement, follow the steps shown at the terminal to open a new terminal, cd to the directory of “./darknet/” folder and execute the command to set the model for detection. After it is done, press any key to continue. 

### Track 2

Here we give an example about how to use the interface APIs and coding flow. The example is coded in the file "main.cpp". 

In the beginning, we have to claim an object of the tracking system by setting number of the total cameras as an input parameter. 
In the example, we only use 1 camera:

`CIndVehicleTracking ind_vehicle_tracker(1);`

Next, we have to do the initialization by calling:

`IndVehicleTracking_Init(cam_id, iVdoWidth, iVdoHeight, ConfigPath);`

There are 4 input parameters: 1) camera ID, 2) width, 3) height of the videos, and 4) path of configuration file. 
The tracking system then read the configuration file (ex: `cam_config.ini` under the working directory). 
This file defines the configurations, including many thresholds and pre-determined constants.

After initialization, we have to set the background image by calling:

`IndVehicleTracking_ SetBgImg(cam_id, bgData);`

This function can also be called during the processing to update the background. 
As for ROI, there is a corresponding function named:

`IndVehicle Tracking_SetBgImg(cam_id, roiData);`

For each video frame, we have to call:

`IndVehicleTracking_Process(cam_id, imageData);`

to process the tracking. There are 2 input parameters, 1) camera ID, and 2) image data in terms of BYTE*. 
The image data is RGB format in order of RGBRGBRGB…, and the time stamp is used to count the video.

Finally, we release the resource using in the tracking system by calling:

`IndVehicleTracking_UnInit(cam_id);`

## Experimental Results

### Track 1

The details of experimental results of Track 1 can be found on the [challenge website](http://smart-city-conference.com/AICityChallenge/index.html). 

### Track 2

Our experimental data are from two videos within the sub-dataset of Silicon Valley Intersection. 
We manually label 1,760 tracking locations as ground truth including 32 objects across 1,356 frames.

The standard metrics used in [MOT Challenge](https://motchallenge.net/) are adopted to evaluate each method. 
Multiple Object Tracking Accuracy (MOTA) measures tracking accuracy combining three error sources: false positives, missed targets and identity switches. 
Multiple Object Tracking Precision (MOTP) measures the misalignment between the annotated and the predicted bounding boxes. 
FAF represents the average number of false alarms per frame. 
FP, FN, and ID Sw. stand for the total numbers of false positives, false negatives (missed targets) and identity switches respectively. 

Our results are listed as follows:

MOTA: 82.0%

MOTP: 99.5%

FAF: 0.23

FP: 7

FN: 310

ID Sw.: 0

The demo videos can be watched on our website: http://allison.ee.washington.edu/thomas/3dvt/.

## Reference

Z. Tang, G. Wang, T. Liu, Y.-G. Lee, A. Jahn, X. Liu, X. He and J.-N. Hwang, “Multiple-kernel based vehicle tracking using 3D deformable model and camera self-calibration,” arXiv preprint arXiv:1708.06831, Aug. 2017.

## Disclaimer

For any question you can contact [Zheng (Thomas) Tang](https://github.com/zhengthomastang).
