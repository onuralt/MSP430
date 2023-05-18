/***************************************************************************//**
 * @file    main.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   Exercise 5 - Final Project
 *
 * LCD PINS
 * ----------------------------------------------------------------------------
 * CON5:D4          -> CON3:P2.0 (shared)
 * CON5:D5          -> CON3:P2.1 (shared)
 * CON5:D6          -> CON3:P2.2 (shared)
 * CON5:D7          -> CON3:P2.3 (shared)
 * CON5:RS          -> CON4:P3.0
 * CON5:R/W         -> CON4:P3.1
 * CON5:E           -> CON4:P3.2
 *
 * I2C/SPI PINS
 * ----------------------------------------------------------------------------
 * CON6:I2C_SPI     -> CON2:P3.3 (shared)
 * CON6:XSCL        -> CON2:P1.6
 * CON6:XSDA        -> CON2:P1.7
 * CON9:F_/CS       -> CON4:P3.7
 * CON9:F_CLK       -> CON2:P1.5
 * CON9:F_SO        -> CON6:CC_SO
 * CON9:F_SI        -> CON6:CC_SI
 *
 * ULTRASONIC SENSOR PINS
 * ----------------------------------------------------------------------------
 * CON7:US-CLK      -> CON4:P3.3 (shared)
 * CON7:Rx-COMP     -> CON2:P1.0 (shared)
 *
 * ADC PINS
 * ----------------------------------------------------------------------------
 * X9 (U_NTC)       -> CON3:P1.0 (shared)
 * K2 (LDR)         -> CON3:P1.3
 * X6 (U_POT)       -> CON3:P1.4
 *
 * GPIO PINS
 * ----------------------------------------------------------------------------
 * K4 (LED gn)      -> CON3:P3.0 (shared)
 * K3 (LED rt)      -> CON3:P3.1 (shared)
 * X11 (PB5)        -> CON3:P3.5
 * X12 (PB6)        -> CON3:P3.6
 * X5 (REL_STAT)    -> CON3:P3.4
 *
 * MISC
 * ----------------------------------------------------------------------------
 * X10 (HEATER)     -> X3 (REL_ID)
 *
 *
 * @note    The project was exported using CCS 8.0.0.
 ******************************************************************************/
#include "libs/UART.h"
#include "libs/USCIAB0.h"
#include "libs/joy.h"
#include "libs/usonic.h"
#include "libs/mma.h"
#include "string.h"
#include "stdio.h"
#include "libs/lcd.h"
#include "libs/sensorMain.h"
#include "libs/actuatorMain.h"

/* Start timer in up mode*/
#define console_StartTA1()          TA1CTL |= MC_1;

/* Stop timer*/
#define console_StopTA1()           TA1CTL |= MC_0;

/* Set time for 2 seconds*/
#define TIME                35

/* Declare functions*/
void initMSP(void);
void console_TA1_init(void);
void read_str(void);
void sensorRead(void);
unsigned char command(unsigned char cmd_rd);

/* Declare variables*/
volatile unsigned int milisecond = 0;
volatile int pulse_count = 0;
unsigned char func_on = 0;
unsigned char cmd_on = 1;
unsigned int distance;
unsigned char joy_ax[3];
int x = 0;
double mma_ax[3];
char strmem_a[32];
char strmem_b[32];
int flag_2 = 1;
int flag_1 = 1;
char str[32];
int i;
int pot, ldr, ntc, pb;
char led_array[5] = "0000";
char sarray[5][30];

int main(void)
{
    initMSP();

    /* Set pin direction for leds and the relay*/
    P3DIR |= (BIT0 | BIT1 | BIT4);

    /* Set output low*/
    P3OUT &= ~(BIT0 | BIT1 | BIT4);

    /* Initialize TA1*/
    console_TA1_init();

    /* Start TA1*/
    console_StartTA1();

    /* Turn on echo*/
    serialEchoBack(1);

    while (1)
    {
        /* Initialize UART*/
        UART_init();

        /* Enter the initial command once*/
        cmd_on = command(cmd_on);

        /* Read entered command*/
        read_str();

        /* Check if the command is "sensorDashboard"*/
        if ((strcmp(strmem_a, "sensorDashboard\r") == 0) & (flag_1 == 1))
        {
            flag_1 = 0;
            serialEchoBack(0);
            func_on = 1;
            milisecond = TIME;
        }

        /* Dashboard update*/
        if ((func_on == 1) & (milisecond == TIME))
        {
            if (flag_2 == 1)
            {
                memset(str, NULL, 32);
                x = 0;
                flag_2 = 0;
            }

            /* Read all sensors*/
            sensorRead();

            /* Read entered command*/
            read_str();

            /* Initialize UART*/
            UART_init();

            /* Display all readings*/
            serialPrint("\e[H\e[15A\e[0E\e[2KJoystick X value: ");
            serialPrintInt(joy_ax[0]);

            serialPrint("\e[1B\e[0E\e[2KJoystick Y value: ");
            serialPrintInt(joy_ax[1]);

            serialPrint("\e[1B\e[0E\e[2KJoystick Button: ");
            serialPrintInt(joy_ax[2]);

            serialPrint("\e[1B\e[0E\e[2KUltrasonic Sensor: ");
            serialPrintInt((distance / 65));
            serialPrint(" cm");

            serialPrint("\e[1B\e[0E\e[2KAccelerometer X value: ");
            serialPrintFloat(mma_ax[0]);

            serialPrint("\e[1B\e[0E\e[2KAccelerometer Y value: ");
            serialPrintFloat(mma_ax[1]);

            serialPrint("\e[1B\e[0E\e[2KAccelerometer Z value: ");
            serialPrintFloat(mma_ax[2]);

            serialPrint("\e[1B\e[0E\e[2KPotentiometer value: ");
            serialPrintInt(pot);

            serialPrint("\e[1B\e[0E\e[2KLDR value: ");
            serialPrintInt(ldr);

            serialPrint("\e[1B\e[0E\e[2KNTC value: ");
            serialPrintInt(ntc);

            serialPrint("\e[1B\e[0E\e[2KPB1: ");
            serialPrintInt(pb & 0x01);
            serialPrint(", ");

            serialPrint("PB2: ");
            serialPrintInt((pb >> 1) & 0x01);
            serialPrint(", ");

            serialPrint("PB3: ");
            serialPrintInt((pb >> 2) & 0x01);
            serialPrint(", ");

            serialPrint("PB4: ");
            serialPrintInt((pb >> 3) & 0x01);
            serialPrint(", ");

            serialPrint("PB5: ");
            serialPrintInt((pb >> 4) & 0x01);
            serialPrint(", ");

            serialPrint("PB6: ");
            serialPrintInt((pb >> 5) & 0x01);

            serialPrintln("");
            serialFlush();

            /* Reset the timer*/
            milisecond = 0;
        }

        /* Read commands and turn on/off actutors accordingly*/
        if (func_on == 1)
        {
            serialPrint("\e[9D\e[1m\e[4mcommand:\e[m\e[1m ");
            read_str();

            serialPrint(str);

            for (i = 0; i < x; i++)
            {
                serialPrint("\e[1D");
            }
        }
        if (strcmp(strmem_a, "led_1_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[0] = 0x31;
        }
        if (strcmp(strmem_a, "led_2_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[1] = 0x31;
        }
        if (strcmp(strmem_a, "led_3_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[2] = 0x31;
        }
        if (strcmp(strmem_a, "led_4_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[3] = 0x31;
        }
        if (strcmp(strmem_a, "led_5_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT |= BIT1;
        }
        if (strcmp(strmem_a, "led_6_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT |= BIT0;
        }

        if (strcmp(strmem_a, "led_1_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[0] = 0x30;
        }
        if (strcmp(strmem_a, "led_2_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[1] = 0x30;
        }
        if (strcmp(strmem_a, "led_3_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[2] = 0x30;
        }
        if (strcmp(strmem_a, "led_4_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            led_array[3] = 0x30;
        }
        if (strcmp(strmem_a, "led_5_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT &= ~BIT1;
        }
        if (strcmp(strmem_a, "led_6_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT &= ~BIT0;
        }

        actuatorMain_init();
        led_control(led_array);

        if (strcmp(strmem_a, "heater_on\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT |= BIT4;
        }

        if (strcmp(strmem_a, "heater_off\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            P3OUT &= ~BIT4;
        }

        memcpy(strmem_b, strmem_a, 10);
        if ((strcmp(strmem_b, "lcd_print_") == 0)
                | (strcmp(strmem_b, "lcd_print_d") == 0))
        {
            memset(strmem_b, NULL, 32);
            memcpy(strmem_b, &strmem_a[10], 16);
            memset(strmem_a, NULL, 32);
            strmem_b[strlen(strmem_b) - 1] = '\0';
            lcd_init();
            lcd_putText(strmem_b);
        }

        if (strcmp(strmem_a, "lcd_clear\r") == 0)
        {
            memset(strmem_a, NULL, 32);
            lcd_init();
            lcd_clear();
        }
    }
}

/* Read all sensor values*/
void sensorRead(void)
{
    console_StopTA1();

    joy_init();
    joy_read_all(joy_ax);

    usonic_init();
    usonic_read(distance);

    __delay_cycles(10000);
    mma_init();
    mma_ax[0] = mma_getRealX();
    mma_ax[1] = mma_getRealY();
    mma_ax[2] = mma_getRealZ();

    if (mma_ax[0] == 255)
    {
        mma_ax[0] = 0;
    }
    __delay_cycles(10000);

    sensorMain_init();
    pb = pbRead();
    pot = potRead();
    ldr = ldrRead();
    ntc = ntcRead();

    console_StartTA1();
}

/* Enter a command once*/
unsigned char command(unsigned char cmd_rd)
{
    switch (cmd_rd)
    {
    case 0:
        break;
    case 1:
        serialPrint("\e[9D\e[1m\e[4mcommand:\e[m ");
        return 0;
    default:
        /*Programmers trap*/
        while (1)
        {

        }
    }
}

/* Read a character from the serial terminal*/
void read_str(void)
{
    while (serialAvailable())
    {
        str[x] = serialRead();
        if (str[x] == 0x7F)
        {
            str[x - 1] = NULL;
            str[x] = NULL;
            x -= 2;
            if (x < 0)
            {
                x = -1;
            }
        }
        if (str[x] == 0x0D)
        {
            memset(strmem_a, NULL, 32);
            strcpy(strmem_a, str);
            memset(str, NULL, 32);
            x = -1;
        }
        serialPrint("\e[K");
        x++;
    }
}

void initMSP(void)
{
    /* Stop Watchdog Timer*/
    WDTCTL = WDTPW + WDTHOLD;

    /* If the calibration constants were erased, stop here.*/
    if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF)
    {
        while (1)
        {

        }
    }
    BCSCTL1 = CALBC1_1MHZ;

    /* Set DCO step + modulation*/
    DCOCTL = CALDCO_1MHZ;

    /* Now enable the global interrupts*/
    __enable_interrupt();

    /* Boot-up message*/
    serialWrite(0x0C);
    serialPrintln("Launchpad booted.");
}

/* TA1*/
void console_TA1_init(void)
{
    /* SMCLK, count up to CCR0*/
    TA1CTL = TASSEL_2;

    /* CCR0 interrupt enabled*/
    TA1CCTL0 = CCIE;

    /* Tint = 1 ms @ fclk = 1 MHz & VALUE = 1000*/
    TA1CCR0 = 60000;

}

/* TA1 interrupt*/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TA1CCR0_ISR(void)
{
    milisecond++;

    if (milisecond > TIME)
    {
        milisecond = 0;
    }
}

/* GPIO interrupt for the ultrasonic sensor*/
# pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    /**/
    TA0CCTL0 ^= CCIS0;

    /* Pullup / Pulldown Resistor disabled for P1.0*/
    P1REN &= ~BIT0;

    /* Pullup disabled for P1.0*/
    P1OUT &= ~BIT0;

    /* disable High /Low - Edge*/
    P1IES &= ~BIT0;

    /* Disable interrupt*/
    P1IE &= ~BIT0;

    /**/
    usonic_StopTA0();

    /**/
    distance = TA0CCR0;

    /* Clear interrupt flag*/
    P1IFG &= ~BIT0;

    console_StartTA1();
}
