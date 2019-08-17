# Hamlab-C25-Module-I2C-control-from-arduino
Control filters and PTT command with I2C bus from arduino,

  F4HJH - August 2019
  Sends I2C commands to the Charly 25AB device on the I2C bus interface of the Arduino board
  Display state of band filter and PTT state with LEDs
  thanks to F6EHJ for his contributions

B00000001   6m        Port 1 : 0x03
B00000010   10/12m    Port 1 : 0x03
B00000100   15/17m    Port 1 : 0x03
B00001000   20m       Port 1 : 0x03
B00010000   30/40m    Port 1 : 0x03
B00100000   60/80m    Port 1 : 0x03
B01000000   160m      Port 1 : 0x03

Bxx11xxxx   PTT ON    Port 0 : 0x02
Bxx00xxxx   PTT OFF   Port 0 : 0x02

(Per Amp and Att functions not implemented yet)
