#include <iostream>
#include <string.h>
using namespace std;

class SBUS {

public:
    int receiverBuffer[25];

    void read_buffer (char buffer[])
    {
        //converts and saves the buffer to internal one
        for (int i =0; i < 25; i++)
        {
            receiverBuffer[i] = (int)buffer[i];
        }
    }

    //should be repeated 12 times 1-13
    void int8_to_string (int number)
    {
        unsigned char help = 128;

        for (int i = 0; i < 8; i++)
        {
            if (receiverBuffer[number] & help)
            {
                binOut += to_string(1);
            } else {
                binOut += to_string(0);
            }
            help >>=1;
        }
    }

    //enter
    int channel (int channel)
    {
        //only 8 channels are scanned
        if (channel > 8 || channel < 1)
        {
            return -1;
        }

        return get_channel(channel);
    }

private:
    string binOut;

    int get_channel (int pos)
    {
        //goes to exact location in the binary array and convert
        //next 12bits to int
        // (1 channel is 12bit long but its transmitted by 8bits)
        string aura = binOut.substr(pos * 12, 12);
        //convert the binary string to an integer
        return stoi(aura, 0, 2);
    }
};

class receiver {

public:
    bool flags[2];
    string binString;

    /*
     * TO DO:
     *  1.) get flags
     *  2.) check for individual packets
     *  3.) optimize
     */

    void read_buffer (int receiverBuffer[])
    {
        //clears the string -> before cycles were appended to the end of the string
        //bluepill run out of memory
        binString.clear();
        bin_to_string(receiverBuffer);
    }

    //get the channel
    int channel(int number)
    {
        //number -1 because arrays starts with 0 :)
        number--;

        //only 8 channels are scanned
        if (number > 8 || number < 0)
        {
            return -1;
        }
        //returns the value of selected channel
        return get_channel(number);
    }

private:
    int *binArr;

    //converts 8bit int to binary array
    int* int8_to_bin (int number)
    {
        static int buffer[8];
        //sets the arr to 0 -> resets the arr
        memset(buffer, 0, sizeof buffer);
        int i = 7;

        while (number != 0)
        {
            buffer[i] = number % 2;
            number /= 2;
            i--;
        }
        return buffer;
    }

    //converts whole raw sbus receiver channel to
    //one long binary number stored in string
    string bin_to_string(int receiverBuffer[])
    {
        for (int i = 1; i < 13; i++)
        {
            //converts the int to the binary arr
            binArr = int8_to_bin(receiverBuffer[i]);
            for (int x = 0; x < 8; x++)
            {
                //append the binary to one big string dru
                binString += to_string(*(binArr + x));
            }
        }
        return binString;
    }

    //cuts the long binary string from previous function
    //and converts it to integer
    int get_channel(int pos)
    {
        //goes to exact location in the binary array and convert
        //next 12bits to int
        // (1 channel is 12bit long but its transmitted by 8bits)
        string aura = binString.substr(pos * 12, 12);
        //convert the binary string to an integer
        return stoi(aura, 0, 2);
    }

    int get_flags(char receiverBuffer[])
    {
        
    }
};

int main() {
    //test receiverBuffer
    //arr[0] = start bit(240d, F0h, 11110000b)
    char arr[25] = {20, 15, 43, 34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,90};
    int chan;
    //testing the class
    SBUS sbus;

    while (1)
    {
        sbus.read_buffer(arr);

        for (int i = 1; i < 13; i++)
        {
            sbus.int8_to_string(i);
        }

        chan = sbus.channel(1);
        printf("%d \n", chan);
    }
    return 0;
}
