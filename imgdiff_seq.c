
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imgdiff.h"


double calc_diff(size_t width, size_t height, unsigned char* images, size_t i, size_t j) {
	size_t x, y;
	double diff = 0.0;

	for(y = 0; y < height; y++) 	{
		for(x = 0; x < width; x++) {
			int diff_r = ((int)images[(i*width*height + y*width+x) * 3]) -
                   ((int)images[(j*width*height + y*width+x) * 3]);
      int diff_g = ((int)images[(i*width*height + y*width+x) * 3 + 1]) -
                   ((int)images[(j*width*height + y*width+x) * 3 + 1]);
      int diff_b = ((int)images[(i*width*height + y*width+x) * 3 + 2]) -
                   ((int)images[(j*width*height + y*width+x) * 3 + 2]);

      // Difference @ pixel (x, y)
      diff += sqrt((double)diff_r * diff_r + (double)diff_g * diff_g + 
                   (double)diff_b * diff_b);
		}
	}

	return diff;
}

void imgdiff(size_t N, size_t width, size_t height, double* diff_matrix, unsigned char* images) {
	size_t i, j;

	for(i = 0; i < N; ++i) {
		for (j = i + 1; j < N; ++j) {
			double diff = calc_diff(width, height, images, i, j);
			diff_matrix[i * N + j] = diff_matrix[j * N + i] = diff;
		}
	}
}
