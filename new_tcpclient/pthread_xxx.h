#ifndef PTHREAD_XXX_H
#define PTHREAD_XXX_H
#include <arpa/inet.h>
#include <string>
using namespace std;
void *pthread_xxx(void *arg);
int network_init_xxx(char*, uint16_t);
bool arduinotorpi(int&, char*);
void rpitodeepin(int, const char*);

char deepintorpi(int);
void rpitoarduino();

#endif