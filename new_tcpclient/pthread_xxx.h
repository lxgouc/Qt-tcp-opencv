#ifndef PTHREAD_XXX_H
#define PTHREAD_XXX_H
#include <arpa/inet.h>
#include <string>
using namespace std;
void *pthread_xxx(void *arg);
int network_init_xxx(char*, uint16_t);
string getarduinodata(int&, string*);
void sendarduinodata(int, string);

char getraspberrydata(int);
void sendraspberrydata();

#endif