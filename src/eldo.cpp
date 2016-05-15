/*
Copyright 2016 by Marvin Winkler
Based on PowerPi by Anton Hammerschmidt

This file is part of eldo.

eldo is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

eldo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with eldo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include <stdio.h>

#include "tools.h"
#include "picontrol.h"

using namespace std;

string current_exec_name;

void printInfo() {
  std::cout << "try:" << std::endl;
  std::cout << current_exec_name << " (gpio):(elro_code):(status [1|0])" << std::endl;
  std::cout << current_exec_name << " 17:10101A:1" << std::endl;
}

int executeCmd(string cmd) {
    if(cmd.length() < 1) {
        std::cout << "statement too short" << std::endl;
        printInfo();
        return 1;
    }

    vector<string> words = Tools::explode(":", cmd);
    if(words.size() == 2) {
        std::cout << "invalid statement" << std::endl;
        printInfo();
        return 1;
    }

    int datagpio = atoi(words[0].c_str());
    string socketcode = words[1];
    int status = atoi(words[2].c_str());

    PiControl::send433Mhz(datagpio, socketcode, status);
    std::cout << "set socket " << socketcode << " to " << status << std::endl;
    return 0;
}

int main( int argc, const char* argv[] )
{
  current_exec_name = argv[0]; // Name of the current exec program

  if(argc == 2) {
    string cmd = argv[1];
    return executeCmd(cmd);
  } else {
    std::cout << "invalid cmd: ";
    for(int i = 0; i < argc; i++) {
      std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
    printInfo();
    return 1;
  }
}
