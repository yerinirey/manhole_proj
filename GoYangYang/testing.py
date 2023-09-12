# import serial.tools.list_ports
# ports = serial.tools.list_ports.comports()

# for i in ports:
#     print(i.name)
#     print(i.description)
    
import serial
print('serial ' + serial.__version__)

PORT = "COM8"
BaudRate = 9600

ARD = serial.Serial(PORT, BaudRate)

def Decode(response):
    response = response[:len(response) - 2].decode() # 문자열 끝에 붙는 \r\n 슬라이싱
    response = str(response).split(',')
    # ['LIGHT ON/off", 'a.acceleration.x', 'a.acceleration.y', 'a.acceleration.z']
    state = response[0]
    accel=  response[1:]
    if state == "LIGHT ON":
        return f"기울임이 감지되었습니다: {accel}"
    elif state == "off":
        return ". . ."
    else:
        return "Wrong access from _Decode_"

def Ardread():
    if ARD.readable():
        LINE = ARD.readline()
        code = Decode(LINE)
        print(code)
        return code
    else:
        print("읽기 실패 from _Ardread_")
        
while(True):
    Ardread()