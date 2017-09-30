#ifndef PTHREAD_XXX_H
#define PTHREAD_XXX_H
#include <arpa/inet.h>
#include <string>
using namespace std;

void *pthread_xxx(void *arg);
bool arduinotorpi(int&, char*);
void rpitodeepin(int, const char*);

void *deepintorpi(void *arg);
void rpitoarduino();

#endif