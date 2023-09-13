from flask import Flask, render_template
import serial

app = Flask(__name__)
PORT = "COM8"
BaudRate = 9600
ARD = serial.Serial(PORT, BaudRate)
db = []

def Decode(response):
    response = response[:len(response) - 2].decode() # 문자열 끝에 붙는 \r\n 슬라이싱
    state, accelX, accelY, accelZ = response.split(',')
    # ['LIGHT ON/off", 'a.acceleration.x', 'a.acceleration.y', 'a.acceleration.z']
    if state == "LIGHT ON":
        return f"기울임이 감지되었습니다: {accelX, accelY, accelZ}"
    elif state == "off":
        return ". . ."
    else:
        return "Wrong access from _Decode_"

def Ardread(ARD):
    if ARD.readable():
        LINE = ARD.readline()
        code = Decode(LINE)
        print(code)
        return code
    else:
       return "읽기 실패 from _Ardread_"       



@app.route('/')
def home():
    code = Ardread(ARD)
    db.append(code)
    return render_template("home.html", DataHtml=db)

@app.route('/location')
def location():
    return render_template("location.html")

if __name__ == "__main__":
    app.run()