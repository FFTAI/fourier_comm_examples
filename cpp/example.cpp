#include "fourier_motor_manager.h"
#include <iostream>
#include <vector>
#include <unistd.h>

int main()
{
    std::vector<int32_t> ids;
    ids.push_back(13);
    ids.push_back(14);
    ids.push_back(15);
    FourierMotorManager manager(std::move(ids));
    manager.wait_for_first_messages(1.0);
    for (auto id : ids)
    {
        manager.enable(id);
        manager.set_control_mode(id, "position");
    }
    sleep(1);
    for (auto id : ids)
    {
        auto pos = manager.get_position(id);
        std::cout << "Motor " << id << " posittion: " << pos << std::endl;

        auto mode = manager.get_control_mode(id);
        std::cout << "Motor " << id << " Control mode: " << mode << std::endl;

        auto age = manager.get_motor_state(id);
        std::cout << "Motor " << id << " state: " << age << std::endl;
    }

    for (auto id : ids)
    {
        manager.disable(id);
    }
}