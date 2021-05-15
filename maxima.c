#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE     32
#define SCAN_RANGE      1
float buffer[BUFFER_SIZE];
float maxima_array[];

/* THIS IS THE FUNCTION WE NEED */
int maxima(float *buffer)
{
    int i, j, maxima_pos=0;
    float max_value=0;
    for(i=0; i< BUFFER_SIZE; i++)
    {
        if(buffer[i] < 0) buffer[i] = -buffer[i];
    }
    for(i = SCAN_RANGE; i < BUFFER_SIZE-SCAN_RANGE; i++)
    {
        for(j=i-SCAN_RANGE; j <= i+SCAN_RANGE; j++)
        {
            if(max_value < buffer[j])   max_value = buffer[j];
        }
        if(max_value == buffer[i])
        {
            maxima_array[maxima_pos] = buffer[i];
            maxima_pos++;
        }
        max_value = 0;
    }
    return maxima_pos;          //return number of elements in maxima array
}
/*********** END OF FUNCTION *************/

int main()
{
    buffer[0] = 0.0000;
    buffer[1] = 0.3944;
    buffer[2] = 0.7248;
    buffer[3] = 0.9378;
    buffer[4] = 0.9987;
    buffer[5] = 0.8978;
    buffer[6] = 0.6514;
    buffer[7] = 0.2994;
    buffer[8] = -0.1012;
    buffer[9] = -0.4853;
    buffer[10] = -0.7908;
    buffer[11] = -0.9681;
    buffer[12] = -0.9885;
    buffer[13] = -0.8486;
    buffer[14] = -0.5713;
    buffer[15] = -0.2013;
    buffer[16] = 0.2013;
    buffer[17] = 0.5713;
    buffer[18] = 0.8486;
    buffer[19] = 0.9885;
    buffer[20] = 0.9681;
    buffer[21] = 0.7908;
    buffer[22] = 0.4853;
    buffer[23] = 0.1012;
    buffer[24] = -0.2994;
    buffer[25] = -0.6514;
    buffer[26] = -0.8978;
    buffer[27] = -0.9987;
    buffer[28] = -0.9378;
    buffer[29] = -0.7248;
    buffer[30] = -0.3944;
    buffer[31] = -0.0000;

    int i, sizeof_maxima_array;
    sizeof_maxima_array = maxima(buffer);
    printf("buffer size = %d\n", sizeof(buffer)/sizeof(float));
    printf("maxima_array size = %d\n\n", sizeof_maxima_array);
    for(i=0; i< sizeof_maxima_array; i++)
        printf("maxima[%d] = %f\n", i, maxima_array[i]);
    getchar();
}