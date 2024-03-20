from machine import Pin,I2C
import ssd1327

i2c = I2C(sda=Pin("Y10"), scl=Pin("Y9"))
display = ssd1327.QWIIC_128X128_OLED(i2c)

# Test
display.fill(0)
display.text('ABCDEFGHIJKLMNOP',0,0*8,15)
display.text('b Line 2',0,1*8,15)
display.text('c Line 3',0,2*8,15)
display.text('d Line 4',0,3*8,15)
display.text('e Line 5',0,4*8,15)
display.text('f Line 6',0,5*8,15)
display.text('g Line 7',0,6*8,15)
display.text('h Line 8',0,7*8,15)
display.text('i Line 9',0,8*8,15)
display.text('j Line 10',0,9*8,15)
display.text('k Line 11',0,10*8,15)
display.text('l Line 12',0,11*8,15)
display.text('m Line 13',0,12*8,15)
display.text('n Line 14',0,13*8,15)
display.text('o Line 15',0,14*8,15)
display.text('p Line 16',0,15*8,15)
display.show()
