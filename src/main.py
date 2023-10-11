from modes import Mode
from connection import Connection
from enum import Enum
import time

IDLE_MODE_TIME = 90
ATTRACT_MODE_TIME = 30

# Controller class for Halloween 2023 project
class Halloween2023:

  # Initialize the control program
  def __init__(self):

    print("Initializing Halloween 2023 program!")
    this._connection = Connection()
    print("Successfully initialized Halloween 2023 program!")
  
  # Set the prorgam mode
  def setMode(self, mode):
    self._mode = mode

  # Get the current program mode
  def getMode(self):
    return self._mode

  # Run program loop
  def run(self):
    while True:
      pass


if __name__ == "__main__":
  program = Halloween2023()
  program.run()
