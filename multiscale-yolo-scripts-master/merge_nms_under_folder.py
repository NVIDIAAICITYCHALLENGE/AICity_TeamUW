import cv2
import os
import numpy
# input is the pyramid 0~8 txt

# for 540 val
input_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_val_pyramid_imgs/out_txt"
output_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_val_pyramid_imgs/merge_vis"
txt_out_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_val_pyramid_imgs/nms_txt"
img_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/aic_540/540_val/540_val_imgs"

# for 540 test
# input_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_test_pyramid_imgs/out_txt"
# output_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_test_pyramid_imgs/merge_vis"
# txt_out_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/540_test_pyramid_imgs/nms_txt"
# img_folder = "/home/adwin/Desktop/NV_ai_city/adwin_NV_test/aic_540/540_test/540_test_imgs"


#y_len,x_len = 480, 720
y_len,x_len = 540, 960
scale = 3
y_win = y_len/(scale+1)
x_win = x_len/(scale+1)


# import the necessary packages
import numpy as np

#  Felzenszwalb et al.
def non_max_suppression_slow(boxes, overlapThresh):
	# if there are no boxes, return an empty list
	if len(boxes) == 0:
		return []

	# initialize the list of picked indexes
	pick = []

	# grab the coordinates of the bounding boxes
	x1 = boxes[:,0]
	y1 = boxes[:,1]
	x2 = boxes[:,2]
	y2 = boxes[:,3]

	# x1 = map(int, x1)
	# y1 = map(int, y1)
	# x2 = map(int, x2)
	# y2 = map(int, y2)
	
	# print x1, y1, x2, y2
	# compute the area of the bounding boxes and sort the bounding
	# boxes by the bottom-right y-coordinate of the bounding box
	area = (x2 - x1 + 1) * (y2 - y1 + 1)
	idxs = np.argsort(y2)

	# keep looping while some indexes still remain in the indexes
	# list
	while len(idxs) > 0:
		# grab the last index in the indexes list, add the index
		# value to the list of picked indexes, then initialize
		# the suppression list (i.e. indexes that will be deleted)
		# using the last index
		last = len(idxs) - 1
		i = idxs[last]
		pick.append(i)
		suppress = [last]

		# loop over all indexes in the indexes list
		for pos in xrange(0, last):
			# grab the current index
			j = idxs[pos]
 
			# find the largest (x, y) coordinates for the start of
			# the bounding box and the smallest (x, y) coordinates
			# for the end of the bounding box
			xx1 = max(x1[i], x1[j])
			yy1 = max(y1[i], y1[j])
			xx2 = min(x2[i], x2[j])
			yy2 = min(y2[i], y2[j])
 
			# compute the width and height of the bounding box
			w = max(0, xx2 - xx1 + 1)
			h = max(0, yy2 - yy1 + 1)
 
			# compute the ratio of overlap between the computed
			# bounding box and the bounding box in the area list
			overlap = float(w * h) / area[j]
 
			# if there is sufficient overlap, suppress the
			# current bounding box
			if overlap > overlapThresh:
				suppress.append(pos)
 
		# delete all indexes from the index list that are in the
		# suppression list
		idxs = np.delete(idxs, suppress)
 
	# return only the bounding boxes that were picked
	return boxes[pick]

# called per image
def merge(subdir, file, out_folder):
	bd_array = []
	info_array = []
	out_name = file[1:]
	out_name = out_name.split('.')[0]
	img = cv2.imread(img_folder+"/"+out_name+".jpeg")
	img2 = np.copy(img)
	#iterate through fragments
	for i in range(scale*scale):
		frag_txt_name = subdir+"/"+str(i)+out_name+".txt"
		print "* "+frag_txt_name

		# check if the fragment txt exists
		if not os.path.isfile(frag_txt_name):
			print "cannot find "+ str(frag_txt_name)
			continue
		f = open(frag_txt_name)
		for line in f:
			obj_list = line.split()#[0:-1]
			if obj_list[0] == 'traffic' or obj_list[0] == 'bicycle' or obj_list[0] == 'bus' or obj_list[0] == 'motorbike' or obj_list[0] == 'person':# or obj_list[0] == 'truck':
				#psudo rule
				if float(obj_list[-1])<0.35:
					continue

				#print obj_list
				min_x, min_y, max_x, max_y = int(obj_list[-5]), int(obj_list[-4]), int(obj_list[-3]), int(obj_list[-2])
				#draw bdbox
				offset_x = (i%3)*x_win
				offset_y = (i/3)*y_win
				left = offset_x + min_x
				top = offset_y + min_y
				right = offset_x+max_x
				bot = offset_y + max_y

				#draw box on the image
				#cv2.rectangle(img, (left, top), (right, bot), (0,255,0), 1)
				#cv2.putText(img, obj_list[0]+str(float("%.2f" % float(obj_list[-1]))), (left, top-12), 0 , 1e-3 *y_len, (0,255,0), 1)
				bd_array.append((left, top, right, bot))
				info_array.append((left, top, right, bot, obj_list[0], obj_list[-1]))

		f.close()
	#cv2.imwrite(out_folder+"/miltiscale--"+out_name+".jpg", img)
	
	#print bd_array
	nms = non_max_suppression_slow(np.array(bd_array), 0.3)

	# write to nms-multiscale--***.txt

	nms_file = open(txt_out_folder+"/nms-miltiscale--"+out_name+".txt",'w') 

	# draw based on nms result
	for box in nms:
		cv2.rectangle(img2, (box[0], box[1]), (box[2], box[3]), (0,0,255), 1)
		id_ = bd_array.index((box[0], box[1], box[2], box[3]))

		# reverse to find id (for class and confidence)
		info_ = info_array[id_]
		cv2.putText(img2, info_[4]+str(float("%.2f" % float(info_[-1]))), (box[0], box[1]-12), 0 , 1e-3 *y_len, (0,0,255), 1)
		#bd_array.append((min_x, min_y, max_x, max_y))

		# for txt
		txt_str = info_[4]+" "+str(box[0])+" "+str(box[1])+" "+str(box[2])+" "+str(box[3])+" "+str(info_[-1])
		print txt_str
		nms_file.write(txt_str+"\n") 
	
	# write to image
	cv2.imwrite(out_folder+"/nms-miltiscale--"+out_name+".jpg", img2)
	nms_file.close()


def iterate_under(rootdir, out_folder):
	for subdir, dirs, files in os.walk(rootdir):
		for file in files:
        	#print os.path.join(subdir, file)
			filepath = subdir + os.sep + file
			if filepath.endswith(".txt") and file[0] == '0':
				print subdir, file
				merge(subdir, file, out_folder)
				#break
iterate_under(input_folder, output_folder)
