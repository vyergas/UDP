/* udpszerver.c
 * 
 * Egyszerű UDP példa.
 * A program fogadja az UDP csomagokat és kiírja a forrásukat és a tartalmukat.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT "1122"

int main() 
{
  struct addrinfo hints;
  struct addrinfo* res;
  int err;
  struct sockaddr_in addr;
  socklen_t addrlen;
  char ips[NI_MAXHOST];
  char servs[NI_MAXSERV];
  int sock;
  char buf[256];
  int len;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  err = getaddrinfo(NULL, PORT, &hints, &res);
  if(err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return -1;
  }
  if(res == NULL)
  {
    return -1;
  }
  
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sock < 0)
  {
    perror("socket"); 
    return 1;
  }
  
  if(bind(sock, res->ai_addr, res->ai_addrlen) < 0)
  {
    perror("bind");    
    return 1;
  }

  freeaddrinfo(res);

  addrlen = sizeof(addr);
	
  while(1) {
    if(len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen) > 0)
    {
      switch(buf[0]){

        case 0x00:
          printf("received command ID: 0\n");
          buf[0] = 0x01;
          buf[1] = 0x02;
          if (sendto(sock, buf, 2, 0, (struct sockaddr *)&addr, addrlen) < 0)
  	    perror("sendto");
          break;

        case 0x01:
          printf("received command ID: 1\n");
          buf[0] = 0x03;
          buf[1] = 0x04;
          if (sendto(sock, buf, 2, 0, (struct sockaddr *)&addr, addrlen) < 0)
      	    perror("sendto");
          break;

        case 0x02:
          printf("received command ID: 2\n");
          buf[0] = 0x05;
          buf[1] = 0x06;
          if (sendto(sock, buf, 2, 0, (struct sockaddr *)&addr, addrlen) < 0)
       	    perror("sendto");
          break;

        default:
          printf("Unknown command\n");
          break;
      }//switch
    }//if
    else
      printf("uh oh - something went wrong!\n");
  }//while
  
  close(sock);
  
  return 0;
}
