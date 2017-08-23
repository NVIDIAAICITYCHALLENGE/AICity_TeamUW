
# coding: utf-8

# In[1]:


import numpy as np
import copy
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import os


# In[2]:


def vis_detections(im, rclasses, rscores, rbboxes, thresh=0.5):
    im = im[:, :, (0, 1, 2)]
    fig, ax = plt.subplots(figsize=(12, 12))
    ax.imshow(im, aspect='equal')
    for i in range(len(rclasses)):
        #import pdb; pdb.set_trace()
        bbox = np.zeros(4) 
        bbox[0] = rbboxes[i][0]
        bbox[1] = rbboxes[i][1]
        bbox[2] = rbboxes[i][2]
        bbox[3] = rbboxes[i][3]
        score = rscores[i]
        #import pdb; pdb.set_trace()
        ax.add_patch(
            plt.Rectangle((bbox[0], bbox[1]),
                          bbox[2] - bbox[0],
                          bbox[3] - bbox[1], fill=False,
                          edgecolor='red', linewidth=3.5)
            )
        ax.text(bbox[0], bbox[1] - 2,
                '{:s} {:.3f}'.format(rclasses[i], score),
                bbox=dict(facecolor='blue', alpha=0.5),
                fontsize=14, color='white')

    plt.axis('off')
    plt.tight_layout()
    plt.savefig('b.jpg')


# In[3]:


def get_IOU(bbox1,bbox2,img_shape):
    im1 = np.zeros((int(img_shape[0]),int(img_shape[1])))
    im2 = np.zeros((int(img_shape[0]),int(img_shape[1])))
    im1[int(bbox1[1]):int(bbox1[3]),int(bbox1[0]):int(bbox1[2])] = 1
    im2[int(bbox2[1]):int(bbox2[3]),int(bbox2[0]):int(bbox2[2])] = 1
    im3 = im1+im2
    index = np.where(im3==2)
    overlap_area = index[0].shape[0]
    index2 = np.where(im3>0)
    union_area = index2[0].shape[0]
    overlap_ratio = overlap_area/union_area
    return overlap_ratio


# In[4]:


def get_overlap(bbox1,bbox2,img_shape):
    im1 = np.zeros((img_shape[0],img_shape[1]))
    im2 = np.zeros((img_shape[0],img_shape[1]))
    im1[int(bbox1[1]):int(bbox1[3]),int(bbox1[0]):int(bbox1[2])] = 1
    im2[int(bbox2[1]):int(bbox2[3]),int(bbox2[0]):int(bbox2[2])] = 1
    im3 = im1+im2
    index = np.where(im3==2)
    overlap_area = index[0].shape[0]
    area1 = (bbox1[2]-bbox1[0])*(bbox1[3]-bbox1[1])
    area2 = (bbox2[2]-bbox2[0])*(bbox2[3]-bbox2[1])
    overlap_ratio = max(overlap_area/area1, overlap_area/area2)
    return overlap_ratio


# In[5]:


def combine_bbox(rclasses1, rscores1, rbboxes1, rclasses2, rscores2, rbboxes2):
    overlap_ratio = 0.5
    
    # 1 for SSD and 2 for Yolo
    total_classes = []
    total_scores = []
    total_bboxes = []
    for n in range(len(rclasses1)):
        total_classes.append(rclasses1[n])
        total_scores.append(rscores1[n])
        total_bboxes.append(np.copy(rbboxes1[n][:]))
    
    for n in range(len(rclasses2)):
        
        # other classes
        if rclasses2[n]!='bus' and rclasses2[n]!='traffic light' and rclasses2[n]!='motorbike' and rclasses2[n]!='person' and rclasses2[n]!='bicycle':
            continue
        
        # motorbike
        elif rclasses2[n]=='motorbike' and rscores2[n]>0.2:
            #import pdb; pdb.set_trace()
            replace_flag = 0
            for m in range(len(rclasses1)):
                max_shape = [max(rbboxes1[m][3],rbboxes2[n][3]),max(rbboxes1[m][2],rbboxes2[n][2])]
                
                if rclasses1[m]=='Motorcycle' and get_IOU(rbboxes1[m][:],rbboxes2[n][:],max_shape)>overlap_ratio:
                    replace_flag = 1
                    #print(rbboxes1[0])
                    total_bboxes[m][:] = np.copy(rbboxes2[n][:])
                    #print(rbboxes1[0])
                    break
            if replace_flag==1:
                continue
            else:
                total_classes.append('Motorcycle')
                total_scores.append(rscores2[n])
                total_bboxes.append(rbboxes2[n][:])
        
        # person
        elif rclasses2[n]=='person' and rscores2[n]>0.9:
            #import pdb; pdb.set_trace()
            replace_flag = 0
            for m in range(len(rclasses1)):
                max_shape = [max(rbboxes1[m][3],rbboxes2[n][3]),max(rbboxes1[m][2],rbboxes2[n][2])]
                
                if rclasses1[m]=='Pedestrian' and get_IOU(rbboxes1[m][:],rbboxes2[n][:],max_shape)>overlap_ratio:
                    replace_flag = 1
                    #print(rbboxes1[0])
                    total_bboxes[m][:] = np.copy(rbboxes2[n][:])
                    #print(rbboxes1[0])
                    break
            if replace_flag==1:
                continue
            else:
                total_classes.append('Pedestrian')
                total_scores.append(rscores2[n])
                total_bboxes.append(rbboxes2[n][:])
                
        # bicycle
        elif rclasses2[n]=='bicycle' and rscores2[n]>0.2:
            replace_flag = 0
            for m in range(len(rclasses1)):
                max_shape = [max(rbboxes1[m][3],rbboxes2[n][3]),max(rbboxes1[m][2],rbboxes2[n][2])]
                
                if rclasses1[m]=='Bicycle' and get_IOU(rbboxes1[m][:],rbboxes2[n][:],max_shape)>overlap_ratio:
                    replace_flag = 1
                    #print(rbboxes1[0])
                    total_bboxes[m][:] = np.copy(rbboxes2[n][:])
                    #print(rbboxes1[0])
                    break
            if replace_flag==1:
                continue
            else:
                total_classes.append('Bicycle')
                total_scores.append(rscores2[n])
                total_bboxes.append(rbboxes2[n][:])
        
        # bus
        elif rclasses2[n]=='bus' and rscores2[n]>0.2:
            replace_flag = 0
            for m in range(len(rclasses1)):
                max_shape = [max(rbboxes1[m][3],rbboxes2[n][3]),max(rbboxes1[m][2],rbboxes2[n][2])]
                
                if rclasses1[m]=='Bus' and get_IOU(rbboxes1[m][:],rbboxes2[n][:],max_shape)>overlap_ratio:
                    replace_flag = 1
                    #print(rbboxes1[0])
                    total_bboxes[m][:] = np.copy(rbboxes2[n][:])
                    #print(rbboxes1[0])
                    break
            if replace_flag==1:
                continue
            else:
                total_classes.append('Bus')
                total_scores.append(rscores2[n])
                total_bboxes.append(rbboxes2[n][:])
                
            
        # traffic light
        '''
        elif rclasses2[n]=='traffic light':
            for m in range(len(rclasses1)):
                max_shape = [max(rbboxes1[m][3],rbboxes2[n][3]),max(rbboxes1[m][2],rbboxes2[n][2])]
                if (rclasses1[m]=='TrafficSignal-Green' or rclasses1[m]=='TrafficSignal-Yellow' or rclasses1[m]=='TrafficSignal-Red') and get_IOU(rbboxes1[m][:],rbboxes2[n][:],max_shape)>overlap_ratio:
                    ratio1 = (rbboxes1[m][3]-rbboxes1[m][1])/(rbboxes1[m][2]-rbboxes1[m][0])
                    ratio2 = (rbboxes2[n][3]-rbboxes2[n][1])/(rbboxes2[n][2]-rbboxes2[n][0])
                    
                    if abs(ratio2-3)<abs(ratio1-3):
                        total_bboxes[m][:] = np.copy(rbboxes2[n][:])
                        break
        '''
    return total_classes, total_scores, total_bboxes


# In[22]:


def read_txt_bbox(file_name):
    number_set = ['0','1','2','3','4','5','6','7','8','9']
    file = open(file_name, 'r') 
    rows = file.readlines()
    N_rows = len(rows)
    classes = []
    bboxes = []
    scores = []
    for n in range(N_rows):
        temp_line = rows[n]
        line_length = len(temp_line)
        class_name = []
        bbox = np.zeros(4)
        score = 0
        value_idx = 0
        for k in range(line_length):
            if len(class_name)==0 and temp_line[k]==' ' and temp_line[k+1] in number_set:
                class_name = temp_line[:k]
                start_idx = k+1
                continue
            if len(class_name)!=0 and temp_line[k]==' ' and value_idx==0:
                bbox[0] = int(float(temp_line[start_idx:k]))
                value_idx = value_idx+1
                start_idx = k+1
                continue
            if len(class_name)!=0 and temp_line[k]==' ' and value_idx==1:
                bbox[1] = int(float(temp_line[start_idx:k]))
                value_idx = value_idx+1
                start_idx = k+1
                continue
            if len(class_name)!=0 and temp_line[k]==' ' and value_idx==2:
                bbox[2] = int(float(temp_line[start_idx:k]))
                value_idx = value_idx+1
                start_idx = k+1
                continue
            if len(class_name)!=0 and (temp_line[k]==' ' or temp_line[k]=='\n') and value_idx==3:
                #import pdb; pdb.set_trace()
                bbox[3] = int(float(temp_line[start_idx:k]))
                value_idx = value_idx+1
                start_idx = k+1
                continue
            if len(class_name)!=0 and temp_line[k]=='\n':
                score = float(temp_line[start_idx:k])

        classes.append(class_name)
        bboxes.append(bbox)
        scores.append(score)
    
    return classes, bboxes, scores


# In[57]:


#txt_dir1 = 'D:/Code/AI city/aic_540/540_val_label/stevens_cr_winchester_20170607_1_02538.txt'
#img_file = 'D:/Code/AI city/aic_540/540_val_imgs/stevens_cr_winchester_20170607_1_02538.jpeg'


txt_dir1 = 'D:/Code/AI city/SSD_detection/540_test/walsh_santomas_20170603_027_00572.txt'
txt_dir1 = 'D:/Code/AI city/Yolo_detection/540_test_nms_txt/walsh_santomas_20170603_027_00572.txt'
img_file = 'D:/Code/AI city/aic_540/540_test_imgs/walsh_santomas_20170603_027_00572.jpeg'
img = mpimg.imread(img_file)
classes1, bboxes1, scores1 = read_txt_bbox(txt_dir1)
classes2, bboxes2, scores2 = read_txt_bbox(txt_dir2)
total_classes, total_scores, total_bboxes = combine_bbox(classes1, scores1, bboxes1, classes2, scores2, bboxes2)
vis_detections(img, total_classes, total_scores, total_bboxes, thresh=0.5)

