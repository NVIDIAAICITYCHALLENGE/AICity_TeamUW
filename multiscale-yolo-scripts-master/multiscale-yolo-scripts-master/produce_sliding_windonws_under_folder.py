# produce focus fragments for all the images under folder input_folder
input_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/aic_540/540_test/540_test_imgs"
output_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_test_pyramid_imgs/"


import numpy as np
import cv2
import os

y_len,x_len = 540, 960

#input
#infile = "540_imgs/stevens_cr_winchester_20170607_1_06314.jpeg"

	# #output folder
	# out_folder = "pyramid_yolo/"
def slide_win(infile, out_folder):
	out_name = infile.split('/')[-1]
	# check if the fragment txt exists
	print infile
	if not os.path.isfile(infile):
		print "cannot find "+ str(infile)
		return
	img = cv2.imread(infile)

	#y_len,x_len,_=img.shape
	scale = 3
	y_win = y_len/(scale+1)
	x_win = x_len/(scale+1)
	for y in range(scale):
		for x in range(scale):
			cropped_image = img[y*y_win:(y+2)*y_win, x*x_win:(x+2)*x_win]
			serial = x+y*scale
			cv2.imwrite(out_folder+str(serial)+out_name,cropped_image)

def iterate_under(rootdir, out_folder):
	for subdir, dirs, files in os.walk(rootdir):
		for file in files:
        	#print os.path.join(subdir, file)
			filepath = subdir + os.sep + file
			if filepath.endswith(".jpeg"):
				print filepath
				slide_win(filepath, out_folder)
iterate_under(input_folder, output_folder)
