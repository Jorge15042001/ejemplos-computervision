import cv2
from sys import argv
from time import sleep
import os

# open a camera, all available cameras are listed in consecutive order,,
# on Linux 0-> first camera 2-> second camera 4-> third camera ...
# on Windows 0-> first camera 1-> second camera 3-> third camera ...

# create directory for the images
if not os.path.exists("images/"):
    os.makedirs("images/")

def show_help():
    print("python3 main.py <<numero_de_imagenes>>")
    print("\t numero_de_imagenes es un entero positivo que indice la cantidad de fotos a captura")
    print("\t si no se indica el numero de imagenes el programa permite visualizar las imagenes en tiempo real")
live_stream = False
n_frames  = -1
if len(argv) > 2:
    print("numero de parametros invalidos")
    show_help()
    exit(1)
elif len(argv) == 1:
    live_stream = True
else:
    if not argv[1].isdigit() or int(argv[1]) < 0:
        print("el segundo argumento debe ser un numero mayor a 0")
        show_help()
        exit(1)
    n_frames = int(argv[1])
    live_stream = False

cam = cv2.VideoCapture(0)
# set buffer  size to 1 frame
cam.set(cv2.CAP_PROP_BUFFERSIZE, 1)

# set the resolution of the camera
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cam.set(cv2.CAP_PROP_FRAME_WIDTH,  640)
# enbale/disable automatic exposure
# 1 -> disable
# 3 -> enbale
cam.set(cv2.CAP_PROP_AUTO_EXPOSURE, 3)
# set exposure time
#  cam.set(cv2.CAP_PROP_EXPOSURE, 1000)

# set capture format to MJPG, MJPG is a compressed image format

#  cam.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))

# number of saved frames
img_counter = 0

print("press 'p' to save a frame, 'q' to exit")

if live_stream:
    # main loop to get frames
    while cam.isOpened():
        # get one frame from the camera
        success, frame = cam.read()

        if not success:
            print("failed to grab frame")
            break
        # You can manipulate your image here


        # show the image in the window created previously
        cv2.imshow("demo", frame)

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

else:
    for img_counter in range(n_frames):
        sleep(0.1)
        success, frame = cam.read()
        img_name = f"./images/opencv_frame_{img_counter}.png"
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))


# close the cameras, so they can be used by another program
# this step is not needed however it is good practices
cam.release()

# close all open windows
# this step is not needed however it is good practices
cv2.destroyAllWindows()
