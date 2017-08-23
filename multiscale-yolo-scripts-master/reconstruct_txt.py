import make_focus
import cv2

print make_focus.infile
y_len,x_len = make_focus.y_len, make_focus.x_len
img = make_focus.img
x_win, y_win = make_focus.x_win, make_focus.y_win
out_folder, out_name = make_focus.out_folder, make_focus.out_name.split(".")[0]
print y_len, x_len, x_win, y_win
multi_imgs_txt_folder = out_folder+"out_txt/"
print multi_imgs_txt_folder
scale = make_focus.scale
#iterate through fragments
for i in range(scale*scale):
	frag_txt_name = multi_imgs_txt_folder+str(i)+out_name+".txt"
	print "* "+frag_txt_name
	f = open(frag_txt_name)
	for line in f:
		obj_list = line.split()#[0:-1]
		if obj_list[0] == 'traffic' or obj_list[0] == 'bicycle' or obj_list[0] == 'bus' or obj_list[0] == 'motorbike' or obj_list[0] == 'person' or obj_list[0] == 'truck':
			print obj_list
			min_x, min_y, max_x, max_y = int(obj_list[2]), int(obj_list[3]), int(obj_list[4]), int(obj_list[5])
			#draw bdbox
			offset_x = (i%3)*x_win
			offset_y = (i/3)*y_win
			left = offset_x + min_x
			top = offset_y + min_y
			right = offset_x+max_x
			bot = offset_y + max_y
			cv2.rectangle(img, (left, top), (right, bot), (255,0,0), 1)
			
	f.close()
cv2.imwrite(out_folder+"miltiscale--"+out_name+".jpg", img)
