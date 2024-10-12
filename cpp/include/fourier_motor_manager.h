#pragma once

#include "rust/cxx.h"
#include "fourier_comm/src/cpp.rs.h"

struct MotorManagerSync;

class FourierMotorManager
{

public:
    FourierMotorManager(const std::vector<int32_t> &ids)
        : manager(make_motor_manager_v1(ids)) {}

    bool wait_for_first_messages(float timeout)
    {
        return manager->cxx_wait_for_first_messages(timeout);
    }

    bool enable(int32_t id)
    {
        return manager->cxx_enable(id);
    }

    bool disable(int32_t id)
    {
        return manager->cxx_disable(id);
    }

    bool set_position(int32_t id, float value)
    {
        return manager->cxx_set_position(id, value);
    }

    float get_position(int32_t id)
    {
        return manager->cxx_get_position(id);
    }

    float get_velocity(int32_t id)
    {
        return manager->cxx_get_velocity(id);
    }

    float set_velocity(int32_t id, float value)
    {
        return manager->cxx_set_velocity(id, value);
    }

    float get_current(int32_t id)
    {
        return manager->cxx_get_current(id);
    }

    float set_current(int32_t id, float value)
    {
        return manager->cxx_set_current(id, value);
    }

    float get_effort(int32_t id)
    {
        return manager->cxx_get_effort(id);
    }

    float set_effort(int32_t id, float value)
    {
        return manager->cxx_set_effort(id, value);
    }

    bool set_control_mode(int32_t id, std::string mode)
    {
        return manager->cxx_set_control_mode(id, mode);
    }

    std::string get_control_mode(int32_t id)
    {
        rust::String mode = manager->cxx_get_control_mode(id);
        return std::string(mode);
    }

    std::string get_motor_state(int32_t id)
    {
        auto state = manager->cxx_get_motor_state(id);
        return std::string(state);
    }

private:
    rust::Box<MotorManagerSync> manager;
};