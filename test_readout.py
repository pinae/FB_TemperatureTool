import serial
import json

if __name__ == "__main__":
    rs485_serial = serial.Serial(
        port='/dev/ttyUSB0',  # Adjust the serial port as needed. For example on Windows it might be COM3.
        baudrate=9600,
        bytesize=8,
        parity='N',           # No parity bit
        stopbits=1,           # One stop bit
        timeout=1)
    print("Using port {}.".format(rs485_serial.name))
    data = "r".encode('UTF-8') + b'\x00'
    writtenBytes = rs485_serial.write(data)
    print("Just sent {} which were {} bytes.".format(str(data), writtenBytes))
    answer = rs485_serial.readline()
    try:
        answer = str(answer.strip(b'\x00'), encoding='UTF-8')
        print(json.loads(answer))
    except UnicodeDecodeError:
        print("The transmitted string was no valid unicode.")
    rs485_serial.close()
