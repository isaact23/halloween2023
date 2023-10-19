# Based off
# https://www.datacamp.com/tutorial/face-detection-python-opencv
# https://pyimagesearch.com/2017/04/10/detect-eyes-nose-lips-jaw-dlib-opencv-python/
# https://github.com/davisking/dlib-models

from collections import OrderedDict
from imutils import face_utils
import cv2, dlib, imutils, argparse
import numpy as np

FACIAL_LANDMARKS_IDXS = OrderedDict([
	("mouth", (48, 68)),
	("right_eyebrow", (17, 22)),
	("left_eyebrow", (22, 27)),
	("right_eye", (36, 42)),
	("left_eye", (42, 48)),
	("nose", (27, 35)),
	("jaw", (0, 17))
])

COLORS = [(19, 199, 109), (79, 76, 240), (230, 159, 23),
        (168, 100, 168), (158, 163, 32),
        (163, 38, 32), (180, 42, 220)]

class FaceRecorder:

  def __init__(self):
    self.faceClassifier = cv2.CascadeClassifier(
      cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
    )

    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--shape-predictor", required=True,
      help="path to facial landmark predictor")
    args = vars(ap.parse_args())
    print(args["shape_predictor"])

    self.detector = dlib.get_frontal_face_detector()
    self.predictor = dlib.shape_predictor(args["shape_predictor"])
  
  # def detectFace(self, frame):

  #   grayImg = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
  #   faces = self.faceClassifier.detectMultiScale(grayImg, 1.1, 5, minSize=(40, 40))

  #   for (x, y, w, h) in faces:
  #     cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 4)

  #   return faces

  def readFace(self, inputFrame):
    image = imutils.resize(inputFrame, width=500)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    rects = self.detector(gray, 1)

    # loop over the face detections
    for (i, rect) in enumerate(rects):

      # determine the facial landmarks for the face region, then
      # convert the landmark (x, y)-coordinates to a NumPy array
      shape = self.predictor(gray, rect)
      shape = face_utils.shape_to_np(shape)

      # loop over the face parts individually
      for (name, (i, j)) in face_utils.FACIAL_LANDMARKS_IDXS.items():

        # clone the original image so we can draw on it, then
        # display the name of the face part on the image
        clone = image.copy()
        cv2.putText(clone, name, (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
          0.7, (0, 0, 255), 2)
        
        # loop over the subset of facial landmarks, drawing the
        # specific face part
        for (x, y) in shape[i:j]:
          cv2.circle(clone, (x, y), 1, (0, 0, 255), -1)
        
        # extract the ROI of the face region as a separate image
        (x, y, w, h) = cv2.boundingRect(np.array([shape[i:j]]))
        print(x, y, w, h)
        roi = image[y:y + h, x:x + w]
        roi = imutils.resize(roi, width=250, inter=cv2.INTER_CUBIC)

        # show the particular face part
        cv2.imshow("ROI", roi)
        cv2.imshow("Image", clone)
        cv2.waitKey(0)
        
      # visualize all facial landmarks with a transparent overlay
      output = face_utils.visualize_facial_landmarks(image, shape)
      cv2.imshow("Image", output)
      cv2.waitKey(0)

  
  def record(self):
    videoCapture = cv2.VideoCapture(0)

    while True:

      result, frame = videoCapture.read()
      if result is False:
        print("Video capture failed")
        break

      self.readFace(frame)

      cv2.imshow("Face Recorder (Halloween 2023)", frame)
      
      if (cv2.waitKey(1) & 0xFF == ord("q")):
        print("Quitting program")
        break
    
    videoCapture.release()
    cv2.destroyAllWindows()
