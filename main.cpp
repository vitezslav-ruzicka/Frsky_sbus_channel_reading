#include <iostream>
#include <string.h>
using namespace std;


class receiver {

public:
    bool flags[2];

    /*
     * TO DO:
     *  1.) get flags
     *  2.) check for individual packets
     *  3.) optimize
     */

    void read_buffer (int receiverBuffer[])
    {
        binString.clear();
        bin_to_string(receiverBuffer);
    }

    int channel(int number)
    {
        //number -1 because arrays starts with :)
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
    string binString;
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
    int arr[25] = {240, 15, 43, 434,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,90};
    receiver sbus;

    while (1) {
        //read the buffer and convert them into channels
        sbus.read_buffer(arr);
        //get the 2nd channel
        int chan = sbus.channel(2);
        //print the channel
        printf("%d \n", chan);
    }
    return 0;
}
