#include "openclmanager.hpp"

bool OpenCLManager::create_context() {
    try {
        context_ = cl::Context(CL_DEVICE_TYPE_DEFAULT);
        queue_ = cl::CommandQueue(context_);

        std::cout << "Using platform: " << m_platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;
        std::cout << "Using device: " << m_device.getInfo<CL_DEVICE_NAME>() << std::endl;
        std::cout << "Using version: " << m_device.getInfo<CL_DEVICE_VERSION>() << std::endl;

        return true;
    } catch(...) {
        std::cerr << "Failed to create an OpenCL context!" << std::endl << std::endl;
        return false;
    }
}