#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"

class Handle {
   public:
    ~Handle(){};

    void handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const exlcm::example_t* msg) {
        int i;
        std::cout << "Receive message on channel \"%s\":\n" << chan << std::endl;
        std::cout << "  timestamp   = " << msg->timestamp << std::endl;
        std::cout << "  position    = [" << msg->position[0] << ", " << msg->position[1] << ", " << msg->position[2] << "]" << std::endl;
        std::cout << "  orientation = [" << msg->orientation[0] << ", " << msg->orientation[1] << ", " << msg->orientation[2] << ", "
                  << msg->orientation[3] << "]" << std::endl;
        std::cout << "  ranges      = [" << msg->ranges[0];
        for (i = 1; i < msg->num_ranges; i++) {
            std::cout << ", " << msg->ranges[i];
        }
        std::cout << "]" << std::endl;
        std::cout << "  name        = \"" << msg->name << "\"" << std::endl;
        std::cout << "  enabled     = " << (msg->enabled ? "true" : "false") << std::endl;
    }
};

int main(int argc, char** argv) {
    lcm::LCM lcm;
    if (!lcm.good()) {
        return -1;
    }

    Handle handle_object;

    lcm.subscribe("EXAMPLE", &Handle::handleMessage, &handle_object);

    while (0 == lcm.handle()) {
    };

    return 0;
}