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

#include "picontrol.h"

void PiControl::printCode(int code[]) {
    printf("Sending ");
    for(int i=0; i<16; i++) {
        printf("%d ", code[i]);
    }
    printf("via Ether\n");
}

void PiControl::sendEther(int gpio, int code[]) {
    if (wiringPiSetupGpio() == -1)
        return;

    pinMode(gpio, OUTPUT);

    int x = 0;
	for(int r=0; r<REPEAT; r++) {
		for(int c=0; c<16; c++) {
			x = 128;
			for(int i=1; i<9; i++) {
				if((code[c] & x) > 0)
					digitalWrite(gpio, HIGH);
				else
					digitalWrite(gpio, LOW);

				usleep(PLENGTH);
            			x = x>>1;
			}
		}
	}
}

void PiControl::send433Mhz(int gpio, std::string codeStr, int active) {
    // Validate code
	if(codeStr.length()!=6){
		printf("The code must be 6 characters long");
		return;
	}

	for(int i=0; i<5; i++) {
		if(codeStr[i]!='1' && codeStr[i]!='0') {
			printf("The code must be in this format: 10101A\n");
			return;
		}
	}

	if(!(codeStr[5]>=(int)'A' && codeStr[5]<=(int)'E')) {
		printf("The code must be in this format: 10101B\n");
    return;
	}

	int code[16] = {142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 0, 0, 0};
	//int mycode[16] = {142, 142, 136, 136, 142, 136, 136, 136, 142, 136, 136, 142, 128, 0, 0, 0};

	// Parse device-code
	for(int i=0; i<5; i++) {
		if(codeStr[i]=='1') code[i] = WTRUE;
		else code[i] = WFALSE;
	}

	// Parse device-id (A - E)
	int id = pow(2, (int)codeStr[5] - 65);

	// Set device-id
	int x = 1;
	for(int i=1; i<6; i++) {
		if((id & x)>0) code[4+i] = WTRUE;
		else code[4+i] = WFALSE;
		x = x<<1;
	}

	// Set Status
	if(active==1) {
		code[10] = WTRUE;
		code[11] = WFALSE;
	}else{
		code[10] = WFALSE;
		code[11] = WTRUE;
	}

	sendEther(gpio, code);
	printCode(code);
}

void PiControl::writeGpio(int gpio, int status) {
    if (wiringPiSetupGpio() == -1)
        return;

    pinMode(gpio, OUTPUT);

    if(status == 1)
        digitalWrite(gpio, HIGH);
    else
        digitalWrite(gpio, LOW);
}
