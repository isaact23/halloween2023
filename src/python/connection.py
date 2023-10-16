# Handle connection between this python program and the
# other systems.

from modes import Mode
import serial

class Connection:

  # Initialize connection.
  def __init__(self):
    self._port = None
    self._connected = False
    self._connect()
  
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
        self._sendMessage("IDLE")
      case Mode.ATTRACT:
        self._sendMessage("ATTRACT")
      case Mode.WELCOME:
        self._sendMessage("WELCOME")
      case Mode.SCARE:
        self._sendMessage("SCARE")
  
  # Send a string over serial connection.
  def _sendMessage(self, message):
    self._ensureConnected()

    binMsg = message.encode('ascii')
    if self._connected:
      self._port.write(binMsg)
    else:
      print("Connection not initialized. Tried to send " + message)

  # Return the next incoming line from serial, or None if there is none.
  def _readMessage(self):
    self._ensureConnected()

    if self._connected:
      try:
        if self._port.in_waiting > 0:
          return self._port.readline().decode("Ascii")
      except serial.serialutil.SerialException:
        print("Failed to read message.")
        self._connected = False
        return None

    else:
      print("Connection not initialized. Cannot read message.")
    
    return None
  
  # Try to connect to serial.
  def _connect(self):
    if self._connected:
      print("Already connected.")
      return

    try:
      self._port = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
      self._connected = True

    except serial.serialutil.SerialException:
      print("Failed to open serial.")
      self._port = None
      self._connected = False
  
  # If not connected, try to reconnect.
  def _ensureConnected(self):
    if not self._connected:
      self._connect()
