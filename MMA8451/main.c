/***************************************************************************//**
 * @file    main.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 3>
 * @date    <11.06.2022>
 * @brief   Exercise 3 - Advanced Sensors
 *
 * Description and pin connections go here.
 *
 * CON6:I2C_SPI -> CON2:P1.3
 * CON6:XSCL    -> CON2:P1.6
 * CON6:XSDA    -> CON2:P1.7
 * CON6:UDAC    -> CON5:BCKL
 * CON5:D4 -> CON3:P2.0
 * CON5:D5 -> CON3:P2.1
 * CON5:D6 -> CON3:P2.2
 * CON5:D7 -> CON3:P2.3
 * CON5:RS -> CON4:P3.0
 * CON5:R/W -> CON4:P3.1
 * CON5:E -> CON4:P3.2
 *
 * @note    The project was exported using CCS 8.0.0.
 *
 ******************************************************************************/

#include "libs/templateEMP.h"   // UART disabled, see @note!
#include "libs/mma.h"
#include "libs/lcd.h"

unsigned char bubble_state(unsigned char state);
void bubble_treshold(void);

unsigned char error = 0;
unsigned char i;
volatile int count = 0;
volatile int inter = 0;
volatile int sleep = 0;
volatile int first = 0;
volatile int state, state_mem;
int acc;

/*Custom characters*/
unsigned char bubble_up6_r[8] =
        { 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char bubble_up6_l[] =
        { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char bubble_up5_r[8] =
        { 0x1C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char bubble_up5_l[] =
        { 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char bubble_up4_r[8] =
        { 0x1C, 0x1C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char bubble_up4_l[] =
        { 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char bubble_up3_r[8] =
        { 0x1C, 0x1C, 0x1C, 0x18, 0x00, 0x00, 0x00, 0x00 };
unsigned char bubble_up3_l[] =
        { 0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00 };

unsigned char bubble_up2_r[8] =
        { 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x00, 0x00, 0x00 };
unsigned char bubble_up2_l[] =
        { 0x07, 0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00 };

unsigned char bubble_up1_r[8] =
        { 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x00, 0x00 };
unsigned char bubble_up1_l[] =
        { 0x03, 0x07, 0x07, 0x07, 0x07, 0x03, 0x00, 0x00 };

unsigned char bubble_mid_r[8] =
        { 0x00, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x00 };
unsigned char bubble_mid_l[] =
        { 0x00, 0x03, 0x07, 0x07, 0x07, 0x07, 0x03, 0x00 };

unsigned char bubble_down1_r[8] = { 0x00, 0x00, 0x18, 0x1C, 0x1C, 0x1C, 0x1C,
                                    0x18 };
unsigned char bubble_down1_l[] = { 0x00, 0x00, 0x03, 0x07, 0x07, 0x07, 0x07,
                                   0x03 };

unsigned char bubble_down2_r[8] = { 0x00, 0x00, 0x00, 0x18, 0x1C, 0x1C, 0x1C,
                                    0x1C };
unsigned char bubble_down2_l[] = { 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x07,
                                   0x07 };

unsigned char bubble_down3_r[8] = { 0x00, 0x00, 0x00, 0x00, 0x18, 0x1C, 0x1C,
                                    0x1C };
unsigned char bubble_down3_l[] = { 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07,
                                   0x07 };

unsigned char bubble_down4_r[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1C,
                                    0x1C };
unsigned char bubble_down4_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07,
                                   0x07 };

unsigned char bubble_down5_r[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
                                    0x1C };
unsigned char bubble_down5_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
                                   0x07 };

unsigned char bubble_down6_r[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x18 };
unsigned char bubble_down6_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x03 };

int main(void)
{
    initMSP();
    // TODO: Add your initialization here.
    mma_init();
    lcd_init();
    __delay_cycles(10000);

    error = 1;
    while (error == 1)
    {
        for (i = 0; i < 2; i++)
        {
            lcd_clear();
            __delay_cycles(50000);
            lcd_putText("SELF-TEST");
            __delay_cycles(5000000);
            lcd_clear();
            __delay_cycles(50000);
            lcd_putText("SELF-TEST.");
            __delay_cycles(5000000);
            lcd_clear();
            __delay_cycles(50000);
            lcd_putText("SELF-TEST..");
            __delay_cycles(5000000);
            lcd_clear();
            __delay_cycles(50000);
            lcd_putText("SELF-TEST...");
            __delay_cycles(5000000);
        }
        lcd_clear();
        __delay_cycles(50000);
        lcd_putText("SELF-TEST:");
        __delay_cycles(5000000);
        error = mma_selftest(); // Self test
        if (error == 0)
        {
            lcd_clear();
            __delay_cycles(500000);
            lcd_putText("SELF-TEST:PASSED");
            __delay_cycles(50000000);
            lcd_clear();
        }
        else
        {
            lcd_clear();
            __delay_cycles(500000);
            lcd_putText("SELF-TEST:FAILED");
            __delay_cycles(50000000);
            lcd_clear();
            __delay_cycles(500000);
            lcd_putText("RERUN SELF-TEST");
            __delay_cycles(50000000);
            lcd_clear();
        }
    }

    mma_setResolution(1); // 14 bit resolution
    configure_gpio();
    mma_enableTapInterrupt();

    while (1)
    {
        if ((inter == 1) & (first == 2))
        {

            lcd_putText("WAKE UP");
            __delay_cycles(5000000);
            lcd_clear();
            __delay_cycles(50000);
            inter = 0;
            sleep = 0;
        }

        if (sleep == 1)
        {
            // disable timer interrupts
            TA1CCTL0 &= ~CCIE;
            count = 0;
            lcd_putText("SLEEPING");
            __delay_cycles(50000000);
            lcd_clear();
            __delay_cycles(50000);
            sleep = 2;
            __bis_SR_register(LPM3_bits + GIE);      // Enter LPM3 w/ interrupts
        }
        else if (sleep == 0)
        {
            bubble_treshold();
        }
    }
}

// Port 1 interrupt vector
# pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (first < 2)
    {
        first++;
    }
    __bic_SR_register_on_exit(LPM3_bits);          // Exit LPM0
    inter = 1;
    P1IFG &= ~BIT0;
}

/*Timer interrupt*/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void myTimerISR(void)
{
    // P3 .6 output
    P3DIR |= BIT6;
    // P3 .6 TA0.2 option
    P3SEL |= BIT6;
    if (count == 0)
    {
        state_mem = state;
    }
    count++;
    if (count == 320)
    {
        count = 0;
        // disable timer interrupts
        TA1CCTL0 &= ~CCIE;
        if (state == state_mem)
        {
            sleep = 1;
            TA1R &= 0;
            P1IE |= BIT0;       // Enable interrupt
        }
    }
}

/*Set threshold values*/
void bubble_treshold(void)
{
    acc = mma_get14Y();

    if ((acc >= 0) & (acc < 13))
    {
        state = bubble_state(20);
    }
    else if ((acc >= 13) & (acc < 26))
    {
        state = bubble_state(19);
    }
    else if ((acc >= 26) & (acc < 39))
    {
        state = bubble_state(18);
    }
    else if ((acc >= 39) & (acc < 52))
    {
        state = bubble_state(17);
    }
    else if ((acc >= 52) & (acc < 65))
    {
        state = bubble_state(16);
    }
    else if ((acc >= 65) & (acc < 78))
    {
        state = bubble_state(15);
    }
    else if ((acc >= 78) & (acc < 91))
    {
        state = bubble_state(14);
    }
    else if ((acc >= 91) & (acc < 104))
    {
        state = bubble_state(13);
    }
    else if ((acc >= 104) & (acc < 117))
    {
        state = bubble_state(12);
    }
    else if ((acc >= 117) & (acc < 130))
    {
        state = bubble_state(11);
    }
    else if ((acc >= 130) & (acc < 143))
    {
        state = bubble_state(10);
    }
    else if ((acc >= 143) & (acc < 156))
    {
        state = bubble_state(9);
    }
    else if ((acc >= 156) & (acc < 169))
    {
        state = bubble_state(8);
    }
    else if ((acc >= 169) & (acc < 182))
    {
        state = bubble_state(7);
    }
    else if ((acc >= 182) & (acc < 195))
    {
        state = bubble_state(6);
    }
    else if ((acc >= 195) & (acc < 208))
    {
        state = bubble_state(5);
    }
    else if ((acc >= 208) & (acc < 221))
    {
        state = bubble_state(4);
    }
    else if ((acc >= 221) & (acc < 234))
    {
        state = bubble_state(3);
    }
    else if ((acc >= 234) & (acc < 247))
    {
        state = bubble_state(2);
    }
    else if ((acc >= 247) & (acc < 260))
    {
        state = bubble_state(1);
    }
    else if ((acc >= 260) & (acc < 273))
    {
        state = bubble_state(0);
    }
    // enable timer interrupts
    TA1CCTL0 = CCIE;
}

unsigned char bubble_state(unsigned char state)
{
    switch (state)
    {
    case 0:

        CreateCustomCharacter(bubble_down6_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down6_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 1:
        CreateCustomCharacter(bubble_down5_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down5_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 2:
        CreateCustomCharacter(bubble_down4_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down4_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 3:
        CreateCustomCharacter(bubble_down3_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down3_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 4:
        CreateCustomCharacter(bubble_down2_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down2_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 5:
        CreateCustomCharacter(bubble_down1_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down1_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 6:
        CreateCustomCharacter(bubble_mid_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_mid_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 7:
        CreateCustomCharacter(bubble_up1_l, 0);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up1_r, 1);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 8:
        CreateCustomCharacter(bubble_up2_l, 2);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(2);        //Display character
        CreateCustomCharacter(bubble_up2_r, 3);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(3);        //Display character

        CreateCustomCharacter(bubble_down6_l, 4);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(4);        //Display character
        CreateCustomCharacter(bubble_down6_r, 5);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(5);        //Display character
        break;
    case 9:
        CreateCustomCharacter(bubble_up3_l, 2);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(2);        //Display character
        CreateCustomCharacter(bubble_up3_r, 3);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(3);        //Display character

        CreateCustomCharacter(bubble_down5_l, 4);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(4);        //Display character
        CreateCustomCharacter(bubble_down5_r, 5);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(5);        //Display character
        break;
    case 10:
        CreateCustomCharacter(bubble_up4_l, 2);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(2);        //Display character
        CreateCustomCharacter(bubble_up4_r, 3);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(3);        //Display character

        CreateCustomCharacter(bubble_down4_l, 4);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(4);        //Display character
        CreateCustomCharacter(bubble_down4_r, 5);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(5);        //Display character
        break;
    case 11:
        CreateCustomCharacter(bubble_up5_l, 2);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(2);        //Display character
        CreateCustomCharacter(bubble_up5_r, 3);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(3);        //Display character

        CreateCustomCharacter(bubble_down3_l, 4);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(4);        //Display character
        CreateCustomCharacter(bubble_down3_r, 5);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(5);        //Display character
        break;
    case 12:
        CreateCustomCharacter(bubble_up6_l, 2);
        lcd_cursorSet(8, 1); // Cursor -> second row 8th position
        lcd_putChar(2);        //Display character
        CreateCustomCharacter(bubble_up6_r, 3);
        lcd_cursorSet(9, 1); // Cursor -> second row 9th position
        lcd_putChar(3);        //Display character

        CreateCustomCharacter(bubble_down2_l, 4);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(4);        //Display character
        CreateCustomCharacter(bubble_down2_r, 5);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(5);        //Display character
        break;
    case 13:
        CreateCustomCharacter(bubble_down1_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_down1_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 14:
        CreateCustomCharacter(bubble_mid_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_mid_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 15:
        CreateCustomCharacter(bubble_up1_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up1_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 16:
        CreateCustomCharacter(bubble_up2_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up2_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 17:
        CreateCustomCharacter(bubble_up3_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up3_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 18:
        CreateCustomCharacter(bubble_up4_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up4_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 19:
        CreateCustomCharacter(bubble_up5_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up5_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    case 20:
        CreateCustomCharacter(bubble_up6_l, 0);
        lcd_cursorSet(8, 0); //Cursor -> first row 8th position
        lcd_putChar(0);        //Display character
        CreateCustomCharacter(bubble_up6_r, 1);
        lcd_cursorSet(9, 0); // Cursor -> first row 9th position
        lcd_putChar(1);        //Display character
        break;
    }
    __delay_cycles(500000);
    lcd_clear();
    __delay_cycles(50000);
    return state;
}

