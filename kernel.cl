
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "imgdiff.h"

_kernel void imgdiff(size_t N, _global size_t width, _global size_t height, _global double* diff_matrix, unsigned char* images) {
    size_t x=get_global_id[0];
    size_t y=get_global_id[0];
    double diff=0,0;
    int diff_r;
    int diff_g;
    int diff_b;
    if(y<height&&x<width){
        
        diff_r=((int)images[(i*width*height + y*width+x) * 3]) -
        ((int)images[(j*width*height + y*width+x) * 3]);
        
        diff_g = ((int)images[(i*width*height + y*width+x) * 3 + 1]) -
        ((int)images[(j*width*height + y*width+x) * 3 + 1]);
        
        diff_b = ((int)images[(i*width*height + y*width+x) * 3 + 2]) -
        ((int)images[(j*width*height + y*width+x) * 3 + 2]);
        
        // Difference @ pixel (x, y)
        diff += sqrt((double)diff_r * diff_r + (double)diff_g * diff_g +
                     (double)diff_b * diff_b);
    }
    for(i = 0; i < N; ++i) {
        for (j = i + 1; j < N; ++j) {
            double diff = calc_diff(width, height, images, i, j);
            diff_matrix[i * N + j] = diff_matrix[j * N + i] = diff;
        }
    }
}

