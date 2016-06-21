# eldo
_eldo is a small utility to controll Elro AB440 Switches and other compatible Devices._

It's designed to run as statically linked binary mainly on the Raspberry Pis.

It's based on Anton Hammerschmidt's PowerPi 0.1.

eldo is short for: elro do

It uses WiringPi and is available as a static and dynamically linked linux binary.

## Quick Install (ARM only)

Install the static binary is recommended for the Raspberry Pi 1

    wget -O eldo https://github.com/MarvAmBass/eldo/raw/master/bin/rpi1/eldo-static &&\
    chmod a+x eldo &&\
    sudo mv eldo /bin/eldo

Install wiringPi library and eldo binary for Raspberry Pi 3
  
## Different Modes

### Commandline AB440 Sender

If you want to send codes, superuser rights are required.

We need to know on which GPIO we connected the DATA Channel to the Sender and the AB440 ID of the switch.

To turn on the device with the AB440 ID: 11011B after we connected the sender to GPIO 17.

    sudo eldo 17 11011B:1
    
And to turn it off
    
    sudo eldo 17 11011B:0
    
Syntax: _gpio\_number ab440\_code:on\_off\_integer_

### Multicast Listening Server

You can use this mode to keep eldo running and listening for __UDP__ commands
on port __2112__.

Start the server mode (keep in mind, this doesn't fork to background):

    eldo 17
    
Syntax: _eldo gpio\_number_

Now eldo listens for incomming commands on port __2112/UDP__

UDP Command Syntax

Turn on Device 11011B

    11011B:1
    
And to turn it off

    11011B:0
    
Syntax: _ab440\_code:on\_off\_integer_

### Mutlicast Command Sender

To send messages to all multicast eldo servers, just use the following command:

Turn on Device 11011B:

    eldo send 11011B:1

Turn it off:

    eldo send 11011B:0

Syntax: _eldo send ab440\_code:on\_off\_integer_

#### Multiple Networks

To use UDP Multicast over different Networks you can use the small utility on the routers.

    udp-broadcast-relay -f 1 2112 eth0 eth1

More informations: https://www.joachim-breitner.de/udp-broadcast-relay/

## Hardware

### Installing a Sender on the Raspberry Pi:

On Rev 1 & Rev 2:

5V / VCC / POWER    = PIN 4  
GROUND / GND / GRND = PIN 6  
DATA / GPIO 17      = PIN 11  

_More about the Raspberry Pi Pins: http://www.raspberrypi-spy.co.uk/2012/06/simple-guide-to-the-rpi-gpio-header-and-pins/_
  
### Tested with Hardware:

* http://www.amazon.de/dp/B00R2U8OEU
