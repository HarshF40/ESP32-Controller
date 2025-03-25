import socket
import Gamepad as gp
import time

def translate(value):
    # Scale the input value from the range [-1, 1] to [0, 255]
    left_span = 2  # Range width of [-1, 1]
    right_span = 255  # Range width of [0, 255]
    value_scaled = float(value - (-1)) / float(left_span)
    return int(0 + (value_scaled * right_span))

# Initialize gamepad
gamepad_type = gp.Xbox360
if not gp.available():
    print('Please connect your gamepad...')
    while not gp.available():
        time.sleep(1.0)
gamepad = gamepad_type()
print('Gamepad connected')

# UDP server details
udp_ip = '172.26.18.134'  # Replace with your ESP32's IP address
udp_port = 8080

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

value_lt = 0
value_rt = 0

try:
    while True:
        event_type, control, value = gamepad.getNextEvent()
        if control == 'LT':
            value_lt = translate(value)
        elif control == 'RT':
            value_rt = translate(value)
        
        # Prepare and send the message
        message = f"({value_lt},{value_rt})".encode()
        sock.sendto(message, (udp_ip, udp_port))
        
        print(f"Sent: {message.decode()}")

        # Exit loop if 'A' button is pressed
        if control == 'A' and value:
            print("A button pressed. Exiting loop.")
            break
finally:
    sock.close()

