#pragma once

#include <string_view>
#include <stdexcept>

#include <CL/cl2.hpp>

class OpenCLManager {
    cl::Platform     paltform_;
    cl::Device       device_;
    cl::Context      context_;
    cl::CommandQueue queue_;
    cl::Program      program_;

  public:
    OpenCLManager() {
      if (!create_context())
        throw std::runtime_error("Couldn't setup OpenCL.");
    }

    bool create_kernel_program(const char* program_str) {
      program_ = cl::Program(context_, program_src.data(), true);
      // TODO check `program_`'s validity.
      return true;
    }

    cl::Program get_program() { return program_; }
    cl::Context get_context() { return context_; }
    cl::CommandQueue get_queue() { return queue_; }

  private:
    bool create_context();
};
