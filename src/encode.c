#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PACKET_SIZE     256
#define THRESHOLD_VAL   10

// Performs thresholding with <THRESHOLD_VAL> and performs zero-length encoding
// @data: pointer to the dataset to encode; encodes <PACKET_SIZE> bytes at a time
// @output: pointer to the output buffer; assume max size of <PACKET_SIZE> bytes
// @return: final size of the encoded packet
int encode(float *data, float *output)
{
    int i, j; 
    char zero_length = 0;

    for (i = 0, j = 0; i < PACKET_SIZE, j < PACKET_SIZE; i++)
    {
       if (data[i] < THRESHOLD_VAL) 
       {
            if (!zero_length)
            {
                output[j] = 0;
                j++;
                output[j] = 0;
                zero_length = 1;
            }
            output[j]++;
       }
       else
       {
            if (zero_length)
            {
                zero_length = 0;
                j++;
            }
            output[j] = data[i];
            j++;
       }
    }
    return j;
}
