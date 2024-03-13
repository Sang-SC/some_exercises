#include <lcm/lcm-cpp.hpp>
#include "latency_lcm/test_latency.hpp"
#include "utils/logger_config.h"

#include <chrono>

class Handle {
   public:
    ~Handle(){};

    void handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const latency_lcm::test_latency* msg) {
        auto time_current = std::chrono::high_resolution_clock::now();
        auto time_current_sec = std::chrono::duration_cast<std::chrono::seconds>(time_current.time_since_epoch()).count();
        auto time_current_nanosec =
            std::chrono::duration_cast<std::chrono::nanoseconds>(time_current.time_since_epoch() % std::chrono::seconds(1)).count();

        double time_delay_us = ((time_current_sec - msg->sec) * 1e9 + (time_current_nanosec - msg->nanosec)) / 1e3;

        // logger->info("Receive message on channel: {}", chan);
        // logger->info("  sec     = {}", msg->sec);
        // logger->info("  nanosec = {}", msg->nanosec);
        logger->info("LCM: Subscribe successfully! Delay   = {} us", time_delay_us);
    }
};

int main(int argc, char** argv) {
    lcm::LCM lcm;
    if (!lcm.good()) {
        return -1;
    }

    Handle handle_object;

    lcm.subscribe("2_test_latency", &Handle::handleMessage, &handle_object);

    while (0 == lcm.handle()) {
    };

    return 0;
}