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
    leftEye = clone[y:y + h, x:x + w]
    lower = np.array([0, 50, 0])
    upper = np.array([255, 255, 255])
    mask = cv2.inRange(leftEye, lower, upper)

    BORDER_SIZE = 5
    borderedMask = cv2.copyMakeBorder(mask, top=BORDER_SIZE, bottom=BORDER_SIZE, left=BORDER_SIZE, right=BORDER_SIZE,
                                      borderType=cv2.BORDER_CONSTANT,
                                      value=[255, 255, 255])

    # Detect blobs
    params = cv2.SimpleBlobDetector_Params()
    params.minThreshold = 0
    params.maxThreshold = 255
    params.thresholdStep = 50
    params.filterByArea = False
    params.filterByCircularity = False
    params.filterByConvexity = False
    params.filterByInertia = False
    detector = cv2.SimpleBlobDetector_create(params)
    keypoints = detector.detect(borderedMask)
    print(keypoints)

    mask = cv2.drawKeypoints(borderedMask, keypoints, np.array([]), (0, 0, 255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    # visualize all facial landmarks with a transparent overlay
    black = np.zeros(scaled.shape, np.uint8)
    black[:] = (0, 0, 0)
    output = face_utils.visualize_facial_landmarks(black, shape)
    return {
      "scaledInput": scaled,
      "image": output,
      "leftEye": mask
    }
  
  def readEye(self, roi):
    _, threshold = cv2.threshold(roi, 20, 255, cv2.THRESH_BINARY_INV)
    return threshold
  