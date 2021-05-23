#Blinklib : https://pypi.org/project/blynklib/
#smbus2   : https://pypi.org/project/smbus2/
#RTC      : https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
#Sensor   : https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf

from smbus2 import SMBus #pip install smbus2
import blynklib #sudo pip install blynklib
import time
import board
from adafruit_htu21d import HTU21D #pip install adafruit-circuitpython-htu21d

BlynkAuth = 'yfW9zija6MMG9q5DKPBM8JRalVZPX845'
blynk = blynklib.Blynk(BlynkAuth)
i2c = SMBus(1) #número de bus i2c
device = {"HTU21D": 0x40,
          "DS3231" : 0x68
          }
sensor = HTU21D(board.I2C())
setTemp = 0
temp = 0

pins = {"Temp" : 0,
        "Humidity" : 2,
        "Hours" :10,
        "Minutes" : 11,
        "Seconds" : 12,
        "Heating" : 3,
        "Cooling" : 4
        }

def readSensor():
    return[sensor.temperature,sensor.relative_humidity]

def setupRTC():
    currentTime = [0x00,0x36,0x15,0x02,0x19,0x05,0x20]
    i2c.write_i2c_block_data(device["DS3231"],0x00,currentTime)

@blynk.handle_event('write V1')
def changeSetTemp(pin, value):
    global setTemp
    print("Temp set ",value)
    setTemp = int(value[0])

setupRTC()
oldSeconds = 0
while(True):
    blynk.run()
    currentTime = i2c.read_i2c_block_data(device["DS3231"],0x00,18)
    if abs(currentTime[0] - oldSeconds) >= 1:
        oldSeconds = currentTime[0]
        temp,humidity = readSensor()
        blynk.virtual_write(pins["Temp"],format(temp,".2f"))
        blynk.virtual_write(pins["Humidity"],format(humidity,".2f"))
    if(setTemp>temp):
        blynk.virtual_write(pins["Heating"],255)
        blynk.virtual_write(pins["Cooling"],0)
    elif(setTemp<=temp):
        blynk.virtual_write(pins["Heating"],0)
        blynk.virtual_write(pins["Cooling"],255)
    blynk.virtual_write(pins["Seconds"],hex(currentTime[0])[2:])
    blynk.virtual_write(pins["Minutes"],hex(currentTime[1])[2:])
    blynk.virtual_write(pins["Hours"],  hex(currentTime[2])[2:])
    