import cv2

# Some initial parameters
webots_video_file = "./simvid.mp4"
pressure_video_file = "./pressure_plot.mp4"
out_video_name = "./merged_video.mp4"
outfps = 50

# Load the video files
webots_video = cv2.VideoCapture(webots_video_file)
pressure_video = cv2.VideoCapture(pressure_video_file)

# Get webots video metadata
numframes = int(webots_video.get(cv2.CAP_PROP_FRAME_COUNT))
width = int(webots_video.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(webots_video.get(cv2.CAP_PROP_FRAME_HEIGHT))

# Set up video writer
writer = cv2.VideoWriter(out_video_name, cv2.VideoWriter_fourcc(*'mp4v'), outfps, (width, height))

# Iterate through frames
for i in range(numframes):
    print(i)
    # Get webots frame
    success, wb_frame = webots_video.read()
    if not success:
        break
    for j in range(2):
        # Get pressure frame
        success, press_frame = pressure_video.read()
        if not success:
            break
        # Resize it
        press_frame = cv2.resize(press_frame, (550, 550), interpolation=cv2.INTER_AREA)
        # Place it in the webots frame
        wb_frame[510:1060, 60:610] = press_frame
        writer.write(wb_frame)
    

# Do cleanup
#writer.release()
webots_video.release()
pressure_video.release()