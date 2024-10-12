from fourier_comm_rs import FourierMotorManager

if __name__ == "__main__":
    # Create a new instance of the FourierMotorManager class
    ids = [13, 14, 15]
    manager = FourierMotorManager(ids)

    # Connect to the Fourier motors
    manager.wait_for_first_messages(1.0)

    for id in ids:
        manager.enable(id)
        manager.set_control_mode(id, "position")
        
    for id in ids:
        pos = manager.get_position(id)
        print(f"Motor {id} is at position: {pos}")
        
        mode = manager.get_control_mode(id)
        print(f"Motor {id} is in control mode: {mode}")
        
        