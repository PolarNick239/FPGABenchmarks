#include <iostream>
#include <numeric>

#include "cl/Platform.h"
#include "cl/Device.h"


int main() {
    auto platforms = cl::getPlatforms();
    std::cout << "Platforms number: " << platforms.size() << std::endl;

    int i = 0;
    for (auto platform : platforms) {
        std::cout << "  Platform " << i + 1 << "/" << platforms.size() << ":" << std::endl;
        if (platform) {
            std::cout << "    Name:       " << platform->name << std::endl;
            std::cout << "    Vendor:     " << platform->vendor << std::endl;
            std::cout << "    Version:    OpenCL " << platform->version.majorVersion << "." << platform->version.minorVersion << " (" << platform->version.info << ")" << std::endl;
            std::cout << "    Profile:    " << (platform->profile == cl::FULL_PROFILE ? "FULL_PROFILE" : platform->profile == cl::EMBEDDED_PROFILE ? "EMBEDDED_PROFILE" : "UNKNOWN_PROFILE") << std::endl;
            std::cout << "    Extensions:";
            for (auto extension : platform->extensions) {
                std::cout << " " << extension;
            }
            std::cout << std::endl;

            auto devices = cl::getDevices(platform);
            for (size_t j = 0; j < devices.size(); j++) {
                auto device = devices[j];
                std::cout << "      Device " << j + 1 << "/" << devices.size() << ":" << std::endl;
                if (device) {
                    std::cout << "        Name:              " << device->name << std::endl;
                    std::cout << "        Vendor:            " << device->vendor << std::endl;
                    std::cout << "        Vendor ID:         " << device->vendor_id << std::endl;
                    std::cout << "        Driver:            " << device->driver_version.majorVersion << "." << device->driver_version.minorVersion << std::endl;
                    std::cout << "        Version:           OpenCL " << device->device_version.majorVersion << "." << device->device_version.minorVersion << " (" << device->device_version.info << ")" << std::endl;
                    std::cout << "        Max compute units: " << device->max_compute_units << std::endl;
                    std::cout << "        Max workgroup:     " << device->max_work_group_size << std::endl;
                    std::cout << "        Memory size:       " << (device->global_mem_size >> 20) << "\tMB" << std::endl;
                    std::cout << "        Max buffer:        " << (device->max_mem_alloc_size >> 20) << "\tMB" << std::endl;
                    std::cout << "        Local memory:      " << (device->local_mem_size >> 10) << "\tKB" << std::endl;
                    std::cout << "        Max frequency:     " << device->max_clock_freq << "\tMHz" << std::endl;
                    std::cout << "        Extensions:       ";
                    for (auto extension : device->extensions) {
                        std::cout << " " << extension;
                    }
                    std::cout << std::endl;
                } else {
                    std::cout << "        Failed!" << std::endl;
                }
            }
        } else {
            std::cout << "    Failed!" << std::endl;
        }
        i++;
    }
    return 0;
}
