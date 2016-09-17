#ifndef GAUGE
#define GAUGE

#include <sys/types.h>
#include "raspi_can_queue.h"

typedef struct
{
	int displayValue;
	int timeStamp;
	int targetValue;
	char errFlag;
	char timeOutFlag;
	char bFLag;
	char bDisp;
	char fastSpeed;
	short msgId;

}Gauge;

Gauge tacho;

char IGNF;
/*
 * sendData:  animateSeq
 * data sampling: getData:   dataProcessing:    dataFilter, saveData
 * */
void sendData(int dV, int tV);
void getData(can_msg* msg);
void dataProcessing(can_msg* msg);
void tachoProcess(int data, Gauge *tacho);
//int tachoTimeOut(int id);
void gaugeInit(Gauge *g);
void ignProcess(int data);

#endif
