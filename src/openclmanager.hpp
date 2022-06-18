#pragma once

#include <string_view>
#include <stdexcept>

#define CL_HPP_TARGET_OPENCL_VERSION 210
#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/cl2.hpp>

// edited from: https://github.com/vencabkk/opencl-resizer/

class OpenCLManager {
    cl::Context      context_;
    cl::CommandQueue queue_;
    cl::Program      program_;

  public:
    OpenCLManager()
        : context_(CL_DEVICE_TYPE_DEFAULT)
        , queue_(context_)
    {}

    OpenCLManager(const char* program_src)
        : context_(CL_DEVICE_TYPE_DEFAULT)
        , queue_(context_)
        , program_(context_, program_src, true)
    {}

    // TODO check `program_`'s validity.
    bool create_kernel_program(const char* program_src) {
      program_ = cl::Program(context_, program_src);
      program_.build();
      return true;
    }

    cl::Program get_program() { return program_; }
    cl::Context get_context() { return context_; }
    cl::CommandQueue get_queue() { return queue_; }
};
