/***************************************************************************//**
 * @file    mma.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 3>
 * @date    <11.06.2022>
 *
 * @brief   <mma8451q header file>
 *
 * Here goes a detailed description if required.
 *
 * @ note   The X/Y-direction printed on the board are wrong:
//          X is the real Y and Y is the real X. Sorry for the confusion. :-(
 ******************************************************************************/

#ifndef EXERCISE_3_LIBS_MMA_H_
#define EXERCISE_3_LIBS_MMA_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "./i2c.h"

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
/*MMA8451Q Command Constans*/
#define CMD_LENGTH          2
#define RD8_LENGTH          1
#define RD14_LENGTH         2
#define REG_LENGTH          1
#define AX_LENGTH           3

#define NO_OF_SAMPLES       10

#define REG_IX              0
#define CMD_IX              1

#define DIV_14_2G           4096
#define DIV_14_4G           2048
#define DIV_14_8G           1024

#define DIV_8_2G            64
#define DIV_8_4G            32
#define DIV_8_8G            16

#define MASK                0xFFFFC000

#define GRAVITY             9.80665

/*MMA8451Q Slave Address*/
#define MMA8451Q    0x1D

/*MMA8451Q Register Addresses*/
#define F_STATUS            0x00    // Data Status R

#define OUT_X_MSB           0x01    // 14 bit X Data R
#define OUT_X_LSB           0x02    // 14 bit X Data R
#define OUT_Y_MSB           0x03    // 14 bit Y Data R
#define OUT_Y_LSB           0x04    // 14 bit Y Data R
#define OUT_Z_MSB           0x05    // 14 bit Z Data R
#define OUT_Z_LSB           0x06    // 14 bit Z Data R

#define F_SETUP             0x09    // FIFO Setup R/W

#define TRIG_CFG            0x0A    // FIFO Triggers R/W

#define SYSMOD              0x0B    // System Mode R

#define INT_SOURCE          0x0C    // Interrupt Status R

#define WHO_AM_I            0x0D    // ID Register R

#define XYZ_DATA_CFG        0x0E    // Data Config R/W

#define HP_FILTER_CUTOFF    0x0F    // HP Filter Setting R/W

#define PL_STATUS           0x10    // PL Status R
#define PL_CFG              0x11    // PL Configuration R/W
#define PL_COUNT            0x12    // PL DEBOUNCE R/W
#define PL_BF_ZCOMP         0x13    // PL Back/Front Z Comp R/W
#define P_L_THS_REG         0x14    // PL THRESHOLD R/W

#define FF_MT_CFG           0x15    // Freefall/Motion Config R/W
#define FF_MT_SRC           0x16    // Freefall/Motion Source R
#define FF_MT_THS           0x17    // Freefall/Motion Threshold R/W
#define FF_MT_COUNT         0x18    // Freefall/Motion Debounce R/W

#define TRANSIENT_CFG       0x1D    // Transient Config R/W
#define TRANSIENT_SRC       0x0E    // Transient Source R
#define TRANSIENT_THS       0x0F    // Transient Threshold R/W
#define TRANSIENT_COUNT     0x20    // Transient Debounce R/W

#define PULSE_CFG           0x21    // Pulse Config R/W
#define PULSE_SRC           0x22    // Pulse Source R
#define PULSE_THSX          0x23    // Pulse X Threshold R/W
#define PULSE_THSY          0x24    // Pulse Y Threshold R/W
#define PULSE_THSZ          0x25    // Pulse Z Threshold R/W
#define PULSE_TMLT          0x26    // Pulse First Timer R/W
#define PULSE_LTCY          0x27    // Pulse Latency R/W
#define PULSE_WIND          0x28    // Pulse 2nd Window R/W

#define ASLP_COUNT          0x29    // Auto-SLEEP Counter R/W

#define CTRL_REG1           0x2A    // Control Reg1 R/W
#define CTRL_REG2           0x2B    // Control Reg2 R/W
#define CTRL_REG3           0x2C    // Control Reg3 (WAKE Interrupts from SLEEP) R/W
#define CTRL_REG4           0x2D    // Control Reg4 (Interrupt Enable Map) R/W
#define CTRL_REG5           0x2E    // Control Reg5 (Interrupt Configuration) R/W

#define OFF_X               0x2F    // X 8-bit offset R/W
#define OFF_Y               0x30    // Y 8-bit offset R/W
#define OFF_Z               0x31    // Z 8-bit offset R/W

/*Clear Register*/
#define CLR                 0x00
/*Control Register 1 Bits*/
#define ASLP_RATE1          0x80
#define ASLP_RATE0          0x40
#define DR2                 0x20
#define DR1                 0x10
#define DR0                 0x08
#define LNOISE              0x04
#define F_READ              0x02
#define ACTIVE              0x01

/*Control Register 2 Bits*/
#define ST                  0x80
#define RST                 0x40
#define SMODS1              0x10
#define SMODS0              0x08
#define SLPE                0x04
#define MODS1               0x02
#define MODS0               0x01

/*Control Register 3 Bits*/
#define FIFO_GATE           0x80
#define WAKE_TRANS          0x40
#define WAKE_LNDPRT         0x20
#define WAKE_PULSE          0x10
#define WAKE_FF_MT          0x08
#define IPOL                0x02
#define PP_OD               0x01

/*Control Register 4 Bits*/
#define INT_EN_ASLP         0x80
#define INT_EN_FIFO         0x40
#define INT_EN_TRANS        0x20
#define INT_EN_LNDPRT       0x10
#define INT_EN_PULSE        0x08
#define INT_EN_FF_MT        0x04
#define INT_EN_DRDY         0x01

/*Control Register 5 Bits*/
#define INT_CFG_ASLP        0x80
#define INT_CFG_FIFO        0x40
#define INT_CFG_TRANS       0x20
#define INT_CFG_LNDPRT      0x10
#define INT_CFG_PULSE       0x08
#define INT_CFG_FF_MT       0x04
#define INT_CFG_DRDY        0x01

/*Data Config Register Bits*/
#define HPF_Out             0x10
#define FS1                 0x02
#define FS0                 0x01

/*Data Status Register Bits*/
#define ZYXOW               0x80
#define ZOW                 0x40
#define YOW                 0x20
#define XOW                 0x10
#define ZYXDR               0x08
#define ZDR                 0x04
#define YDR                 0x02
#define XDR                 0x01

/*Pulse Config Register Bits*/
#define DPA                 0x80
#define ELE                 0x40
#define ZDPEFE              0x20
#define ZSPEFE              0x10
#define YDPEFE              0x08
#define YSPEFE              0x04
#define XDPEFE              0x02
#define XSPEFE              0x01

/*Pulse First Timer Register Bits*/
#define TMLT7               0x80
#define TMLT6               0x40
#define TMLT5               0x20
#define TMLT4               0x10
#define TMLT3               0x08
#define TMLT2               0x04
#define TMLT1               0x02
#define TMLT0               0x01

/*Pulse Latency Register Bits*/
#define LTCY7               0x80
#define LTCY6               0x40
#define LTCY5               0x20
#define LTCY4               0x10
#define LTCY3               0x08
#define LTCY2               0x04
#define LTCY1               0x02
#define LTCY0               0x01

/*Pulse 2nd Window Register Bits*/
#define WIND7               0x80
#define WIND6               0x40
#define WIND5               0x20
#define WIND4               0x10
#define WIND3               0x08
#define WIND2               0x04
#define WIND1               0x02
#define WIND0               0x01

/*Pulse Z Threshold Register Bits*/
#define THSZ6               0x40
#define THSZ5               0x20
#define THSZ4               0x10
#define THSZ3               0x08
#define THSZ2               0x04
#define THSZ1               0x02
#define THSZ0               0x01


/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/
void configure_gpio(void);
unsigned char mma8451q_write_cmd(unsigned char reg, unsigned char cmd);
unsigned char mma8451q_read_byte(unsigned char reg);
unsigned char mma8451q_read_two_bytes(unsigned char reg);
double mma_getReal(unsigned char axis);
int mma_get14(unsigned char axis_reg);
signed char mma_get8(unsigned char axis);
unsigned char mma_gettest(unsigned char range);

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
extern double READ_AXIS[];

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// All configuration functions return 0 if everything went fine
// and anything but 0 if not (they are the ones with a unsigned char return type).

// Initialize the MMA with 8 bit resolution and 4G measurement range (1 pt.)
unsigned char mma_init(void);

// Change the measurement range. (0: 2g, 1: 4g, >1: 8g) (0.5 pt.)
unsigned char mma_setRange(unsigned char range);
// Change the resolution (0: 8 Bit, >= 1: 14 Bit) (0.5 pt.)
unsigned char mma_setResolution(unsigned char resolution);

// Run a self-test on the MMA, verifying that all three axis and all three
// measurement ranges are working. (1 pt.)
/* HINT:
 * The idea of the self test is that you measure the current acceleration values,
 * then enable the on-chip self-test and then read the values again.
 * The values without selftest enabled and those with selftest enabled
 * should now feature a predefined difference (see the datasheet).
 */
unsigned char mma_selftest(void);


// Set up the double tap interrupt on the MMA (do not set up the interrupt on
// the MSP in this function!). This means that the MMA should change the INT1-
// pin whenever a double tap is detected. You may freely choose the axis on
// which the tap has to be received. (You should put a comment in your code,
// which axis you chose, though). (1 pt.)
/* HINT:
 * As the datasheet for the MMA is a bit stingy when it comes to the double
 * tap stuff, so here's (roughly) what you should do:
 *
 *  1) Go to standby (as you can only change the registers when in standby)
 *  2) Write MMA_PULSE_CFG  to enable the z-axis for double tap
 *  3) Write MMA_PULSE_THSZ to set the tap threshold (e.g. to 2g)
 *  4) Write MMA_PULSE_TMLT to set the pulse time limit (e.g. to 100 ms)
 *  5) Write MMA_PULSE_LTCY to set the pulse latency timer (e.g. to 200 ms)
 *  6) Write MMA_PULSE_WIND to set the time window for the second tap
 *  7) Write MMA_CTRL_REG4  to set the pulse interrupt
 *  8) Write MMA_CTRL_REG5  to activate the interrupt on INT1
 *  9) Write MMA_CTRL_REG3  to set the interrupt polarity
 * 10) Return to active mode
 */
unsigned char mma_enableTapInterrupt(void);
// Disable the double-tap-interrupt on the MMA. (0.5 pt.)
unsigned char mma_disableTapInterrupt(void);


// Read the values of all three axis from the chip and store the values
// internally. Take the requested resolution into account. (1 pt.)
unsigned char mma_read(void);


/* Get Functions (1 pt. total): */

// Return the appropriate 8 bit values
// If the resolution during mma_read was 14 bit, translate the data to 8 bit
signed char mma_get8X(void);
signed char mma_get8Y(void);
signed char mma_get8Z(void);

// Return the appropriate 14 bit values
// If the resolution during mma_read was 8 bit, translate the data to 14 bit
int mma_get14X(void);
int mma_get14Y(void);
int mma_get14Z(void);

// Return the appropriate values in m*s^-2.
double mma_getRealX(void);
double mma_getRealY(void);
double mma_getRealZ(void);

#endif /* EXERCISE_3_LIBS_MMA_H_ */
