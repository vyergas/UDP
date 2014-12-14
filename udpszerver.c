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
      switch(buf[1]){

        case 0x00:
          printf("received command ID: 00 (Connection check)\n");
          buf[0] = 0x01; //payload length
          buf[1] = 0x40; //MSG ID
          if (sendto(sock, buf, 2, 0, (struct sockaddr *)&addr, addrlen) < 0)
       	    perror("sendto");
          break;

        case 0x01:
          printf("received command ID: 01 (Number of nodes)\n");
          buf[0] = 0x05; //payload length
          buf[1] = 0x41; //MSG ID
       	  buf[2] = 0x03; //Number of nodes
 	  buf[3] = 0x01; //ID1
	  buf[4] = 0x02; //ID2
	  buf[5] = 0x12; //ID3
          if (sendto(sock, buf, 6, 0, (struct sockaddr *)&addr, addrlen) < 0)
       	    perror("sendto");
          break;

        case 0x02:
          printf("received command ID: 02 (Node datas)\n");
          buf[0] = 0x14; //payload length
          buf[1] = 0x42; //MSG ID
	  buf[2] = 0x03; //Number of nodes
	  buf[3] = 0x01; //Node iD1
	  buf[4] = 0x01; //Type 1 Temp
	  buf[5] = 0x00; //Val1		
	  buf[6] = 0x18; //Val2
 	  buf[7] = 0x00; //Thres1
	  buf[8] = 0x20; //Thres2
	  buf[9] = 0x02; //Node iD2 Door
	  buf[10] = 0x02; //Type
	  buf[11] = 0x00; //Val1		
	  buf[12] = 0x00; //Val2
 	  buf[13] = 0x00; //Thres1
	  buf[14] = 0x01; //Thres2
	  buf[15] = 0x12; //Node iD3 Distance
	  buf[16] = 0x03; //Type
	  buf[17] = 0x00; //Val1		
	  buf[18] = 0x80; //Val2
 	  buf[19] = 0x00; //Thres1
	  buf[20] = 0x70; //Thres2
          if (sendto(sock, buf, 21, 0, (struct sockaddr *)&addr, addrlen) < 0)
      	    perror("sendto");
          break;

        case 0x03:
          printf("received command ID: 03 (Setting acknowledged)\n");
          buf[0] = 0x01;
          buf[1] = 0x43;
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
