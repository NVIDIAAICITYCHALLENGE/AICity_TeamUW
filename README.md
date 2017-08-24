# Multiple-Kernel Based Vehicle Tracking Using 3D Deformable Model and Camera Self-Calibration for 2017 IEEE Smart World NVIDIA AI City Challenge
This repository contains the source codes of Track 1 and Track 2 for the NVIDIA AI City Challenge. 
The source code of Track 1 is built upon the original single shot multibox detector (SSD) by researchers from UNC Chapel Hill. 
The source code of Track 2 developed based on the implementation by the honorable graduates from the Information Processing Lab, Dr. Kuan-Hui Lee, who is currently a research scientist at the Toyota Research Institute, 
and Dr. Chun-Te Chu, who is currently a software development engineer at Microsoft. 

The codes of Track 1 have been tested on NVIDIA DGX station. Dependencies include TensorFlow, NumPy, OpenCV and matplotlib. 
The codes of Track 2 have been tested on Windows 8.1 x64, Microsoft Visual Studio 2010 - Professional, C++ 11, OpenCV 2.4.3.

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

### Single Shot MultiBox Detector (SSD) and Multiscale YOLO

SSD has been evaluated on many benchmark datasets, e.g., PASCAL and COCO, which have confirmed its competitive accuracy and computation efficiency in object detection. 
In Track 1 of the NVIDIA AI City Challenge, our team adopts the VGG-based SSD network to train on the joint datasets of aic480 and aic540. 
The network is based on a pre-trained model on ImageNet. 
We set the number of iterations as 200,000 with a batch size of 16. 
A TensorFlow re-implementation of the original Caffe code is used for this challenge. 
We use 512 inputs instead of 300 to enhance detection of small objects like traffic lights. 

Since some classes like bus, motorcycle, pedestrian and bicycle have very few instances in the training dataset, we also adopt the pre-trained model from YOLO9000 to help detect such categories. 
Moreover, a multi-scale testing method is applied in the testing stage to detect far-away small objects. Then we combine the detection results from SSD and YOLO9000 according to ensemble learning. 

The SSD model is described in a [ECCV 2016 paper](https://arxiv.org/pdf/1512.02325.pdf). 
The YOLO9000 model is described in a [CVPR 2017 paper](https://arxiv.org/pdf/1612.08242.pdf).

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

## Installation

### Track 1

For SSD, a TensorFlow re-implementation of the original Caffe code is adopted for this challenge. 
The codes are under `ssd-tensorflow/` folder. 
Dependencies include TensorFlow, NumPy, OpenCV and matplotlib. 
Please carefully read and follow the instructions at https://github.com/balancap/SSD-Tensorflow for any question.

As for multiscale YOLO, the codes are under `multiscale-yolo-scripts-master/` folder. 
Please follow the instructions at the GitHub of our team member, Adwin Jahn, for more details: https://github.com/adwin5/multiscale-yolo-scripts.

### Track 2

Please follow the “How to Build” section in the `ReadMe.pdf` file within the `CMK3dVehTrk/` folder. 

1. In Windows environment (tested on Windows 8.1 x64), install Microsoft Visual Studio 2010 – Professional using `en_visual_studio_2010_professional_web_installer_x86_516532.exe`. Install only necessary components of Visual C++. 
2. Install OpenCV v2.1+ package (tested on OpenCV v2.4.3). Or you can extract `OpenCV-2.4.3.zip` to your target directory. 
3. Open `CMK3dVehTrk.sln`.
4. Set proper directories for OpenCV headers and libraries in VS2010 (for both projects of “CMK3dVehTrk” and “CMK3dVehTrkAPI” in configurations of both “Release” and “Debug”): Right click on the project name(s) in “Solution Explorer” -> Properties -> Select “Platform” of “x64” instead of “Win32” (If this choice does not exist, create one through the “Solution Platform” on top of the homepage) -> Configuration Properties -> VC++ Directories -> Set “Include Directories” and “Library Directories”. The default is “OpenCV-2.4.3” at the root directory of C drive.
5. Also, for “CMK3dVehTrkAPI” only (in configurations of both “Release” and “Debug”), change the working directory: Properties -> Configuration Properties -> Debugging -> Command: Change “$(TargetPath)” into “.\bin\$(ProjectName).exe”; Working Directory: Change “$(ProjectDir)” into “.\bin”. The output directory needs to be changed as well: Properties -> Configuration Properties -> Linker -> General -> Output File: Change “$(OutDir)$(TargetName)$(TargetExt)” into “.\bin\$(ProjectName).exe”.
6. Set the configuration and other inputs in the folder `data/`.
7. Build and run the program.
8. The compiled executable file and outputs are saved in the folder `bin/`.

## Usage

### Track 1

Training of SSD: Please follow the training step at https://github.com/balancap/SSD-Tensorflow with your own dataset. 
Pre-trained SSD model on AIC datasets is provided under the `ssd-tensorflow/checkpoints/` folder. 

To visualize and save the detection results on the pre-trained model, run `detection_demo.ipynb`. 
Change "ckpt_filename", "path" and "output_dir" into your model directorys, image directory and output directory, respectively. 

A multi-scale testing of YOLO9000 is avaiable in the `multiscale-yolo-scripts-master/` folder. 
Please follow the instructions at the GitHub of our team member, Adwin Jahn, for more details: https://github.com/adwin5/multiscale-yolo-scripts.

To combine results from SSD and YOLO9000 into one txt file, run `comb_result.py`. 
You should change "txt_dir1", "txt_dir2" and "img_file" into your own directories, where "txt_dir1" is the txt output from SSD, "txt_dir2" is the txt output from YOLO9000 and "img_file" is the input image.

### Track 2

Please follow the “How to Use” section in the `ReadMe.pdf` file within the `CMK3dVehTrk/` folder. 
Here we give an example about how to use the interface APIs and coding flow. The example is coded in the file “main.cpp”. 

#### Inputs

The vehicle tracking system needs five inputs (included in the folder “data”):
1. configuration file, described later.
2. camera parameter file, described later.
3. background image (can be set in the beginning, or set by each frame processing)
4. ROI image (can be set in the beginning, or set by each frame processing)
This is optional. If there’s no ROI image, ROI covers whole image.
5. video (frame)

#### Prerequisite

The surveillance camera should be well calibrated. The camera parameters file should include camera’s intrinsic parameters and extrinsic parameters, and follow the format as below:

`fx     0    cx    0    fy    cy    0    0    1`

`R[0]  R[1]  R[2]  R[3]  R[4]  R[5]  R[6]  R[7]  R[8]`

`C[0]  C[1]  C[2]`

The file path of the camera parameters is needed to be assign to “InExCamParamPath” within the configuration file.

#### Outputs

In the “bin” folder, the output 3D vehicle model(s) at each frame are saved in the “camID” folder as “carID_frameID.txt”: 
`L, W1, W2, H1, H2, H3, H4, X1, X2, X3, X4, Ta, Translation.x, Translation.y, Translation.z, Rotation.x, Rotation.y, Rotation.z, DeformableRange.ScaleLength, DeformableRange. ScaleWidth, DeformableRange.ScaleHeight`. 
Please read the vehicle modeling paper referred to in our final paper/report to understand the concepts of these parameters. 
The command window output is saved in “log.txt”. 
The output frames with 3D models/tracking results plotted are saved in the folder “results”. 
The FES (Fitness Evaluation Score) for each initialized 3D vehicle model is saved in the folder “fes_score” as “carID_fes_scoreframeID.txt”. 

#### Start Coding

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
Some example results are available under the `ssd-tensorflow/` folder as `ex_1.jpg`, `ex_2.jpg` and `ex_3.jpg`.

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

## Disclaimer

For any question you can contact [Zheng (Thomas) Tang](https://github.com/zhengthomastang).
