#include <Elevator.h>

Elevator::Elevator(int elv_port, int elv_ch_a, int elv_ch_b, int arm_port, int arm_pivot_port, int arm_pivot_pot_port, int flipper_port, int rin_port, int lin_port) :
intake(arm_port, arm_pivot_port, arm_pivot_pot_port, flipper_port, rin_port, lin_port),
elvtr(elv_port),
elvtr_enc(elv_ch_a, elv_ch_b){
    elvtr_enc.Reset();
    i = 0;
    desired_pos = 0;
    derivative = 0, last_err = 0;
}

void Elevator::UpdatePID(){
    i += elvtr_enc.Get()*.02;
    last_err = error;
    error = desired_pos - elvtr_enc.Get();
    change = ELEVATOR_P*error + ELEVATOR_I*i + ELEVATOR_D*((error - last_err)/.02);
    
    elvtr.Set(ControlMode::PercentOutput, change);
}

void Elevator::SetPosition(int pos){
    desired_pos = pos;
}
