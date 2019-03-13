#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>



#define BUFFER_SIZE 50
#define CASTADDR reinterpret_cast <in_addr*>

using namespace std;

[[ noreturn ]] void exitError(const string & msg) {
    perror(msg.c_str());
    exit( EXIT_FAILURE);

}

void testGetHostByName (const string & name){

    struct  hostent* h = gethostbyname (name.c_str());

    if(h == nullptr) exitError("gethostbyname");

    cout << "Première adresse IP de ce nom de machine : " << endl;

    cout <<  inet_ntoa (*CASTADDR (h->h_addr)) <<endl ;


    cout << "Liste de toutes les adresses IP de ce nom de machine : " << endl;

    for(char** p (h->h_addr_list); *p != nullptr ; ++p) {
        cout <<  inet_ntoa (*CASTADDR (*p)) << endl;
    }

    cout << "Liste de tous les alias de ce nom de machine : " << endl;

    for(char** p (h->h_aliases); *p != nullptr ; ++p) {
        cout << *p << endl;
    }
}

int getIPAdress (const string & name, struct in_addr * adr){

    struct  hostent* h = gethostbyname (name.c_str());

    if (h == nullptr) return 0;

    *adr = *CASTADDR (h->h_addr);
    return 1;

}

struct in_addr getIPAdress (const string & name){

    struct  hostent* h = gethostbyname (name.c_str());
    if (h == nullptr) exitError("getIPAdress");
    return *CASTADDR (h->h_addr);
}


int main(){
    testGetHostByName("www.yahoo.com");

    /*
    struct in_addr  adrip;
    getadresseIP("www.yahoo.com", &adrip);
    cout << inet_ntoa(adrip) <<endl ;
    */
    struct in_addr  adrip = getIPAdress("www.yahoo.com");
    cout << inet_ntoa(adrip);

    return EXIT_SUCCESS;
}
