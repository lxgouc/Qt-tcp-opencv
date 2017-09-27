#ifndef PTHREAD_FRAME_H
#define PTHREAD_FRAME_H
#include <arpa/inet.h>
#include <vector>
using namespace std;

class QByteArray;
void *pthread_frame(void *arg);
int network_init_frame(char*, uint16_t);
void getframe(int);
void sendframe(vector<unsigned char>&, const vector<unsigned char>::size_type&, const int&);

#endif