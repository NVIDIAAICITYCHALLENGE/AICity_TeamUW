#focus
#sliding window
import numpy as np
import cv2

#input
infile = "540_imgs/stevens_cr_winchester_20170607_1_06314.jpeg"

#output folder
out_folder = "pyramid_yolo/"
out_name = infile.split('/')[-1]
img = cv2.imread(infile)

y_len,x_len,_=img.shape
scale = 3
y_win = y_len/(scale+1)
x_win = x_len/(scale+1)
for y in range(scale):
	for x in range(scale):
		cropped_image = img[y*y_win:(y+1)*y_win, x*x_win:(x+1)*x_win]
		serial = x+y*scale
		cv2.imwrite(out_folder+str(serial)+out_name,cropped_image)



