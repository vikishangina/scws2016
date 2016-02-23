
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "imgdiff.h"


#define CHECK_ERROR(err) \
if (err != CL_SUCCESS) { \
printf("[%s:%d] OpenCL error %d\n", __FILE__, __LINE__, err); \
exit(EXIT_FAILURE); \
}
    
    char *get_source_code(const char *file_name, size_t *len) {
        char *source_code;
        size_t length;
        FILE *file = fopen(file_name, "r");
        if (file == NULL) {
            printf("[%s:%d] Failed to open %s\n", __FILE__, __LINE__, file_name);
            exit(EXIT_FAILURE);
        }
        
        fseek(file, 0, SEEK_END);
        length = (size_t)ftell(file);
        rewind(file);
        
        source_code = (char *)malloc(length + 1);
        fread(source_code, length, 1, file);
        source_code[length] = '\0';
        
        fclose(file);
        
        *len = length;
        return source_code;
    }
    
    void imgdiff_opencl(size_t N,  size_t width,  size_t height,
                        double* diff_matrix, unsigned char* images) {
        cl_platform_id platform;
        cl_device_id device;
        cl_context context;
        cl_command_queue queue;
        cl_program program;
        char kernel_source;
        size_t kernel_source_size;
        cl_kernel kernel;
        cl_int err;
        err = clGetPlatformIDs(1,&platform,NULL);
        CHECK_ERROR(err);
        err = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,NULL);
        CHECK_ERROR(err);
        context = clCreateContext(NULL,1,&device,"",NULL,NULL);
        CHECK_ERROR(err);
        queue = clCreateCommandQueue(context,device,0,&err);
        CHECK_ERROR(err);
        kernel_source = get_source_code("kernel.cl",&kernel_source_size);
        program=clCreateProgramWithSource(context,1,(const char**)&kernel_source,&kernel_source_size,&err);
        CHECK_ERROR(err);
        err=clBuildProgram(program,1,&device,"",NULL,NULL);
        if (err==CL_Build_Program_Failure){
            size_t log_size;
            char *log;
            err=clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,
                                      0,NULL,&log_size);
            CHECK_ERROR(err);
            log=(char*)malloc(log_size+1);
            err=clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,log_size,log,NULL);
            
            CHECK_ERROR(err);
            
            log[log_size]='\0';
            printf("Compiler error:"\n%s\n,log);
            free(log);
            
        }
        CHECK_ERROR(err);
        
        kernel = clCreateKernel(program,"imgdiff",&err);
        
        CHECK_ERROR(err);
        cl_mem bufPic,bufDiff;
        
        bufPic = clCreateBuffer(context;CL_MEM_READ_ONLY;sizeof(float)*width*height,NULL,err);
        bufDiff = clCreateBuffer(context;CL_MEM_READ_Write;sizeof(float)*N*N,NULL,err);
        
        CHECK_ERROR(err);
        

}

