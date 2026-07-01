#pragma once

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

using namespace std;