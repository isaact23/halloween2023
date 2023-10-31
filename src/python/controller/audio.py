import os
from modes import Mode
from pygame import mixer

class AudioPlayer:

  def __init__(self, audioDir):
    self._audioDir = audioDir
    mixer.init(44100, -16, 1, 1024)
    mixer.music.set_volume(1)

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
    self._playMusic("Graveyard.mp3")

  def _attract(self):
    self._playMusic("Right Behind You.mp3")
  
  def _welcome(self):
    self._playMusic("Graveyard.mp3")

  def _scare(self):
    self._playMusic("Flowey.mp3")

  def _stopAudio(self):
    mixer.stop()
    mixer.music.stop()
  
  def _getAudioPath(self, audio):
    return os.path.join(self._audioDir, audio)
  
  def _playSound(self, sound):
    path = self._getAudioPath(sound)
    mixer.Sound(path).play()

  def _playMusic(self, music):
    path = self._getAudioPath(music)
    mixer.music.load(path)
    mixer.music.play(-1)
