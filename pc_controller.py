# import serial
# import keyboard

# ser = serial.Serial('COM5', 9600)  # Замените 'COM3' на соответствующий COM порт и 9600 на скорость передачи данных, используемую на Arduino

# def start_game():
#     ser.write(b'1')
#     #print("Start game")

# def rosh_killed():
#     ser.write(b'2')
#     #print("rosh_killed")

# def torm_killed():
#     ser.write(b'3')
#     #print("torm_killed")

# keyboard.add_hotkey('F6', start_game)
# keyboard.add_hotkey('F7', rosh_killed)
# keyboard.add_hotkey('F8', torm_killed)
# keyboard.wait('Esc')



import serial
import keyboard
import pystray
from pystray import MenuItem as item
from PIL import Image
ser = serial.Serial('COM5', 9600)

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

def exit_action(icon, item):
    icon.stop()

def reconnect():
    global ser
    ser.close()
    ser = serial.Serial('COM5', 9600)

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
    image = Image.open("Kaixin-Icon_33953.png")  # Замени "icon.png" на путь к своей иконке
    icon = pystray.Icon("name", image, "Title", menu)
    icon.run()

if __name__ == '__main__':
    main()