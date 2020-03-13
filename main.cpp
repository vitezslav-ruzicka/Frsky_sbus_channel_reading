#include "mbed.h"

/*
*   TO DO LIST:
*   1. make the get_channel_values function to work
*   2. separete the flags from channels
*   3. add more security to the program
*   4. test
*/

//#define ALL_CHANNELS
#define SBUS_SIGNAL_OK          0x00
#define SBUS_SIGNAL_LOST        0x01
#define SBUS_SIGNAL_FAILSAFE    0x03

Serial sbus(NC, PB_11);
Serial pc(PA_9, PA_10);

//not tested
void get_channel_values (uint8_t *channels, uint8_t *failsafe_status, char buffer[])
{
    //get the value from channels
    channels[0]  = ((buffer[1]|buffer[2]<< 8) & 0x07FF);
    channels[1]  = ((buffer[2]>>3|buffer[3]<<5) & 0x07FF);
    channels[2]  = ((buffer[3]>>6|buffer[4]<<2|buffer[5]<<10) & 0x07FF);
    channels[3]  = ((buffer[5]>>1|buffer[6]<<7) & 0x07FF);
    channels[4]  = ((buffer[6]>>4|buffer[7]<<4) & 0x07FF);
    channels[5]  = ((buffer[7]>>7|buffer[8]<<1|buffer[9]<<9) & 0x07FF);
    channels[6]  = ((buffer[9]>>2|buffer[10]<<6) & 0x07FF);
    channels[7]  = ((buffer[10]>>5|buffer[11]<<3) & 0x07FF);
#ifdef ALL_CHANNELS
    channels[8]  = ((buffer[12]|buffer[13]<< 8) & 0x07FF);
    channels[9]  = ((buffer[13]>>3|buffer[14]<<5) & 0x07FF);
    channels[10] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
    channels[11] = ((buffer[16]>>1|buffer[17]<<7) & 0x07FF);
    channels[12] = ((buffer[17]>>4|buffer[18]<<4) & 0x07FF);
    channels[13] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9) & 0x07FF);
    channels[14] = ((buffer[20]>>2|buffer[21]<<6) & 0x07FF);
    channels[15] = ((buffer[21]>>5|buffer[22]<<3) & 0x07FF);
#endif

    //check for signal state (signal OK, lost, failsafe)
    *failsafe_status = SBUS_SIGNAL_OK;
    if (buffer[23] & (1<<2)) {
        *failsafe_status = SBUS_SIGNAL_LOST;
    }
    if (buffer[23] & (1<<3)) {
        *failsafe_status = SBUS_SIGNAL_FAILSAFE;
    }

}

int main() {
    sbus.format(8, Serial::Even, 2);
    sbus.baud(100000);
    pc.baud(115200);

    char buffer[25];
    uint8_t channels[18];
    uint8_t failsave_status;
    int i;

    while (1)
    {
        sbus.gets(buffer, 25);
        pc.printf("%d   ",((buffer[1]|buffer[2]<< 8) & 0x07FF));
        pc.printf("%d   ",((buffer[2]>>3|buffer[3]<<5) & 0x07FF));
        pc.printf("%d   ",((buffer[3]>>6|buffer[4]<<2|buffer[5]<<10) & 0x07FF));
        pc.printf("%d   ", ((buffer[5]>>1|buffer[6]<<7) & 0x07FF));
        pc.puts("\n");
        wait_ms(100);
    }

}