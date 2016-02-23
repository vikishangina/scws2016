#include <stdio.h>
#include <stdlib.h>
#include "libjpeg_wrapper.h"

void open_jpeg_image(const char* filename, unsigned char* image) {
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	dinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dinfo);

	FILE* fp = fopen(filename, "rb");
	if( fp == NULL )
	{
		printf("File open error. filename=%s\n", filename);
		return;
	}

	jpeg_stdio_src(&dinfo, fp);

	jpeg_read_header(&dinfo, TRUE);

	jpeg_start_decompress(&dinfo);

	int rowstride = dinfo.output_width * dinfo.output_components;
	int x;
	int y =0;
	JSAMPARRAY buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr)&dinfo, JPOOL_IMAGE, rowstride, 1);

	while(dinfo.output_scanline < dinfo.output_height) {
		jpeg_read_scanlines(&dinfo, buffer, 1);
		for( x = 0; x < dinfo.output_width; x++ )
		{
			JSAMPLE* p = buffer[0] + 3 * x;
			image[y*dinfo.output_width+x + 0] = p[0];
			image[y*dinfo.output_width+x + 1] = p[1];
			image[y*dinfo.output_width+x + 2] = p[2];
		}
		y++;
	}

	jpeg_finish_decompress(&dinfo);
	jpeg_destroy_decompress(&dinfo);
	fclose(fp);
}
