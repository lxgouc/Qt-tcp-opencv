######################################################################
# Automatically generated by qmake (3.0) ?? 9? 6 13:33:03 2017
######################################################################

TEMPLATE = app
TARGET = new_tcpclient
INCLUDEPATH += .

# Input
HEADERS += pthread_frame.h pthread_arduino.h network_init.h
SOURCES += pthread_frame.cpp pthread_arduino.cc network_init.cc tcpclient.cpp

INCLUDEPATH +=  /usr/local/include/opencv \
				/usr/local/include \
				/usr/local/include/opencv2

LIBS +=  `pkg-config opencv --cflags --libs` \
         -lwiringPi \
         -lrt \
         -lcrypt 