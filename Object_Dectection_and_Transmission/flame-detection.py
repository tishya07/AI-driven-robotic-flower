import jetson.inference 
import jetson.utils

# net is a detection network object
# loads custom trained object detection model using TensorRT
net = jetson.inference.detectNet("/jetson-inference/python/training/detection/ssd/models/candle/ssd-mobilenet.onnx", labels="/jetson-inference/python/training/detection/ssd/models/candle/labels.txt", threshold=0.8, input_blob="input_0", output_cvg="scores", output_bbox="boxes")

# sets the video source as live video stream
camera = jetson.utils.videoSource("/dev/video0")

# opens a display window
display = jetson.utils.videoOutput()

command_path = "/home/tishya/flame-detection/command.txt"

while True:
	# img holds the current image frame
	img = camera.Capture()

	# detects the flame
	detections = net.Detect(img)

	# renders the frame with dectection overlays (bounding boxes)
	display.Render(img)
	# displays live FPS performance info on screen
	display.SetStatus("Object Detection | Network {:.0f} FPS".format(net.GetNetworkFPS()))
	
	if len(detections) == 0:
		with open(command_path, "w") as f:
			f.write("c\n")
	else:
		with open(command_path, "w") as f:
			f.write("b\n")


