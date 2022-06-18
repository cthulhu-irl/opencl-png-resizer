#include <stdexcept>

#include "openclmanager.hpp"

void OpenCLManager::create_context() {
    try {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.empty())
            throw;
        platform_ = platforms[0];

        std::vector<cl::Device> devices;
        platform_.getDevices(CL_DEVICE_TYPE_GPU, &devices);

        if (devices.empty())
            platform_.getDevices(CL_DEVICE_TYPE_CPU, &devices);

        if (devices.empty())
            throw;

        device_ = devices[0];
        context_ = cl::Context({device_});
        queue_ = cl::CommandQueue(context_, device_);
    } catch(...) {
        throw std::runtime_error("Failed to create an OpenCL context!");
    }
}