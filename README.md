## About Project

VobiLock is the most useful system that I have ever done. When you forget your keys or don't want to enter the pin to unlock your place like house or room..

Just scan your finger and unlock it. 

You can manage your fingerprints with advanced Admin Interface.

## Installation

First of all download the codes from [here](https://github.com/furkansimsekli/VobiLock/archive/master.zip)

And then you should install libraries as .zip in **Arduino IDE**. To do that, go to **Arduino IDE** Sketch > Include Library > Add .ZIP Library > _(then browse your library)_

Needed Equipments:

1x [Arduino Uno](https://www.amazon.com.tr/Development-Board-ATmega328P-Arduino-kablolu/dp/B06X9CNQ1D/ref=asc_df_B06X9CNQ1D/?tag=googleshoptr-21&linkCode=df0&hvadid=390206884515&hvpos=&hvnetw=g&hvrand=17068721123673591880&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1012788&hvtargid=pla-901712249029&psc=1) 
(but you can use Mega or Leonardo, you should change your Adafruit Fingerprint Sensor pins)

1x [Breadboard](https://www.direnc.net/tekli-breadboard?language=tr&h=db4996ab&gclid=EAIaIQobChMIy8HHtoKP6QIVzEPTCh1N1gGzEAYYASABEgJ4bvD_BwE)
(We will not use it when the project is over)

1x [FPM10A Fingerprint Sensor](https://www.direnc.net/parmak-izi-okuyucu)

1x [1x4 Keypad](https://www.direnc.net/1x4-tus-takimi?language=tr&h=ac3022db&gclid=EAIaIQobChMIrpObtfGO6QIVhOiaCh3AKQHhEAYYASABEgIvyfD_BwE)

1x [I2C LCD](https://www.robomarket.com.tr/16x2-lcd-ekran-i2c-lehimli-1602-mavi-display?gclid=EAIaIQobChMIgMGDgfKO6QIVSM-yCh1OyA51EAYYAyABEgL8mPD_BwE)
(You can use normal lcd too.. but it would complicated.

And lots of male-male & female-male jumper wires.

## Libraries

[Adafruit Fingerprint Sensor](https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library)

[I2C LCD](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)

Other libraries is already installed with **Arduino IDE**

```
//Libraries
#include <EEPROM.h>
#include "Theme.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
```

## Building

1) The wires which came with fingerprint sensor is not compatible with our Arduino. So you should cut the wires in the middle. And attach your jumper wires there..

2) Connect GND from Arduino to Breadboard's "-" side. We'll use there for Ground pins. After that 5V from Arduino to Breadboard's "+" side. We'll use there for VCC pins.

3) Now time to connect power supply with FPM10A. 

    **VCC --> VCC**   --- && ---   **GND --> GND**

4) And then you should connect FPM10A to directly Arduino pins. **RX --> 3** --- && --- **TX --> 2**

5) Now, you can try your sensor with Arduino. _In Arduino IDE_ go to Examples > Adafruit Fingerprint Sensor > enroll
(You can enroll your fingerprints here)

6) Let's add LCD to our project.. Again GND to GND and VCC to VCC. After that **SDA --> A4** --- and --- **SCL --> A5**

7) We should add our Keypad for control menu. Just leftmost wire goes to GND on Breadboard. Other ones goes to 9,10,11,12 in Arduino _in order_. I edited codes for it.

```
//1x4 Keypad Definitions
#define key1 10 //num1
#define key2 9  //num2
#define key3 12 //num3
#define key4 11 //num4
```
8) I defined door pin as '6' but if you need that you can change it.

```
const int door = 6;
```
9) Now you should attach your locksystem to Arduino. Connect pin 6 to + side of locksystem. And **GND --> GND**

> Make sure you are safe. You should add a relay between _locksystem and your project_ If locksystem pull too much electrical current from Arduino. It could damage your Arduino and make it sick.

> If you don't know how to connect relay, here is a basic schematic

![alt text](https://drive.google.com/open?id=1ZiBvvcaMHGpeMY7PvAYoDRn3adDUoyum "Relay Schematic")


> Note: If you are using Arduino MEGA or Leonardo change these codes. It actually runs UNO too :)

```
// RXPIN is IN from sensor 
// TXPIN is OUT from arduino

#define RXPIN 2// change this to whatever
#define TXPIN 3// change this to whatever

SoftwareSerial mySerial(RXPIN, TXPIN);
```

10) Final step is uploading your code to your Arduino. Don't forget to choose port from tool in **Arduino IDE** 

## Usage

In the first time of using you will scan your finger for ADMIN. Never mind you are going to see what you are going to in LCD. I explained step by step there!


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


## Builder
[VobiLock Telegram Channel](https://t.me/VobiLock)

@furkansimsekli @iamvobi

- Aksaray Science High School Student
