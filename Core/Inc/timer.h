/*
 * timer.h
 *
 *  Created on: Nov 16, 2024
 *      Author: Asus
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_
#define NUMBER_OF_ACTIONS		10
#define TIME_COUNT_PROGRAM		0
#define ONE_SECOND				1
#define TIME_SEGMENT			3
#define TIME_LED				4
#define TIMER_CYCLE 			10
typedef struct{
	int time;
	int timer_counter;
	int timer_flag;
}detail_time;

extern detail_time actions[NUMBER_OF_ACTIONS];
void Setup_time(int index, int duration);
void timer_run();
void reset(int index);

#endif /* INC_TIMER_H_ */
