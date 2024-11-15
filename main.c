//
////defines:
//#define xdc__strict //suppress typedef warnings
//#define rxBBuffer_SIZE 25  // Size of data that get send via SCIb
//#define BUFFER_SIZE 7    //Amount of cookies on the belt
//
////includes:
//#include <xdc/std.h>
//#include <xdc/runtime/System.h>
//#include <ti/sysbios/BIOS.h>
//#include <Headers/F2837xD_device.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ti/sysbios/knl/Swi.h>
//#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Semaphore.h>
//
//
//// Function prototypes
//extern void DeviceInit(void);
//extern void DelayUs(Uint16);
//extern void scib_xmit(int a);
//extern void scib_msg(char *msg);
//extern void scic_xmit(int a);
//extern void scic_msg(char *msg);
//extern void sendCMD(int xVal, bool dir);
//extern void suck(bool answer);
//
//void Convt2mm(void);
//void Save2Buffer(void);
//void clearBuff(void);
//
////SWIs
//extern const Swi_Handle SwiSCIbRX;
//extern const Swi_Handle SwiSCIcRX;
//
////TSKs
//extern const Task_Handle DeckTimers;
//extern const Semaphore_Handle mySem;
//extern const Task_Handle Convert;
//extern const Semaphore_Handle DSP;
//
////Globals
//volatile UInt tickCount = 0; //counter incremented by timer interrupt
//bool LEDFlag = FALSE;
//bool gotCookie = FALSE;
//bool yes_please = TRUE;
//bool no_thanks = FALSE;
//bool up = TRUE;
//bool down = FALSE;
//
//int the_x = 0;
//int previous_value;
//int stage_timer1 = -1,stage_timer2 = -1, stage_timer3 = -1; //timers for moving the arm
//bool got_cookie = FALSE;
//bool stage1 = FALSE, stage2 = FALSE, stage3 = FALSE, stage4 = FALSE;// variables for the shift reg to move the are
//
//char rxBBuffer[rxBBuffer_SIZE];
//char rxCBuffer[rxBBuffer_SIZE];
//int Buffer[BUFFER_SIZE][2]; //[cookieIndex](time till pickup, x)
//volatile int time_2_pick = 0;
//char *RXBufptr = rxBBuffer;
//char *RXCBufptr = rxCBuffer;
//
//static float mm_ptp_y = 605;   //point to point mm
//static float pix_ptp_y = 1024; //point to point pixels
//
//static float mm_ptp_x = 180;  //point to point mm
//static float pix_ptp_x = 309; //point to point pixels
//
//static float mm_to_cent = 110; //distance from center to edge of belt
//static float mm_to_pnt = 42;   //distance from point to edge of belt
//
//static float belt_speed = 30; // in mm/s


// Defines
#define xdc__strict            // Suppress typedef warnings
#define rxBuffer_SIZE 25      // Size of data sent via SCIb
#define BUFFER_SIZE 7          // Number of cookies on the belt

// Includes
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <Headers/F2837xD_device.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

// Function Prototypes
extern void DeviceInit(void);
extern void DelayUs(Uint16);
extern void scib_xmit(int a);
extern void scib_msg(char *msg);
extern void scic_xmit(int a);
extern void scic_msg(char *msg);
extern void sendCMD(int xVal, bool dir);
extern void suck(bool answer);

void Convt2mm(void);
void Save2Buffer(void);
void clearBuff(void);

// Software Interrupts
extern const Swi_Handle SwiSCIbRX;
extern const Swi_Handle SwiSCIcRX;

// TSKs
extern const Task_Handle DeckTimers;
extern const Semaphore_Handle mySem;
extern const Task_Handle Convert;
extern const Semaphore_Handle DSP;

// Global Variables - Booleans
bool LEDFlag = FALSE;
bool gotCookie = FALSE;
bool yes_please = TRUE;
bool no_thanks = FALSE;
bool up = TRUE;
bool down = FALSE;
bool got_cookie = FALSE;
bool stage1 = FALSE, stage2 = FALSE, stage3 = FALSE, stage4 = FALSE, stage5 = FALSE; // Shift reg variables for arm movement

// Global Variables - Integers
volatile UInt tickCount = 0;   // Counter incremented by timer interrupt
int the_x = 0;
int previous_value = 0;
int stage_timer1 = -1, stage_timer2 = -1, stage_timer3 = -1, stage_timer4 = -1;; // Timers for arm movement
volatile int time_2_pick = 0;

// Buffers
char rxBBuffer[rxBuffer_SIZE];  // RX buffer for SCI B
char rxCBuffer[rxBuffer_SIZE];  // RX buffer for SCI c
int Buffer[BUFFER_SIZE][2];      // [cookieIndex] [time till pickup, x]
char *RXBufptr = rxBBuffer;      // RXB Pointer
char *RXCBufptr = rxCBuffer;     // RXC Pointer

// Measurement Constants - Floats
static float mm_ptp_y = 605;    // Point-to-point in mm (y-axis)
static float pix_ptp_y = 1024;  // Point-to-point in pixels (y-axis)
static float mm_ptp_x = 180;    // Point-to-point in mm (x-axis)
static float pix_ptp_x = 309;   // Point-to-point in pixels (x-axis)
static float mm_to_cent = 110;  // Distance from center to edge of belt in mm
static float mm_to_pnt = 42;    // Distance from point to edge of belt in mm
static float belt_speed = 31;   // Belt speed in mm/s

// Main function
Int main()
{ 
    System_printf("Enter main()\n"); // Use ROV->SysMin to view characters in circular buffer

    // Initialization
    DeviceInit();           // Initialize processor
    clearBuff();
    sendCMD(150, up);       // Initialize arm position

    // Jump to RTOS (does not return)
    BIOS_start();
    return 0;
}

// Initializes the buffer to all -1
// AW
void clearBuff()
{
    int i;
    for (i = 0; i < BUFFER_SIZE; i++) {
        memset(Buffer[i], -1, sizeof(Buffer[i]));  // Set each row in Buffer to -1
    }
}

// SWI
// This SWI saves the rxBBuffer to Buffer, then clears rxBBuffer.
// Once saved, it converts the pixel points to x in mm and calculates time until pickup.
// AW
void Save2Buffer(void)
{
    int i;
    int tempIndex1 = 0, tI2 = 0;
    char tempStr[5];                // Temporary string for parsed numbers

    // Shift existing data in Buffer to make room for the new cookie
    for (i = BUFFER_SIZE - 2; i >= 0; i--) {
        Buffer[i + 1][0] = Buffer[i][0];
        Buffer[i + 1][1] = Buffer[i][1];
    }

    // Parse rxBBuffer into Buffer as integers
    for (i = 0; i < rxBuffer_SIZE; i++) {
        // End of input, finalize last parsed value
        if (*RXBufptr == '\0') {
            if (tempIndex1 > 0) {
                tempStr[tempIndex1] = '\0';
                Buffer[0][tI2] = atoi(tempStr);
            }
            break;
        }
        // Parse each value separated by a comma
        else if (*RXBufptr == ',') {
            tempStr[tempIndex1] = '\0';
            Buffer[0][tI2] = atoi(tempStr);
            tI2++;                                  // Move to next index in Buffer

            memset(tempStr, '\0', sizeof(tempStr)); // Reset tempStr for next value
            tempIndex1 = 0;
        }
        // Continue building the number in tempStr
        else {
            tempStr[tempIndex1] = *RXBufptr;
            tempIndex1++;
        }
        RXBufptr++;
    }

    // Clear rxBBuffer and reset pointer
    memset(rxBBuffer, 0, rxBuffer_SIZE);
    RXBufptr = rxBBuffer;

    Semaphore_post(DSP);  // Signal DSP semaphore
}


void SaveArmBuffer(void)
{

}

// HWI
// Triggered when there is data in the RX B register
// Saves the data in rxBBuffer
// AW
void SCIB_HWI(void)
{
    char receivedChar = ScibRegs.SCIRXBUF.all;  // Read the received character

    if (receivedChar == '\n') {
        *RXBufptr = '\0';                      // Null-terminate the string
        RXBufptr = rxBBuffer;                  // Reset RXBufptr for the next message
        GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1; // Toggle Blue LED to show input
        Swi_post(SwiSCIbRX);                   // Trigger software interrupt for processing
    } else {
        *RXBufptr = receivedChar;              // Store received character in buffer
        RXBufptr++;                            // Move to next buffer position
    }

    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;       // Clear RX interrupt flag
}

// HWI
// Triggered when there is data in the RX C register
// Saves the data in rxCBuffer (response from the arm)
// AW
void SCIC_HWI(void)
{
    char receivedChar = ScicRegs.SCIRXBUF.all;  // Read the received character

    if (receivedChar == '\n') {
        *RXCBufptr = '\0';                      // Null-terminate the string
        RXCBufptr = rxCBuffer;                  // Reset RXCBufptr for the next message
        GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;  // Toggle Blue LED to show input
        Swi_post(SwiSCIcRX);                    // Trigger software interrupt for processing
    } else {
        *RXCBufptr = receivedChar;              // Store received character in buffer
        RXCBufptr++;                            // Move to next buffer position
    }

    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;        // Clear RX interrupt flag
}

// TSK
// This task handles the cookie buffer and arm movement
// Sends commands to the arm when timers expire
// AW
void decrementTime(void)
{
    while(TRUE)
    {
        Semaphore_pend(mySem, BIOS_WAIT_FOREVER); // Wait for semaphore to be posted
        int i;

        if (stage1) {
            sendCMD(the_x, up);       // Move arm up to a safe height
            stage1 = FALSE;
        }

        if (stage2) {
            suck(yes_please);          // Activate suction
            sendCMD(the_x, down);      // Move arm down to pick up cookie
            stage_timer2 = 800;       // Timer until cookie pickup is complete
            stage2 = FALSE;
        }

        if (stage3) {
            scic_msg("G0 X260 Y0 Z50 F10000\n");          // Move arm up to clear the belt
            stage3 = FALSE;
            stage_timer3 = 1700;       // Timer to wait till cookie is off belt
        }

        if (stage4) {
            suck(no_thanks);            // Release cookie off belt
            got_cookie = FALSE;         // Reset flag for next cookie
            stage4 = FALSE;
            stage_timer4 = 200;
        }
        if (stage5) {
            got_cookie = FALSE;
            stage5 = FALSE;
        }

        // Decrement timers if active
        if (stage_timer1 > 0) stage_timer1--;
        if (stage_timer2 > 0) stage_timer2--;
        if (stage_timer3 > 0) stage_timer3--;
        if (stage_timer4 > 0) stage_timer4--;

        // Transition to the next stage when timers reach zero
        if (stage_timer1 == 0) {
            stage_timer1 = -1;
            stage2 = TRUE;
        }
        if (stage_timer2 == 0) {
            stage_timer2 = -1;
            stage3 = TRUE;
        }
        if (stage_timer3 == 0) {
            stage_timer3 = -1;
            stage4 = TRUE;
        }
        if (stage_timer4 == 0) {
            stage_timer4 = -1;
            stage5 = TRUE;
        }

        // Process cookie buffer countdown
        for (i = 0; i < BUFFER_SIZE; i++) {
            if (Buffer[i][0] > 0) {
                Buffer[i][0] -= 1;                         // Countdown each cookie timer
            }
            if (Buffer[i][0] == -1 && previous_value > 0 && !got_cookie) {
                got_cookie = TRUE;
                the_x = Buffer[i - 1][1];                  // Getting the x position of the cookie with lowest timer
                stage1 = TRUE;                             // Move arm to x position and wait
            }
            if (Buffer[i][0] == 0) {
                previous_value = Buffer[i][0];
                memset(Buffer[i], -1, sizeof(Buffer[i]));  // Clear buffer slot
                stage_timer1 = 10;                         // Reset timer for next stage
            } else {
                previous_value = Buffer[i][0];
            }
        }
    }
}

// This function executes once every millisecond
// Posts the semaphore every millisecond
// AW
Void myTikFxn(UArg arg)
{
    tickCount++;                   // Increment the tick counter
    Semaphore_post(mySem);         // Trigger decrementTime task every millisecond

    if (tickCount % 1000 == 0) {   // Every second, toggle LED flag to indicate program is running
        LEDFlag = TRUE;
    }
}

// Idle function to toggle LED
void myIdleFxn(void)
{
    if (LEDFlag) {
        LEDFlag = FALSE;                  // Reset flag after toggling
        GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1; // Toggle red LED
    }
}

// Task for DSP processing
// Converts x position to mm from the base of the Arm
// Converts y to ms from the center point of the arm
// AW
void Convt2mm(void)
{
    while(TRUE)
    {
        Semaphore_pend(DSP, BIOS_WAIT_FOREVER); // Wait for semaphore to be posted

        float offset = mm_to_cent - mm_to_pnt;  // Distance from center of base to ArUco marker corner
        float middle_pnt = mm_ptp_y / 2;        // Middle point of the belt

        // Get coordinates from buffer (camera x -> y, camera y -> x in our system)
        float y = Buffer[0][0];
        float x = Buffer[0][1];

        // Calculate pixel-to-mm conversion factors
        float px2mmx = mm_ptp_x / pix_ptp_x;
        float py2mmy = mm_ptp_y / pix_ptp_y;

        // Convert x position from pixels to mm and adjust with offset
        x = x * px2mmx + offset;
        Buffer[0][1] = x;                      // Save converted x distance in mm

        // Convert y position from pixels to mm
        y = y * py2mmy;
        float dist_to_center = middle_pnt - y; // Calculate distance from belt center

        // Calculate time for cookie to reach the center at current belt speed
        int time_till_pick = dist_to_center / (belt_speed / 1000); // Belt_speed converted to mm/mS
        Buffer[0][0] = time_till_pick;                             // Save time until pickup in buffer
    }
}
