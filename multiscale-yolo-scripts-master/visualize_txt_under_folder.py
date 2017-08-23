import cv2
import os
import numpy
# input is the 
# input_folder = "540_val_yolo_txt/"
# output_folder = "540_val_yolo_vis"
# img_folder = "aic_540/540_val/540_val_imgs"

input_folder = "demo/red_540_val/upload/out_txt/"
output_folder = "demo/red_540_val/upload/vis"
img_folder = "demo/red_540_val/upload"

y_len = 540
# import the necessary packages
import numpy as np


# called per image
def merge(subdir, file, out_folder):
	
	out_name = file.split('.')[0]
	img = cv2.imread(img_folder+"/"+out_name+".jpeg")
	
	#iterate through fragments
	frag_txt_name = subdir+out_name+".txt"
	print "* "+frag_txt_name

	# check if the fragment txt exists
	if not os.path.isfile(frag_txt_name):
		print "cannot find "+ str(frag_txt_name)
		return
	f = open(frag_txt_name)
	for line in f:
		obj_list = line.split()#[0:-1]
		if obj_list[0] == 'traffic' or obj_list[0] == 'bicycle' or obj_list[0] == 'bus' or obj_list[0] == 'motorbike' or obj_list[0] == 'person':# or obj_list[0] == 'truck':

			#psudo rule
			if float(obj_list[-1])<0.35:
				continue
			#print obj_list
			min_x, min_y, max_x, max_y = int(obj_list[-5]), int(obj_list[-4]), int(obj_list[-3]), int(obj_list[-2])

			#draw box on the image
			cv2.rectangle(img, (min_x, min_y), (max_x, max_y), (0,0,255), 1)
			cv2.putText(img, obj_list[0]+str(float("%.2f" % float(obj_list[-1]))), (min_x, min_y-12), 0 , 1e-3 *y_len, (0,0,255), 1)
			

	f.close()
	cv2.imwrite(out_folder+"/yolo--"+out_name+".jpg", img)
	

def iterate_under(rootdir, out_folder):
	for subdir, dirs, files in os.walk(rootdir):
		for file in files:
        	#print os.path.join(subdir, file)
			filepath = subdir + os.sep + file
			if filepath.endswith(".txt"):
				print subdir, file
				merge(subdir, file, out_folder)
				#break
iterate_under(input_folder, output_folder)
