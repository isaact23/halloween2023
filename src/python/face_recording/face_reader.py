from collections import OrderedDict
import cv2, dlib, imutils
import numpy as np
from imutils import face_utils

FACIAL_LANDMARKS_IDXS = OrderedDict([
	("mouth", (48, 68)),
	("right_eyebrow", (17, 22)),
	("left_eyebrow", (22, 27)),
	("right_eye", (36, 42)),
	("left_eye", (42, 48)),
	("nose", (27, 35)),
	("jaw", (0, 17))
])

# Class for interpreting the face picture and getting values.
class FaceReader:

  def __init__(self, shape_predictor):
    self.detector = dlib.get_frontal_face_detector()
    self.predictor = dlib.shape_predictor(shape_predictor)

  def readFace(self, frame):
    scaled = imutils.resize(frame, width=500)
    gray = cv2.cvtColor(scaled, cv2.COLOR_BGR2GRAY)
    rects = self.detector(gray, 1)

    # Only proceed if exactly one face is detected
    if len(rects) != 1:
      return None
    
    rect = rects[0]

    # determine the facial landmarks for the face region, then
    # convert the landmark (x, y)-coordinates to a NumPy array
    shape = self.predictor(gray, rect)
    shape = face_utils.shape_to_np(shape)

    LEFT_EYE_IDXS  = face_utils.FACIAL_LANDMARKS_IDXS.get("left_eye")
    RIGHT_EYE_IDXS = face_utils.FACIAL_LANDMARKS_IDXS.get("right_eye")

    clone = scaled.copy()
    (x, y, w, h) = cv2.boundingRect(np.array([shape[LEFT_EYE_IDXS[0] : LEFT_EYE_IDXS[1]]]))
    MARGIN = 10
    leftEye = clone[y:y + h, x:x + w]
    #leftEye = clone[y - MARGIN:y + h + MARGIN, x - MARGIN:x + w + MARGIN]

    """blurred = cv2.blur(leftEye, (3, 3))
    circles = cv2.HoughCircles(blurred,
      cv2.HOUGH_GRADIENT, 1, 20, param1 = 50, 
      param2 = 30, minRadius = 10, maxRadius = 40) 
    
    if circles is not None:
      circles = np.uint16(np.around(circles))
      for pt in circles[0, :]:
        a, b, r = pt[0], pt[1], pt[2]
        cv2.circle(leftEye, (a, b), r, (0, 255, 0), 2)"""

    # visualize all facial landmarks with a transparent overlay
    black = np.zeros(scaled.shape, np.uint8)
    black[:] = (0, 0, 0)
    facialLandmarks = face_utils.visualize_facial_landmarks(black, shape)
    return {
      "scaledInput": scaled,
      "facialLandmarks": facialLandmarks,
      "leftEye": leftEye
    }
  
  def readEye(self, roi):
    _, threshold = cv2.threshold(roi, 20, 255, cv2.THRESH_BINARY_INV)
    return threshold
  