#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char const *ip = "224.0.0.1";
static int port = 2112;

int main (int argc, char *argv[]) {
  int socket_descriptor;
  struct sockaddr_in address;
  socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
  
  char *echo_string;
  int echo_len;
  
  /* check arguments */
  if (argc < 2) {
     perror("usage: server broadcast_message");
     exit(EXIT_FAILURE);
  }
  
  if (socket_descriptor == -1) {
     perror("socket()");
     exit(EXIT_FAILURE);
  }
  
  memset(&address, 0, sizeof(address));
  
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip);
  address.sin_port = htons(port);

  printf ("server is ready...\n");
  
  echo_string = argv[1];
  echo_len = strlen(echo_string);
  
  /* beginn broadcast */
  int reps = 1;
  for (int i=0; i<reps; i++) {
    if (sendto(socket_descriptor, echo_string, echo_len, 0, (struct sockaddr *) &address, sizeof(address)) < 0) {
       perror("sendto()");
       exit(EXIT_FAILURE);
    }
    printf("send command #%-2d to clients: %s\n", i, echo_string);
    sleep(1);
  }

  return EXIT_SUCCESS;
}
