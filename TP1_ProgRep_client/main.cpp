#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_PORT 50013

using namespace std;

void exitErreur(const char * msg) {
    perror(msg);
    exit(EXIT_FAILURE);
} // exit ()

int main() {
    int sock_serveur = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockaddr_serveur;

    sockaddr_serveur.sin_family = AF_INET;
    sockaddr_serveur.sin_port = htons(NUM_PORT);
    //sockaddr_serveur.sin_addr.s_addr = htonl(INADDR_ANY);

    inet_aton("10.203.9.229", &sockaddr_serveur.sin_addr);

    if(connect(sock_serveur, (struct sockaddr *) &sockaddr_serveur, sizeof (sockaddr_in)) == -1)
        exitErreur("connect");

    char buff [1024];
    buff [read (sock_serveur, buff, sizeof(buff))] = '\0';
    cout << buff << endl;

    close(sock_serveur);

    return 0;
} // main ()
