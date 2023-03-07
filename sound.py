import winsound
import serial

ser = serial.Serial('COM9', baudrate=9600, timeout=1)

def notification():
    print("Cycle Finished!")
    winsound.Beep(880, 800)
    winsound.Beep(880, 300)
    winsound.Beep(880, 300)
    winsound.Beep(880, 800)

centrifugetime = 0

while(1):
    while centrifugetime == 0:
        setspeed = input("Input desired set speed in RPM: ")
        centrifugetime = input("Input desired centrifuge spin cycle time in seconds: ")
        message = str('<' + setspeed + ',' + centrifugetime + '>')
        ser.write(str(message).encode('ascii'))

    #play notification sound when cycle is complete
    arduinoData = ser.readline().decode('ascii')
    if arduinoData == "Cycle Complete!":
        notification()

        user = input("Start new cycle (y/n)? ")
        if user == "y":
            centrifugetime = 0
        if user == "n":
            break
