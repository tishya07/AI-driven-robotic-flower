import time
import bluetooth

target_name = "HC-05"  # Replace with your module name
uuid = "00001101-0000-1000-8000-00805F9B34FB"

print("Searching for Bluetooth device...")
target_address = "00:14:03:05:08:73"

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((target_address, 1))
print("Connected to", target_address)

prev_cmd = ""

while True:
    try:
        with open("/home/tishya/flame-detection/command.txt", "r") as f:
            cmd = f.read().strip()
            if cmd != prev_cmd:
                sock.send(cmd + "\n")
                print("Sent:", cmd)
                prev_cmd = cmd
    except FileNotFoundError:
        print("Waiting for command file...")
    
    time.sleep(5)

