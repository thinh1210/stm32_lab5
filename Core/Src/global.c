/*
 * global.c
 *
 *  Created on: Nov 16, 2024
 *      Author: Asus
 */#include<global.h>

uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
int have_cmd = 0;
char data_out[100];
char data_error[] = "Your cmd is wrong";
int block=0;
State_command st_cd = INIT;
State_communication st_cnc = RECEIVE;

void start_UART_Receive_IT(void) {
    HAL_UART_Receive_IT(&huart2, &temp, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {  // Kiểm tra đúng UART đang nhận
        buffer_flag = 1;
        buffer[index_buffer++] = temp;
        if (index_buffer >= MAX_BUFFER_SIZE) index_buffer = 0;
        HAL_UART_Receive_IT(&huart2, &temp, 1); // Tiếp tục nhận byte tiếp theo
    }
}

void init_PROGRAM() {
    HAL_ADC_Start(&hadc1);
    start_UART_Receive_IT();
    st_cd = INIT;
    st_cnc = RECEIVE;
    have_cmd = 0;
}

void command_parser_fsm() {
    switch (st_cd) {
    case INIT:
        if (temp == '!') {
            st_cd = WAIT_COMMAND;
            index_buffer = 0;
            block=1;
        }
        break;
    case WAIT_COMMAND:
        if (temp == '#') {
            //st_cd = RECEIVE;
            have_cmd = 1;
            st_cd= INIT;
        }
        break;
    default:
        break;
    }
}

void uart_communication_fsm() {
    switch (st_cnc) {
    case RECEIVE:
        if (have_cmd) {
            if (strncmp("RST", (char*)buffer, 3) == 0) {
                int adc = HAL_ADC_GetValue(&hadc1);
                sprintf(data_out, "!ADC=%d#\r\n", adc);
                HAL_UART_Transmit(&huart2, (uint8_t*)data_out, strlen(data_out), 50);
                st_cnc = SPAM;
                Setup_time(SPAM_TASK, TIME_TO_NEXT_SPAM);
                block=0;
            } else {
                HAL_UART_Transmit(&huart2, (uint8_t*)data_error, strlen(data_error), 50);
            }
            have_cmd = 0;
        }
        break;

    case SPAM:
        if (have_cmd) {
            if (strncmp("OK", (char*)buffer, 2) == 0) {
                st_cnc = RECEIVE;
            } else {
                HAL_UART_Transmit(&huart2, (uint8_t*) data_error, strlen(data_error), 50);
                block=0;
            }
            have_cmd = 0;
        }
        if (actions[SPAM_TASK].timer_flag == 1 && !block) {
            HAL_UART_Transmit(&huart2, (uint8_t*)data_out, strlen(data_out), 50);
            reset(SPAM_TASK);
        }
        break;
    }
}
