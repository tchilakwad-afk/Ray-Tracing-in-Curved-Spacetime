#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cl;

vector<Platform> all_platforms;
Platform::get(&all_platforms);
if(all_platforms.empty()){
    cerr << "No OpenCl platforms found. \n";
    exit(1);
}

Platform default_platform = all_platforms[0];
cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

vector<cl::Device> all_devices;
default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
if(all_devices.empty()){
    cerr << "No devices found. \n";
    exit(1);
}

Device default_device = all_devices[0];
cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";