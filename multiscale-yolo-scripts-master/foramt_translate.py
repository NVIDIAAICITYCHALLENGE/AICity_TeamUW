import json
import os

def yoloJson_to_txt(path1, out_folder):
	with open(path1) as data_file:
		data = json.load(data_file)
		#print data
		length_of_objs = len(data)
		# prepared to write to txt
		input_file_name = path1.split("/")[-1]
		input_file_name = input_file_name.split(".")[0]
		file = open(out_folder+input_file_name+".txt",'w') 
		for i in range(length_of_objs):
			data_ = data[i]
			class_str = data_['label']
			min_x, min_y = data_["topleft"]['x'], data_["topleft"]['y']
			max_x, max_y = data_["bottomright"]['x'], data_["bottomright"]['y']
			confidence = data_['confidence']
			txt_str = class_str+" "+str(min_x)+" "+str(min_y)+" "+str(max_x)+" "+str(max_y)+" "+str(confidence)
			print txt_str
			file.write(txt_str+"\n") 
		file.close()

def txt_to_yoloJson(path1, path2):
	pass
def NVJson_to_txt(path1, path2):
	pass

def txt_to_NVJson(path1, path2):
	pass
def iterate_under(rootdir, out_folder):
	for subdir, dirs, files in os.walk(rootdir):
		for file in files:
        	#print os.path.join(subdir, file)
			filepath = subdir + os.sep + file
			if filepath.endswith(".json"):
				print filepath
				yoloJson_to_txt(filepath, out_folder)
def main():
	iterate_under("demo/red_540_val/upload/out/", 'demo/red_540_val/upload/out_txt/')
	#yoloJson_to_txt("adwin_NV_test/out_yolo_thres0_65_jason/indian_river_reon_20140801_00260.json", "")

if __name__ == '__main__':
	main()
