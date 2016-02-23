
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libjpeg_wrapper.h"
#include "imgdiff.h"


int timespec_subtract(struct timespec*, struct timespec*, struct timespec*);
unsigned char* load_image_list(const char*, size_t*, size_t*, size_t*);

int main(int argc, char** argv) {
	char* image_list_filename = argv[1];
	unsigned char* images;
	size_t num_images, width, height;
	double* diff_matrix;
	FILE *io_file;
	struct timespec start, end, spent;

	// Check parameters
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <image_list> <diff_matrix>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	images = load_image_list(image_list_filename, &num_images, &width, &height);
	diff_matrix = (double*)malloc(num_images * num_images * sizeof(double));

	clock_gettime(CLOCK_MONOTONIC, &start);
	imgdiff(num_images, width, height, diff_matrix, images);
	clock_gettime(CLOCK_MONOTONIC, &end);

	timespec_subtract(&spent, &end, &start);
	printf("Elapsed time: %ld.%03ld sec\n", spent.tv_sec, spent.tv_nsec/1000/1000);

	// Write the result
	io_file = fopen(argv[2], "wb");
	fwrite(&num_images, sizeof(int), 1, io_file);
	fwrite(diff_matrix, sizeof(double), num_images * num_images, io_file);
	fclose(io_file);

	free(images);
	free(diff_matrix);

	return 0;
}

unsigned char* load_image_list(const char* list_filename, size_t* num_images, size_t* width, size_t* height) {
	size_t i, j;
	char filename[100];

	FILE* fp = fopen(list_filename, "r");
	if( fp == NULL ) {
		printf("Failed to open image list file. filename=%s\n", list_filename);
		exit(0);
	}

	fscanf(fp, "%ld\n", num_images);
	fscanf(fp, "%ld %ld\n", width, height);

	size_t image_size = (*width) * (*height) * 3 * sizeof(unsigned char);
	unsigned char* images = (unsigned char*)malloc(image_size * (*num_images));

	for(i = 0, j = *num_images; i < j; ++i) {
		memset(filename, 0, 100);
		fscanf(fp, "%s\n", filename);
		open_jpeg_image(filename, &images[image_size * i]);
	}

	return images;
}

int timespec_subtract (struct timespec* result, struct timespec *x, struct timespec *y) {
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_nsec < y->tv_nsec) {
		int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
		y->tv_nsec -= 1000000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_nsec - y->tv_nsec > 1000000000) {
		int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
		y->tv_nsec += 1000000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
		 tv_nsec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_nsec = x->tv_nsec - y->tv_nsec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}
