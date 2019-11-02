/*
 * motor.h
 *
 * Created: 10/2/2019 10:21:19 AM
 *  Author: omara
 */ 

#ifndef MOTOR_H_
#define MOTOR_H_

#define TURN_TIME 10000

#include <util/delay.h>
#include "DIO.h"

typedef enum{
	PAIR1 = 0,
	PAIR2 = 1
	}motor_pairs;


extern void MOTOR_CarForward();
extern void MOTOR_CarBackward();
void MOTOR_CarRight(uint16);
extern void MOTOR_CarStop();
extern void MOTOR_CarLeft(uint16);

#endif /* MOTOR_H_ */
