#include <wiringPi.h>
#include <unistd.h>
/*
Copyright 2013 Anton Hammerschmidt

This file is part of PowerPi.

PowerPi is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PowerPi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PowerPi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

// wireless true and false
#define WTRUE 136
#define WFALSE 142

// Pulselength is 300ms
#define PLENGTH 300

// repeat transmission
#define REPEAT 10


#ifndef PICONTROL_H
#define PICONTROL_H

class PiControl {
    private:
        static void sendEther(int, int[]);
        static void printCode(int[]);
    public:
        static void send433Mhz(int, std::string, int);
        static void writeGpio(int, int);
};

#endif
