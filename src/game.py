from modes import Mode
from connection import Connection
from settings import *

from enum import Enum
import time

# Logic controller / mode manager for the Halloween 2023 project
class Game:

  # Initialize the control program
  def __init__(self):

    print("Initializing Halloween 2023 program!")
    print("")

    self._connection = Connection()
    self.setMode(Mode.IDLE)
  
  # Set the prorgam mode
  def setMode(self, mode):
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
      match self._mode:
        case Mode.IDLE:
          self._runIdle()
        case Mode.ATTRACT:
          self._runAttract()
        case Mode.WELCOME:
          self._runWelcome()
        case Mode.SCARE:
          self._runScare()
  
  # Switch modes if people are approaching or near
  def _checkForPeople(self):
    if self._connection.isPersonClose():
      self.setMode(Mode.SCARE)

    elif self.getMode() != Mode.WELCOME and self._connection.isPersonApproaching():
      self.setMode(Mode.WELCOME)

  
  # Run idle mode
  def _runIdle(self):
    if self.getModeTime() > IDLE_MODE_TIME:
      self.setMode(Mode.ATTRACT)
      return
    
    self._checkForPeople()
  
  # Run attract mode
  def _runAttract(self):
    if self.getModeTime() > ATTRACT_MODE_TIME:
      self.setMode(Mode.IDLE)
      return
    
    self._checkForPeople()
  
  # Run welcome mode
  def _runWelcome(self):
    if self.getModeTime() > WELCOME_MODE_TIME:
      self.setMode(Mode.ATTRACT)
      return
    
    self._checkForPeople()
  
  # Run scare mode
  def _runScare(self):
    if self.getModeTime() > SCARE_MODE_TIME:
      self.setMode(Mode.IDLE)
