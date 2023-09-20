from flask import Flask, render_template        # pip install flask
from flask_socketio import SocketIO, emit       # pip install flask_socketio
import serial       #pip install pyserial
app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

PORT = "COM8"
BaudRate = 9600
ARD = serial.Serial(PORT, BaudRate)

@socketio.on('send_to_arduino')
def send_to_arduino(data):
    ARD.write(data.encode())

@app.route('/')
def home():
    return render_template("home.html")


def read_serial():
    stop_flag = False
    while not stop_flag:
        if ARD.readable():
            LINE = ARD.readline()
            data = LINE[:len(LINE) - 2].decode("cp949")
            print(data)
            socketio.emit('arduino_data', data)

if __name__ == "__main__":
    socketio.start_background_task(target=read_serial)
    socketio.run(app)