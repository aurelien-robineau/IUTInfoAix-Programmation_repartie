#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define BACKLOG 50

[[noreturn]] void exitError(const string & msg) {
    perror(msg.c_str());
    exit( EXIT_FAILURE);
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

void proto (const int & sockClient) {
    while(true) {
        string msgClt;
        readLine(sockClient, msgClt);
        cout << "Client: " << msgClt;

        if(msgClt == "bye")
            break;

        string msgSrv;
        cout << "Serveur: ";
        getline(cin, msgSrv);

        if(msgSrv == "bye")
            break;

        msgSrv.push_back('\n');
        Write(sockClient, msgSrv.c_str(), msgSrv.size());
    }

    if (-1 == close (sockClient))
        exitError("close sock client");

    //cout << "Attente d'un client..." << endl;
}

int main(int argc, char * argv[]) {

    if(argc<2) exitError("Usage invalide");

    // Le serveur écoute sur toutes les interfaces réseaux de sa machine et sur le port donné comme argument de la ligne de commande.

    int port = atoi(argv[1]) ;

    int sockServeur = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockaddrServeur;

    sockaddrServeur.sin_family = AF_INET;
    sockaddrServeur.sin_port = htons(port);
    sockaddrServeur.sin_addr.s_addr = INADDR_ANY;


	if (bind(sockServeur, (struct sockaddr *) &sockaddrServeur,
			sizeof(sockaddr_in)) == -1)
		exitError("bind");


    if (listen(sockServeur, BACKLOG) == -1)
        exitError("listen");

    cout << "Serveur chat lancé  sur le port " << port << endl;
    cout << "Attente d'un client..." << endl;
    while (true) {
        // Le serveur attend un client
        int sockClient = accept(sockServeur, nullptr, nullptr);
        if (sockClient == -1)
            exitError("accept");

        //protocle de dialogue seveur => client
        //cout << "Client trouvé !" << endl;
        proto(sockClient);
    }
    if (-1 == close(sockServeur))
        exitError("close sock server");
    return EXIT_SUCCESS;
}
