#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define FHT_SIZE        256
#define FHT_SIZE_LOG2   log(FHT_SIZE)/log(2)

// Lookup tables for the cas values that an n-point FHT calculation will use
// Indexes are on the log(2) scale and hold the cas values for a 2^i point FHT
// e.g. lt_sin_vals[5] will return a pointer to the cas values for a (2^5) point FHT
float **lt_sin_vals;
float **lt_cos_vals;

void lt_init(void)
{
    int i, j;

    lt_sin_vals = malloc((sizeof(float *) * FHT_SIZE_LOG2) + 1);
    lt_cos_vals = malloc((sizeof(float *) * FHT_SIZE_LOG2) + 1);

    // First loop allocates memory; an n-point FHT uses n/2 distinct cas values
    for (i = 1; i <= FHT_SIZE_LOG2; i++)
    {
        int distinct_vals = (int)(pow(2, i)) >> 1;
        float cas_param = M_PI / distinct_vals;

        lt_sin_vals[i] = malloc(sizeof(float) * distinct_vals);
        lt_cos_vals[i] = malloc(sizeof(float) * distinct_vals);
        
        // Second loop computes the cas values
        for (j = 0; j < distinct_vals; j++)
        {
            lt_sin_vals[i][j] = (float)sin(j * cas_param);
            lt_cos_vals[i][j] = (float)cos(j * cas_param);
        }
    }
}

// Fetch from the cas lookup tables
// e.g. lt_sin(3, 0) will return sin(0 * (2PI / (2^3)))
float lt_sin(int lt_idx, int lt_val)
{
    return lt_sin_vals[lt_idx][lt_val];
}

float lt_cos(int lt_idx, int lt_val)
{
    return lt_cos_vals[lt_idx][lt_val];
}

// FHT transform function for floats (4 bytes)
// @data: the data array to transform
// @size: number of data points in the <data> array
void fht(float *data, uint16_t n)
{
    int i, lt_idx;
    int half_n = n >> 1;
    float tw_factor;
    float even[half_n];
    float odd[half_n];

    printf("n: %d\n", n);
    if (n == 1) return;

    // Signal time decomposition (separating even and odd indexes)
    for (i = 0; i < half_n; i++)
    {
        even[i] = data[i << 1];
        odd[i] = data[(i << 1) + 1];
    } 
    fht(even, half_n);
    fht(odd, half_n);

    lt_idx = log(n) / log(2);

    // Zero and Nyquist frequency calculations
    tw_factor = odd[0] * ((lt_cos(lt_idx, 0) + lt_sin(lt_idx, 0)));
    data[0] = even[0] + tw_factor;
    data[half_n] = even[0] - tw_factor;

    // Main double butterfly calculations
    for (i = 1; i < half_n; i++)
    {
        // Find the twiddle factor using the lookup tables
        tw_factor = (lt_cos(lt_idx, i) * odd[i]) + (lt_sin(lt_idx, i) * odd[half_n - i]);
        data[i] = even[i] + tw_factor;
        data[i + half_n] = even[i] - tw_factor;
    }
}

int main(void)
{
    int i;
    float seq[4] = {3.2, 2.5, 7, 4.8};

    printf("Input: ");
    for (i = 0; i < 4; i++)
    {
        printf("%f ", seq[i]);
    }
    printf("\n");

    lt_init();

    fht(seq, 4);

    printf("Output: ");
    for (i = 0; i < 4; i++)
    {
        printf("%f ", seq[i]);
    }
    printf("\n");

    return 0;
}
