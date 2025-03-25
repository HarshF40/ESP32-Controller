import socket
import Gamepad as gp
import time

def translate(value):
    # Figure out how 'wide' each range is
    leftSpan = 2 
    rightSpan = 255

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value -(-1)) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return 0 + (valueScaled * rightSpan)

gamepadType = gp.Xbox360
if not gp.available():
    print('Please connect your gamepad...')
    while not gp.available():
        time.sleep(1.0)
gamepad = gamepadType()
print('Gamepad connected')
host = '172.26.18.134'
port = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.connect((host, port))
value1 = 0
value2 = 0
while True:
  eventType, control, value = gamepad.getNextEvent()
 # print(eventType, control, value)
  if control == 'LT':
       value1 = translate(value)
  if control == 'RT':
       value2 = translate(value)
  print(int(value1),int(value2))
  sock.sendall(f"{value1},{value2}\n".encode())
 # response = input("Send new values? (y/n): ")
  if control == 'A' and value:
      print("A button pressed. Exiting loop.")
      break
sock.close()
