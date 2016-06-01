# eldo
eldo is a small utility to send Elro AB440 commands via Commandline. It's based on Anton Hammerschmidt's PowerPi 0.1.

eldo is short for: elro do

It uses WiringPi and is available as a static and dynamically linked linux binary.

## Quick Install (ARM only)

Install the static binary is recommended for the Raspberry Pi

    wget -O eldo https://github.com/MarvAmBass/eldo/raw/master/bin/eldo-static && chmod a+x eldo && sudo mv eldo /bin/eldo
  
## Install a Sender on the Raspberry Pi:

On Rev 1 & Rev 2:

5V / VCC / POWER    = PIN 4  
GROUND / GND / GRND = PIN 6  
DATA / GPIO 17      = PIN 11  

_More about the Raspberry Pi Pins: http://www.raspberrypi-spy.co.uk/2012/06/simple-guide-to-the-rpi-gpio-header-and-pins/_
  
## Usage

If you want to send codes, superuser rights are required.

We need to know on which GPIO we connected the DATA Channel to the Sender and the AB440 ID of the switch.

To turn on the device with the AB440 ID: 11011B after we connected the sender to GPIO 17.

    sudo eldo 17 11011B:1
    
And to turn it off
    
    sudo eldo 17 11011B:0
    
Syntax:   gpio_number ab440_code:on_off_integer
    
## Tested with Hardware:

* http://www.amazon.de/dp/B00R2U8OEU
