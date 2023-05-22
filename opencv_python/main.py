import cv2

# open a camera, all available cameras are listed in consecutive order,,
# on Linux 0-> first camera 2-> second camera 4-> third camera ...
# on Windows 0-> first camera 1-> second camera 3-> third camera ...

cam = cv2.VideoCapture(0)
# set buffer  size to 1 frame
cam.set(cv2.CAP_PROP_BUFFERSIZE, 1)

# set the resolution of the camera
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
cam.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
# enbale/disable automatic exposure
# 1 -> disable
# 3 -> enbale
cam.set(cv2.CAP_PROP_AUTO_EXPOSURE, 1)
# set exposure time
cam.set(cv2.CAP_PROP_EXPOSURE, 100)

# set capture format to MJPG, MJPG is a compressed image format

cam.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))

# create a window and name it 
cv2.namedWindow("opencv demo")

# number of saved frames
img_counter = 0

print("press 'p' to save a frame, 'q' to exit")

# main loop to get frames
while cam.isOpened():
    # get one frame from the camera
    success, frame = cam.read()

    if not success:
        print("failed to grab frame")
        break
    # You can manipulate your image here


    # show the image in the window created previously
    cv2.imshow("opencv demo", frame)

    # read keyboard input, for 1 millisecond
    # returns int with ascii representation of the pressed key
    k = cv2.waitKey(1)
    # convert to char
    k_char = chr(k%256)


    if k_char == 'q': # exit main loop
        print("Escape hit, closing...")
        break
    elif k_char == 'p': # save frame
        # image name
        img_name = f"./images/opencv_frame_{img_counter}.png"
        # save the image
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))
        img_counter += 1

# close the cameras, so they can be used by another program
# this step is not needed however it is good practices
cam.release()

# close all open windows
# this step is not needed however it is good practices
cv2.destroyAllWindows()
