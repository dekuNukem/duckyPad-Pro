import serial

ser = serial.Serial('COM42', 115200, timeout=1)

ser.write(bytes([0x00]))

ser.close()