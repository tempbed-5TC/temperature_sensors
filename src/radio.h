#ifndef RADIO_H_INCLUDED
#define RADIO_H_INCLUDED

#include <msp430f2274.h>

#if defined(__GNUC__) && defined(__MSP430__)
/* This is the MSPGCC compiler */
#include <msp430.h>
#include <iomacros.h>
#elif defined(__IAR_SYSTEMS_ICC__)
/* This is the IAR compiler */
//#include <io430.h>
#endif

#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "isr_compat.h"
#include "leds.h"
#include "clock.h"
#include "timer.h"
#include "button.h"
#include "uart.h"
#include "adc10.h"
#include "spi.h"
#include "cc2500.h"
#include "flash.h"
#include "watchdog.h"

#include "pt.h"

#define DBG_PRINTF printf


/* 100 Hz timer A */
#define TIMER_PERIOD_MS 10

#define PKTLEN 7
#define MSG_BYTE_TYPE 0U
#define MSG_BYTE_NODE_ID 1U
#define MSG_BYTE_CONTENT 2U
#define MSG_TYPE_ID_REPLY 0x01
#define MSG_TYPE_TEMPERATURE 0x02

#define NODE_ID_LOCATION INFOD_START

#define NODE_ID_UNDEFINED 0x00
/* 10 seconds to reply to an id request */
#define ID_INPUT_TIMEOUT_SECONDS 10
/* the same in timer ticks */
#define ID_INPUT_TIMEOUT_TICKS (ID_INPUT_TIMEOUT_SECONDS*1000/TIMER_PERIOD_MS)
unsigned char node_id;

#define NUM_TIMERS 6
uint16_t timer[NUM_TIMERS];
#define TIMER_LED_RED_ON timer[0]
#define TIMER_LED_GREEN_ON timer[1]
#define TIMER_ANTIBOUNCING timer[2]
#define TIMER_RADIO_SEND timer[3]
#define TIMER_ID_INPUT timer[4]
#define TIMER_RADIO_FORWARD timer[5]


void printhex(char *buffer, unsigned int len);
void dump_message(char *buffer);
void prompt_node_id();
void set_node_id(unsigned char id);


/* Protothread contexts */

#define NUM_PT 7
struct pt pt[NUM_PT];

/*
 * Timer
 */

void timer_tick_cb();
int timer_reached(uint16_t timer, uint16_t count);

/*
 * leds
 */

int led_green_duration;
int led_green_flag;
void led_green_blink(int duration);
PT_THREAD(thread_led_green(struct pt *pt));
PT_THREAD(thread_led_red(struct pt *pt));


/*
 * Radio
 */

char radio_tx_buffer[PKTLEN];
char radio_rx_buffer[PKTLEN];
int radio_rx_flag;

void radio_cb(uint8_t *buffer, int size, int8_t rssi);
void radio_send_message();
PT_THREAD(thread_process_msg(struct pt *pt));

/*
 * UART
 */

int uart_flag;
uint8_t uart_data;

int uart_cb(uint8_t data);
void init_message();
void send_temperature();
void send_id_reply(unsigned char id);
PT_THREAD(thread_uart(struct pt *pt));


/*
 * Button
 */

#define ANTIBOUNCING_DURATION 10 /* 10 timer counts = 100 ms */
int antibouncing_flag;
int button_pressed_flag;

void button_pressed_cb();
PT_THREAD(thread_button(struct pt *pt));
PT_THREAD(thread_antibouncing(struct pt *pt));
PT_THREAD(thread_periodic_send(struct pt *pt));


#endif //RADIO_H_INCLUDED
