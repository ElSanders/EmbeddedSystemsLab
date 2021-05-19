#Blinklib : https://pypi.org/project/blynklib/
#smbus2   : https://pypi.org/project/smbus2/
#RTC      : https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
#Sensor   : https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf
from smbus2 import SMBus #pip install smbus2
import blynklib #sudo pip install blynklib

BlynkAuth = ''
blynk = blynklib.Blynk(BlynkAuth)

i2c = SMBus(1) #número de bus i2c
#Device addresses
device = {"HTU21D": 0x40, "DS3231" : 0x68}
print(device["HTU21D"])
#Set up RTC
currentTime = [0x00,0x00,0x20,0x02,0x19,0x05,0x20]
i2c.write_i2c_block_data(device["DS3231"],0x00,currentTime)
oldSeconds = currentTime[0]
while(True):
    currentTime = i2c.read_i2c_block_data(device["DS3231"],0x00,7)
    if abs(currentTime[0] - oldSeconds) >= 10:
        oldSeconds = currentTime[0]
        temp = i2c.read_byte_data(device["HTU21D"],0xE3)
        blynk.virtual_writee(pin,data)