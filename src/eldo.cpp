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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "tools.h"
#include "picontrol.h"

#define BUF 1024

using namespace std;

static int datagpio;
static int port = 2112;
static char const *ip = "224.0.0.1";
static struct ip_mreq command;

string current_exec_name;

static int setup_multicast_socket(void) {
  int loop = 1;
  int socket_descriptor;
  struct sockaddr_in sin;

  memset(&sin, 0, sizeof(sin));

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(port);

  if ((socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
     perror("socket()");
     exit(EXIT_FAILURE);
  }

  // allow multiple processes to use the same port
  loop = 1;
  if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)) < 0) {
     perror("setsockopt:SO_REUSEADDR");
     exit(EXIT_FAILURE);
  }

  if(bind(socket_descriptor, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
     perror("bind");
     exit(EXIT_FAILURE);
  }

  // allow multicast on this machine
  loop = 1;
  if (setsockopt(socket_descriptor, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0) {
     perror("setsockopt:IP_MULTICAST_LOOP");
     exit(EXIT_FAILURE);
  }

  // join the broadcast group:
  command.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
  command.imr_interface.s_addr = htonl(INADDR_ANY);
  
  if (setsockopt(socket_descriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, &command, sizeof(command)) < 0) {
     perror("setsockopt:IP_ADD_MEMBERSHIP");
  }
  
  return socket_descriptor;
}

void printInfo() {
  std::cout << "try:" << std::endl;
  std::cout << "single command:" << std::endl;
  std::cout << current_exec_name << " (gpio) (elro_code):(status [1|0])" << std::endl;
  std::cout << current_exec_name << " 17 10101A:1" << std::endl;
  std::cout << std::endl;
  std::cout << "multicast client (listening on udp port " << port << "):" << std::endl;
  std::cout << current_exec_name << " (gpio)" << std::endl;
  std::cout << current_exec_name << " 17" << std::endl;
}

int send_multicast(int reps, char* cmd_cstring[]) {
  int socket_descriptor;
  struct sockaddr_in address;
  socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
  
  char *echo_string;
  int echo_len;
  
  if (socket_descriptor == -1) {
     perror("socket()");
     exit(EXIT_FAILURE);
  }
  
  memset(&address, 0, sizeof(address));
  
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip);
  address.sin_port = htons(port);

  echo_string = *cmd_cstring;
  echo_len = strlen(echo_string);
  
  // beginn broadcast
  for (int i=0; i<reps; i++) {
    if (sendto(socket_descriptor, echo_string, echo_len, 0, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cout << "sendto failure" << std::endl;
        return 1;
    }
    std::cout << "send command #" << (i+1) << " " << echo_string << " to clients" << std::endl;
    sleep(1);
  }

  return EXIT_SUCCESS;
}

int executeCmd(char* cmd_cstring[]) {
    std::cout << "exec: " << *cmd_cstring << std::endl;

    string cmd = *cmd_cstring;
        
    if(cmd.find(':', 0) == string::npos || cmd.length() < 1){
        std::cout << "invalid statement" << std::endl;
        return 1;
    }
    
    vector<string> words = Tools::explode(":", cmd);
    if(words.size() != 2) {
        std::cout << "invalid statement" << std::endl;
        return 1;
    }

    string socketcode = words[0];
    int status = atoi(words[1].c_str());

    PiControl::send433Mhz(datagpio, socketcode, status);
    std::cout << "set socket (datagpio: " << datagpio << ") " << socketcode << " to " << status << std::endl;
    return 0;
}

int main( int argc, char* argv[] )
{
  current_exec_name = argv[0]; // Name of the current exec program

  /* INFO */
  if (argc == 1) {
    printInfo();
    return 1;
  }

  /* COMMANDER */
  char send_arg[] = { "send" };
  if(strcmp(argv[1], send_arg) == 0){
    std::cout << "send multicast cmd: " << argv[2] << std::endl;
    int reps = 1;
    if(argc == 4){
        reps = atoi(argv[3]);
    }
    return send_multicast(reps, &argv[2]);
  }

  /* SINGLE CMD */
  datagpio = atoi(argv[1]);
  std::cout << "init datagpio: " << datagpio << std::endl;
  if(argc == 3) {
    std::cout << "exec single cmd: " << *argv[2] << std::endl;
    return executeCmd(&argv[2]);
  }  
  
  /* MULTICAST LISTENER */
  std::cout << "starting in multicast listening mode" << std::endl;
  
  int iter = 0;
  char *buffer = (char*) malloc(BUF);
  int sin_len;
  int socket;
  struct sockaddr_in sin;
  
  socket = setup_multicast_socket();
  
  // recive multicast messages
  while (1) {
     iter++;
     sin_len = sizeof(sin);
     
     if (recvfrom(socket, buffer, BUF-1, 0, (struct sockaddr *)&sin, (socklen_t*)&sin_len) == -1) {
        std::cout << "error recvieving multicast data" << std::endl;
        std::cout << std::endl;
        // return 1;
     }else{
        std::cout << "recvied cmd (#" << iter << "): " << buffer << std::endl;
        int ret = executeCmd(&buffer);
        std::cout << "command #"<< iter << " executed - return value: " << ret << std::endl;
        std::cout << std::endl;
     }
     sleep(2);
  }

  std::cout << "stopping server mode... (cleaning up)" << std::endl;
  
  // remove multicast-socket from group
  if (setsockopt(socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &command, sizeof(command)) < 0 ) {
      perror("setsockopt:IP_DROP_MEMBERSHIP");
  }
  
  close(socket);

  std::cout << "bye bye :)" << std::endl;
  return EXIT_SUCCESS;
}
