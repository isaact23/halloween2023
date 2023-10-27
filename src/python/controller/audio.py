import os, multiprocessing
from modes import Mode
from playsound import playsound

class AudioPlayer:

  def __init__(self, audioDir):
    self._audioDir = audioDir
    self._audioProcess = None

  def setMode(self, mode):
    match mode:
      case Mode.STANDBY:
        self._standby()
      case Mode.IDLE:
        self._idle()
      case Mode.ATTRACT:
        self._attract()
      case Mode.WELCOME:
        self._welcome()
      case Mode.SCARE:
        self._scare()

  def _standby(self):
    self._stopAudio()
  
  def _idle(self):
    self._stopAudio()

  def _attract(self):
    self._playAudio("Right Behind You.mp3")
  
  def _welcome(self):
    self._stopAudio()

  def _scare(self):
    self._playAudio("Flowey.mp3")

  def _stopAudio(self):
    if self._audioProcess is not None:
      self._audioProcess.terminate()
      self._audioProcess = None
  
  def _getAudioPath(self, audio):
    return os.path.join(self._audioDir, audio)
  
  def _playAudio(self, audio):
    self._stopAudio()
    self._audioProcess = multiprocessing.Process(target=playsound, args=(self._getAudioPath(audio),))
    self._audioProcess.start()
