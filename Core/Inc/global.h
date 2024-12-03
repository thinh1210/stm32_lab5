/*
 * global.h
 *
 *  Created on: Nov 16, 2024
 *      Author: Asus
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include<stdint.h>
#include<timer.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<main.h>

#define MAX_BUFFER_SIZE         30
#define SPAM_TASK               0
#define TIME_TO_NEXT_SPAM       3000 // 3000/10
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

extern uint8_t temp ;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;



typedef enum{
    INIT,
    WAIT_COMMAND,
} State_command;
typedef enum {
    RECEIVE,
    SPAM,
} State_communication;


void init_PROGRAM();
void command_parser_fsm();
void uart_communication_fsm();
#endif /* INC_GLOBAL_H_ */
