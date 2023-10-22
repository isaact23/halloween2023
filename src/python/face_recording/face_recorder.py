# Based off
# https://www.datacamp.com/tutorial/face-detection-python-opencv
# https://pyimagesearch.com/2017/04/10/detect-eyes-nose-lips-jaw-dlib-opencv-python/
# https://github.com/davisking/dlib-models
# https://www.youtube.com/watch?v=kbdbZFT9NQI&t=959s

import cv2, argparse
from face_reader import FaceReader

COLORS = [(19, 199, 109), (79, 76, 240), (230, 159, 23),
  (168, 100, 168), (158, 163, 32),
  (163, 38, 32), (180, 42, 220)]

# Class for recording face values into a file that can be
# read by Node2 (The Nun) for servo movement.
class FaceRecorder:

  def __init__(self):

    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--shape-predictor", required=True,
      help="path to facial landmark predictor")
    ap.add_argument("-v", "--video", required=False,
      help="Input video to analyze")
    self.args = vars(ap.parse_args())

    self.faceReader = FaceReader(self.args["shape_predictor"])

  
  def record(self):

    # Either use webcam input or read from video depending on parameters
    cap = cv2.VideoCapture(self.args.get("video"))

    while True:

      result, frame = cap.read()
      if result is False:
        print("Video capture failed")
        break

      output = self.faceReader.readFace(frame)
      if output is not None:
        cv2.imshow("Facial Landmarks", output["facialLandmarks"])
        cv2.imshow("Left Eye", output["leftEye"])
        cv2.imshow("Scaled Input", output["scaledInput"])
      
      if (cv2.waitKey(1) & 0xFF == ord("q")):
        print("Quitting program")
        break
    
    cap.release()
    cv2.destroyAllWindows()
