#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libnetwork.h>

int main()
{
    //Msg à envoyer à tout le monde en UDP
    char *hello = "Hello from Bot";
    while (1)
    {
        sendUDPBroadcast((unsigned char*) hello, strlen(hello), 5000);   
    }
    return 0;
}