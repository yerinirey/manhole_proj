from flask import Flask, render_template
from flask_socketio import SocketIO
import serial

app = Flask(__name__)
socketio = SocketIO(app)
PORT = "COM8"
BaudRate = 9600
db = []

@app.route('/')
def home():
    # ARD = serial.Serial(PORT, BaudRate)
    # LINE = ARD.readline()
    # state, accelX, accelY, accelZ = LINE[:len(LINE) - 2].decode("utf-8").split(',')
    # if state == "LIGHT ON":
    #     message = f"기울임이 감지되었습니다: {accelX, accelY, accelZ}"
    # elif state == "off":
    #     message = ". . ."
    # else:
    #     message = "Wrong access from _Decode_"
    # # code = Ardread(ARD)
    db.append(message)
    return render_template("home.html", DataHtml=db)

@socketio.on('connect')
def handle_connect():
    print("웹 소켓 연결")
    
@socketio.on("disconnect")
def handle_disconnect():
    print("웹 소켓 연결 해제")

@socketio.on("get_sensor_data")
def get_sensor_data():
    ARD = serial.Serial(PORT, BaudRate)
    LINE = ARD.readline()
    state, accelX, accelY, accelZ = LINE[:len(LINE) - 2].decode("utf-8").split(',')
    if state == "LIGHT ON":
        message = f"기울임이 감지되었습니다: {accelX, accelY, accelZ}"
    elif state == "off":
        message = ". . ."
    else:
        message = "Wrong access from _Decode_"
    sensor_data = message
    socketio.emit('sensor_data', sensor_data)
    

@app.route('/location')
def location():
    return render_template("location.html")

if __name__ == "__main__":
    app.run(debug=True)