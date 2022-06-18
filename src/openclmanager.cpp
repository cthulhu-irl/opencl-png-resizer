#include <stdexcept>

#include "openclmanager.hpp"

void OpenCLManager::create_context() {
    try {
        context_ = cl::Context(CL_DEVICE_TYPE_DEFAULT);
        queue_ = cl::CommandQueue(context_);
    } catch(...) {
        throw std::runtime_error("Failed to create an OpenCL context!");
    }
}