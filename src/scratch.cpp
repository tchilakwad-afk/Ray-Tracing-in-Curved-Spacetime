#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR "/"
#endif

std::string PROGRAM_FILE = PROJECT_SOURCE_DIR + std::string("/kernels/program.cl");
#define KERNEL_FUNCTION "add_vectors"


#define VEC_SIZE 512
#define WORK_SIZE 32
#define INPUT_RANGE 100.0f

float randf()
{
    return ((float)rand()) / RAND_MAX;
}

int main()
{

    cl_context context;
    cl_program program;
    cl_command_queue cmdq;

    cl_platform_id platform;
    cl_uint no_of_platforms;
    cl_device_id dev;
    cl_uint no_of_devs;
    int err;

    err = clGetPlatformIDs(1, &platform, &no_of_platforms);
    if (err < 0)
    {
        perror("Unable to get platform");
        exit(1);
    }

    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, &no_of_devs);
    if (err == CL_DEVICE_NOT_FOUND)
    {
        std::cout << "Unable to find gpu device. Trying for cpu\n";
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, &no_of_devs);
    }
    if (err < 0)
    {
        perror("Unable to get device");
        exit(1);
    }

    context = clCreateContext(NULL, 1, &dev, NULL, NULL, &err); // The two nulls are callback function and its data
    if (err < 0)
    {
        perror("Unable to create context\n");
    }

    char *program_buffer;
    char *program_log;
    FILE *program_handle;
    size_t program_size;
    size_t log_size;

    // int err;

    program_handle = fopen(PROGRAM_FILE.c_str(), "r");
    if (program_handle == NULL)
    {
        std::cout<<"ERROR::UNABLE TO OPEN FILE::"<<PROGRAM_FILE<<std::endl;
        exit(1);
    }

    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char *)malloc((program_size + 1) * sizeof(char));
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    program = clCreateProgramWithSource(context, 1, (const char **)&program_buffer, &program_size, &err);
    if (err < 0)
    {
        perror("Unable to create program with source\n");
        exit(1);
    }

    free(program_buffer);

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
    {
        perror("Unable to build program:\n\n");
        clGetProgramBuildInfo(program, 0, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        program_log = (char *)malloc((log_size + 1) * sizeof(char));
        program_log[program_size] = '\0';
        clGetProgramBuildInfo(program, 0, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL);
        free(program_log);
    }

    float a[VEC_SIZE];
    float b[VEC_SIZE];
    float c[VEC_SIZE];

    for (int i = 0; i < VEC_SIZE; i++)
    {
        a[i] = INPUT_RANGE * (randf() * 2.0f - 1.0f);
        a[i] = INPUT_RANGE * (randf() * 2.0f - 1.0f);
    }

    cl_mem a_mem, b_mem, c_mem;
    a_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, VEC_SIZE * sizeof(float), a, &err);
    b_mem = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, VEC_SIZE * sizeof(float), b, &err);
    c_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, VEC_SIZE * sizeof(float), c, &err);

    if (err < 0)
    {
        perror("Couldn't create a buffer\n");
        exit(1);
    };

    cmdq = clCreateCommandQueueWithProperties(context, dev, NULL, &err);
    if (err < 0)
    {
        perror("Unable to create command queue\n");
        exit(1);
    }

    cl_kernel kernel;
    kernel = clCreateKernel(program, KERNEL_FUNCTION, &err);
    if (err < 0)
    {
        perror("Unable to create kernel\n");
    }

    const uint vec_size = VEC_SIZE;

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &a_mem);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &b_mem);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &c_mem);
    err |= clSetKernelArg(kernel, 3, sizeof(uint), &vec_size);

    const size_t local_size = WORK_SIZE;
    const uint number_of_groups = (const uint)ceil((float)VEC_SIZE / (float)local_size);
    const size_t global_size = number_of_groups * local_size;

    err = clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err < 0)
    {
        perror("Couldn't enqueue the kernel");
        exit(1);
    }


    clFinish(cmdq);
    err = clEnqueueReadBuffer(cmdq, c_mem, CL_TRUE, 0, sizeof(float)*VEC_SIZE, c, 0, NULL, NULL);
    if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);
    }

    float c_ref[VEC_SIZE];
    for(int i=0; i<VEC_SIZE; i++) c_ref[i] = a[i] + b[i];

    bool same = true;

    for(int i=0; i<VEC_SIZE; i++){
        if(abs(c_ref[i] - c[i]) > 1.0e-6f){
            same = false;
            break;
        }
    }
    if(same){
        std::cout<<"Check passed. Sums are the same.\n";
    }else{
        std::cout<<"Check failed. Different values.\n";
    }


    clReleaseKernel(kernel);
    clReleaseMemObject(a_mem);
    clReleaseMemObject(b_mem);
    clReleaseMemObject(c_mem);
    clReleaseCommandQueue(cmdq);
    clReleaseProgram(program);
    clReleaseContext(context);



    return 0;
}
