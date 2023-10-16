# Handle connection between this python program and the
# other systems.

from modes import Mode
import serial

class Connection:

  # Initialize connection.
  def __init__(self):
    self._port = None
    self._connected = False
    self._ensureConnected()
    if not self._connected:
      print("Failed to connect to serial.")
  
  # Return whether a person is approaching.
  def isPersonApproaching(self):
    return False

    #raise NotImplementedError("isPersonApproaching not implemented")
  
  # Return whether a person is close.
  def isPersonClose(self):
    return False

    #raise NotImplementedError("isPersonClose not implemented")

  # Update the mode across all systems.
  def setMode(self, mode):
    match mode:
      case Mode.IDLE:
        self.sendMessage("IDLE")
      case Mode.ATTRACT:
        self.sendMessage("ATTRACT")
      case Mode.WELCOME:
        self.sendMessage("WELCOME")
      case Mode.SCARE:
        self.sendMessage("SCARE")
  
  # Send a string over serial connection.
  def sendMessage(self, message):
    self._ensureConnected()

    binMsg = message.encode('ascii')
    if self._connected:
      try:
        self._port.write(binMsg)
      except serial.serialutil.SerialException:
        print("Failed to write message to serial. Disconnected.")
        self._connected = False
    else:
      pass
      #print("Connection not initialized. Tried to send " + message)

  # Return the next incoming line from serial, or None if there is none.
  def readMessage(self):
    self._ensureConnected()

    if self._connected:
      try:
        if self._port.in_waiting > 0:
          return self._port.readline().decode("Ascii")

      except serial.serialutil.SerialException:
        print("Failed to read message from serial. Disconnected.")
        self._connected = False
      
      except UnicodeDecodeError:
        print("Failed to decode message from serial.")

    else:
      #print("Connection not initialized. Cannot read message.")
      pass
    
    return None
  
  # Try to connect to serial.
  def _connect(self):
    if self._connected:
      #print("Already connected.")
      return

    try:
      self._port = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
      self._connected = True

    except serial.serialutil.SerialException:
      #print("Failed to open serial.")
      self._port = None
      self._connected = False
  
  # If not connected, try to reconnect.
  def _ensureConnected(self):
    if not self._connected:
      self._connect()
      if self._connected:
        print("Connected to serial.")
