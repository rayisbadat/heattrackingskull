# Parts List
* Cheap Plastic Hollow Skull
* [Adafruit Feather](https://www.adafruit.com/product/3619)
* [Adafruit Feather PWM Sheild](https://www.adafruit.com/product/2928)
* [PWM Servo](https://www.adafruit.com/product/2307)
* [RGB LED](https://www.amazon.com/Tricolor-Diffused-Multicolor-Electronics-Components/dp/B01CI6EWHK/)
* [Cheap Fake Gems]()
* [Power Cable](https://www.amazon.com/GOOKYO-Charger-Switching-Replacement-Adapter/dp/B08VRZJV7B/)
* Some barrel plugs, male and female ends
* Some Qwicc to whatever connectors
    * [STEMMA QT / Qwiic JST SH 4-pin Cable with Female Sockets](https://www.adafruit.com/product/4397) 
    * [STEMMA QT / Qwiic JST SH 4-pin Cable with Male Pins](https://www.adafruit.com/product/4209)
* Optional: [LCD Screen]()


# Setup
* Obtain a cheap plastic decorative skull.  
* Drill holes for LED eyes
* Drill holes for heat sensor in nose (or elsewhere)
* Wire up / Stack the feather and the pwm sheild.
* Attach the heat sensor
* Attach 1 motor to PWM 0
* Attach LEDs,
    * Left eye  is PWM 1(r) 2(g) 3(b)
    * Right eye is PWM 4(r) 5(g) 6(b)
* Optional: Attach the LCD
    * For testing purposes the ino file with LED in the name was a debug version.  It allows u to see whats being seen by the sensor
* Wire up power
    * I used the ALED modular plug, with two wires comign out the terminal end. One went into the 5V input on the PWM sheild.  The other was another barrel plug i had laying around, so i could connect the modular usb connector that came with this power cord.
* Test all is working
    * I didnt actually solder anythign togethor. Its all held togethor with qwik connectors and tape.  I wasnt sure if this would be permanent or pulled apart for parts after halloween.
* Drill whole partially through gems, same diameter as ur LEDs
* Push leds through eye holes, use gems to get depth right, hot glue leds in place. Then hot glue gems over.
* Push camera through, secure with hot glue. WARNING: Dont get glue  on the lens.
* Take one of the servo  arms/horns, attach to bottom of skull (screws and hotglue)
* I mounted the skull to a 4" lazy suzan with screws and got glue.  Then mounted the lazy susan to a cardboard box.  Be aware that the lazy susan has a "up" and "down".  The middle ring need to be attached to the "bottom" plate, otherwise ur hotglue gums up the works.
* Attach the servo to some kind of base, i used a small cardboard box.  
* Mount the lazy susan above the motor, snapping the horn/arm onto the sproket.
    * Regardless of what you mount the lazy suzan to, make sure the moto mount is high/low enough for everything to sit/snap togethor properly.


# Upstream Resources
  I learned how to do thsi by using a couple Adafruit examples, namely:
* https://learn.adafruit.com/adafruit-pybadge/overview
    * https://github.com/adafruit/Adafruit_MLX90640/tree/master/examples
* https://learn.adafruit.com/adafruit-8-channel-pwm-or-servo-featherwing
    * https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

# Pinouts
* Stemma QT / Qwiic pinouts
    * Red - 3.3VDC Power
    * Black - Ground
    * Blue - I2C SDA Data
    * Yellow - I2C SCL Clock

* Servo Pinouts
    * Brown - GND
    * Red - 5v 
    * Yellow - Signal/PWM

* RGB LEDs
    * 1 - Red
    * 2 - GND (Longest Pin)
    * 3 - Green
    * 4 - Blue
