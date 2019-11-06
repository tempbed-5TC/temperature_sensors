#include "send.h"

/*
 * main
 */

int main(void)
{
    watchdog_stop();

    TIMER_ID_INPUT = UINT_MAX;
    node_id = NODE_ID_UNDEFINED;

    /* protothreads init */
    int i;
    for(i = 0; i < NUM_PT; i++)
    {
        PT_INIT(&pt[i]);
    }

    /* clock init */
    set_mcu_speed_dco_mclk_16MHz_smclk_8MHz();

    /* timer init */
    timerA_init();
    timerA_register_cb(&timer_tick_cb);
    timerA_start_milliseconds(TIMER_PERIOD_MS);

    /* UART init (serial link) */
    uart_init(UART_9600_SMCLK_8MHZ);
    uart_register_cb(uart_cb);
    uart_flag = 0;
    uart_data = 0;

    /* ADC10 init (temperature) */
    adc10_start();

    /* radio init */
    spi_init();
    cc2500_init();
    cc2500_rx_register_buffer(radio_tx_buffer, PKTLEN);
    cc2500_rx_register_cb(radio_cb);
    cc2500_rx_enter();
    radio_rx_flag = 0;

    /* retrieve node id from flash */
    node_id = *((char *) NODE_ID_LOCATION);
    printf("TAG RUNNING: \r\n");
    printf("node id retrieved from flash: %d\r\n", node_id);
    button_enable_interrupt();
    __enable_interrupt();

    /* simple cycle scheduling */
    while(1) {
        thread_uart(&pt[2]);
        thread_antibouncing(&pt[3]);
        //thread_process_msg(&pt[4]);
        thread_periodic_send(&pt[5]);
    }
}
