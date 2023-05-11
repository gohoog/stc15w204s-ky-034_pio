#include <STC15Fxx.h>

#include <pinout.h>
#include <delay.h>
#include <uart.h>

#define CMD_LED 0x31

unsigned int state;

BYTE get_cmd()
{
    while (!REND && SWITCH_GND != SWITCH_PRESSED)
        ;

    REND = 0;

    if (SWITCH_GND == SWITCH_PRESSED)
    {
        while (SWITCH_GND == SWITCH_PRESSED)
            ;
        return CMD_LED;
    }

    return RBUF;
}
void main()
{
    P5M0 = 0b10000;
    P5M1 = 0; // P54 push-pull
    
    uart_init(BAUD_9600_110592);
    uart_send_string("VK STC15W204S KY-034_PIO P54 - KY-034 VCC V1.0\n");
    uart_send_string("Send 1 to switch KY-34 or press button\n");

    state = P54 = LED_BUILDIN_ON;
    while (1)
    {
        if (get_cmd() == CMD_LED)
        {
            state ^= LED_BUILDIN_ON;
            P54 = state;
            uart_send_string("KY-034 ");
            uart_send_string(state ? "on" : "off");
            uart_send_string("\n");
            delay_ms(500);
        }
    }
}

INTERRUPT(tm0, 1)
{
    uart_handle();
}