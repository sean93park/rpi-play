#### About

5x7 LED dot matrix control test with Raspberry Pi, I2C interface

#### Circuit

* 5x7 LED dot matrix, Common Anode
* MCP23017
  * I2C to 2 x 8bit GPIO Expander
  * A Port: 5 COL, drives PNP TR, L is ON
  * B Port: 7 ROW, drives NPN TR, H is ON
* PNP TR: 2N3906 x 5
* NPN TR: 2N3904 x 7
```
   ---+---- VCC
      |
      |
      |
     e\ |     4.7 KOhm
       \|_____/\/\/\_____ to pin GPA0 ~ GPA4
  PNP  /| b
     c/ |  x5 2N3906
      |
      |
    -----
     \ /   LED 1 pixel
     ---
      |
      /
      \   330 Ohm
      /
      \
      |
      |
     c\ |     4.7 KOhm
       \|____/\/\/\_____ to pin GPB0 ~ GPB6
  NPN  /|b
     e/ |  x7 2N3904
      |
      |
      |
  ----+---- GND
```

* RPi SDL/SDC pin to MCP23017

#### Character Generation

* A port scans from one column at a time
  * bit 0 -> 1 -> 2 -> 4 -> 8
  * Signal Low will turn on column LEDs
  * Value to send will be 0xfe, 0xfd, 0xfb, 0xf7, 0xef for each column
* B port will send 7 bit LED for each column line
  * MSB(bit7) is not used
  * for 'G' character: 0x3E, 0x41, 0x41, 0x51, 0x32
```
  . 0x3E: ..XXXXX.
  . 0x41: .X.....X
  . 0x41: .X.....X
  . 0x51: .X.X...X
  . 0x32: ..XX..X.
```
* Better view with rotation.
```
  . | .XXX. |
  . | X...X |
  . | X.... |
  . | X.... |
  . | X..XX |
  . | X...X |
  . | .XXX. |
```

#### Connecting to Raspberry Pi

Refer [How To Use A MCP23017 I2C Port Expander With The Raspberry Pi – Part 1](http://www.raspberrypi-spy.co.uk/2013/07/how-to-use-a-mcp23017-i2c-port-expander-with-the-raspberry-pi-part-1/)

#### How to build

```
make
```

to clean,
```
make clean
```

Output executable is created at `build/arm-linux/debug/cmake` folder.

#### Copy to Raspberry Pi

Use `scp` command

```
scp build/arm-linux/debug/cmake/ledmatrix pi@(rpi-ip-address):/home/pi/.
```
replace `(rpi-ip-address)` with your RPi IP address.
