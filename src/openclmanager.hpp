#pragma once

#include <string_view>
#include <stdexcept>

#define CL_HPP_TARGET_OPENCL_VERSION 210
#include <CL/cl2.hpp>

// edited from: https://github.com/vencabkk/opencl-resizer/

class OpenCLManager {
    cl::Platform     paltform_;
    cl::Device       device_;
    cl::Context      context_;
    cl::CommandQueue queue_;
    cl::Program      program_;

  public:
    OpenCLManager() { create_context(); }

    bool create_kernel_program(const char* program_src) {
      program_ = cl::Program(context_, program_src, true);
      // TODO check `program_`'s validity.
      return true;
    }

    cl::Program get_program() { return program_; }
    cl::Context get_context() { return context_; }
    cl::CommandQueue get_queue() { return queue_; }

  private:
    void create_context();
};