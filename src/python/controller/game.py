# User modules
from modes import Mode
from connection import Connection
from settings import *

# Python library
from enum import Enum
import keyboard, os, time

# Logic controller / mode manager for the Halloween 2023 project
class Game:

  # Initialize the control program
  def __init__(self):

    print("Initializing Halloween 2023 program!")
    print("")

    self._connection = Connection()
    self.setMode(Mode.ATTRACT)
  
  # Set the prorgam mode
  def setMode(self, mode):
    for i in range(2):
      print("")
    print("Switching to " + str(mode))

    self._mode = mode
    self._connection.setMode(mode)
    self._modeStartTime = time.time()

  # Get the current program mode
  def getMode(self):
    return self._mode
  
  # Get the seconds the program has been in its current mode
  def getModeTime(self):
    return time.time() - self._modeStartTime
  
  # Run program loop
  def run(self):

    while True:
      self._runMode()
      self._readKeyboard()
      self._readFromSerial()
  
  # Run loop code specific to a mode
  def _runMode(mode):
    match mode:
      case Mode.IDLE:
        self._runIdle()
      case Mode.ATTRACT:
        self._runAttract()
      case Mode.WELCOME:
        self._runWelcome()
      case Mode.SCARE:
        self._runScare()

  # Read the keyboard and switch modes accordingly.
  def _readKeyboard(self):
    print("", end="")

    if keyboard.is_pressed(IDLE_KEY) and self._mode != Mode.IDLE:
      self.setMode(Mode.IDLE)
    elif keyboard.is_pressed(ATTRACT_KEY) and self._mode != Mode.ATTRACT:
      self.setMode(Mode.ATTRACT)
    elif keyboard.is_pressed(WELCOME_KEY) and self._mode != Mode.WELCOME:
      self.setMode(Mode.WELCOME)
    elif keyboard.is_pressed(SCARE_KEY) and self._mode != Mode.SCARE:
      self.setMode(Mode.SCARE)
  
  # Read from serial.
  def _readFromSerial(self):
    incomingMessage = self._connection.readMessage()
    if incomingMessage != None:
      print("Incoming message: " + incomingMessage, end="")
  
  # Switch modes if people are approaching or near
  def _checkForPeople(self):
    if AUTO_CHANGE_MODE and self.getMode() != Mode.SCARE and self._connection.isPersonClose():
      self.setMode(Mode.SCARE)

    elif AUTO_CHANGE_MODE and self.getMode() != Mode.WELCOME and self._connection.isPersonApproaching():
      self.setMode(Mode.WELCOME)
  
  # Run idle mode
  def _runIdle(self):
    if AUTO_CHANGE_MODE and self.getModeTime() > IDLE_MODE_TIME:
      self.setMode(Mode.ATTRACT)
      return
    
    self._checkForPeople()
  
  # Run attract mode
  def _runAttract(self):
    if AUTO_CHANGE_MODE and self.getModeTime() > ATTRACT_MODE_TIME:
      self.setMode(Mode.IDLE)
      return
    
    self._checkForPeople()
  
  # Run welcome mode
  def _runWelcome(self):
    if AUTO_CHANGE_MODE and self.getModeTime() > WELCOME_MODE_TIME:
      self.setMode(Mode.ATTRACT)
      return
    
    self._checkForPeople()
  
  # Run scare mode
  def _runScare(self):
    if AUTO_CHANGE_MODE and self.getModeTime() > SCARE_MODE_TIME:
      self.setMode(Mode.IDLE)
