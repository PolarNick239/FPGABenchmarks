#include "cl/Engine.h"

#include <cassert>

static const bool VERBOSE_COMPILATION_LOG = false;

using namespace cl;

void CL_CALLBACK context_error_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data) {
    cl::Engine* engine = (cl::Engine*) user_data;
    std::cerr << "Error occurred in context (platform " << engine->device->platform->name << ", device " << engine->device->name << "):" << std::endl;
    std::cerr << "  Error info: " << errinfo << std::endl;
}

Engine::~Engine() {
    if (initialized) {
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
    }
}

bool Engine::init() {
    int i;
    cl_int error_code;

    i = 0;
    cl_context_properties properties[3];
    properties[i++] = CL_CONTEXT_PLATFORM;
    properties[i++] = (cl_context_properties) device->platform->platform_id;
    properties[i] = 0;

    cl_context new_context = clCreateContext(properties, 1, &device->device_id, context_error_notify, this, &error_code);
    CHECKED_FALSE(error_code);

    cl_command_queue_properties queue_properties = 0;
    cl_command_queue new_queue = clCreateCommandQueue(new_context, device->device_id, queue_properties, &error_code);
    CHECKED_FALSE(error_code);

    context = new_context;
    queue = new_queue;
    initialized = true;
    return true;
}

bool Engine::ready() const {
    return initialized;
}

bool Engine::compile(const char* source, size_t length, cl_program& program, const char* options) const {
    assert(ready());
    cl_int error_code;

    cl_program new_program = clCreateProgramWithSource(context, 1, &source, &length, &error_code);
    CHECKED_FALSE(error_code);
    error_code = clBuildProgram(new_program, 1, &device->device_id, options, NULL, NULL);

    if (error_code == CL_BUILD_PROGRAM_FAILURE || VERBOSE_COMPILATION_LOG) {
        size_t log_size;
        clGetProgramBuildInfo(new_program, device->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        std::string log;
        log.resize(log_size, ' ');
        clGetProgramBuildInfo(new_program, device->device_id, CL_PROGRAM_BUILD_LOG, log_size, (void *) log.data(), &log_size);
        std::cout << "Log:" << std::endl << log << std::endl;
    }

    if (error_code == CL_BUILD_PROGRAM_FAILURE) {
        std::cerr << "Program building failed!\n" << std::endl;
    } else {
        CHECKED_FALSE(error_code);
    }

    program = new_program;
    return true;
}

Kernel_ptr Engine::createKernel(cl_program program, const char* kernel_name) const {
    cl_int error_code;
    cl_kernel kernel = clCreateKernel(program, kernel_name, &error_code);
    CHECKED_NULL(error_code);

    return std::make_shared<Kernel>(program, kernel);
}

Kernel_ptr Engine::compileKernel(const char *source, size_t length, const char *kernel_name,
                                 const char *options) const {
    cl_program program;
    if (!compile(source, length, program, options)) {
        return nullptr;
    }
    return createKernel(program, kernel_name);
}

cl_mem Engine::createBuffer(size_t size, cl_mem_flags flags) const {
    cl_int error_code;
    cl_mem buffer = clCreateBuffer(context, flags, size, NULL, &error_code);
    CHECKED(error_code);
    return buffer;
}

void Engine::deallocateBuffer(cl_mem buffer) const {
    CHECKED(clReleaseMemObject(buffer));
}

void Engine::enqueueKernel(Kernel_ptr kernel, unsigned int workDim, const size_t* globalWorkSize, const size_t* localWorkSize) const {
    CHECKED(clEnqueueNDRangeKernel(queue, kernel->kernel(), workDim, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL));
}

void Engine::readBuffer(cl_mem buffer, size_t size, void* ptr) const {
    CHECKED(clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size, ptr, 0, NULL, NULL));
}

Engine_ptr cl::createGPUEngine() {
    Device_ptr best_device;
    auto platforms = getPlatforms();
    for (auto platform : platforms) {
        auto devices = getDevices(platform, GPU_DEVICE);
        for (auto device : devices) {
            if (!best_device || device->max_compute_units > best_device->max_compute_units) {
                best_device = device;
            }
        }
    }
    if (best_device) {
        return Engine_ptr(new Engine(best_device));
    } else {
        return Engine_ptr();
    }
}
