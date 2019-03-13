#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAILLE_BUFFER 10

using namespace std;

[[noreturn]] void exitError (const string & msg) {
    perror (msg.c_str());
    exit (EXIT_FAILURE);
}

ssize_t Read(int fildes, void *buf, size_t nbyte) {
    ssize_t n;
    if (-1 == (n= read (fildes, buf, nbyte)))
        exitError ("Read");
    return n;
}

ssize_t Recv(int fildes, void *buf, size_t nbyte, int flags) {
    ssize_t n;
    if (-1 == (n= recv (fildes, buf, nbyte, flags)))
        exitError ("Read");
    return n;
}

ssize_t Write(int fildes, const void *buf, size_t nbyte){
    ssize_t n;
    if (-1 == (n= write (fildes, buf, nbyte)))
        exitError ("Write");
    return n;
}

string & readLine (const int & sock, string & msg) {
    msg.resize(0);
    const ssize_t buffSize (5);
    char buff [buffSize];

    while (true)
    {
        // a chaque tour de boucle on fait une lecture sans conommation de l'information
        ssize_t n = Recv(sock, buff, buffSize, MSG_PEEK);
        // question : est-ce qu'on a '\n' dans le buffer? si oui a quelle position du buffer
        string::size_type pos  = string (buff, n).find('\n');
        // on construit le message avec uniquement les informations utiles
        msg += string (buff, pos == string::npos ? n : pos +1);
        // on ne consomme que l'information utile
        Read (sock, buff, pos == string::npos ? n : pos +1);
        // cout << msg << endl;
        if (pos != string::npos) break;
    }
    return msg;
}

void proto (const int & sockServer) {
    string msg;
    cin >> msg;
    msg.push_back('\n');
    Write(sockServer, msg.c_str(), msg.size());
    cout << readLine(sockServer, msg) << endl;
}

int main (int argc, char * argv[]) {

    // argv[1] : l'adresse IP du serveur auquel on veut se connecter
    // argv[2] : le port du serveur auquel on veut se connecter

    if (argc <3){
        cout << "Usage invalide"<<endl;
        exit (EXIT_FAILURE);
    }


    int sockServer = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in sockaddrServeur;

    sockaddrServeur.sin_family = AF_INET;
    inet_aton (argv[1], &sockaddrServeur.sin_addr);
    sockaddrServeur.sin_port = htons (atoi(argv[2])) ;



    if (connect(sockServer, (struct sockaddr *) &sockaddrServeur, sizeof (sockaddrServeur)) == -1)
        exitError("connect");


    proto (sockServer);

    close(sockServer);

}
