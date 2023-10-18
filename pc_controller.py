import serial
import keyboard
import pystray
from pystray import MenuItem as item
from PIL import Image

import serial.tools.list_ports

arduino_ports = [
    p.name 
    for p in serial.tools.list_ports.comports()
    if "CH340" in p.description
]
ser = serial.Serial(arduino_ports[0], 9600)

def start_game():
    try:
        ser.write(b'1')
    except:
        pass

def rosh_killed():
    try:
        ser.write(b'2')
    except:
        pass
def torm_killed():
    try:
        ser.write(b'3')
    except:
        pass

def sound():
    try:
        ser.write(b'4')
    except:
        pass

def exit_action(icon, item):
    icon.stop()

def reconnect():
    global ser
    ser.close()
    ser = serial.Serial('COM3', 9600)

menu = (
    item('Start game', start_game),
    item('Rosh killed', rosh_killed),
    item('Torm killed', torm_killed),
    item('reconnect', reconnect),
    item('Exit', exit_action)
)

def main():
    keyboard.add_hotkey('F6', start_game)
    keyboard.add_hotkey('F7', rosh_killed)
    keyboard.add_hotkey('F8', torm_killed)
    keyboard.add_hotkey('F3', reconnect)
    keyboard.add_hotkey('F2', sound)
    image = Image.open("Kaixin-Icon_33953.png")  # Замени "icon.png" на путь к своей иконке
    icon = pystray.Icon("name", image, "Title", menu)
    icon.run()

if __name__ == '__main__':
    main()