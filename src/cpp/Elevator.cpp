#include <Elevator.h>

Elevator::Elevator(int elv_port, int elv_ch_a, int elv_ch_b, int arm_port, int arm_pivot_port, int arm_pivot_pot_port, int flipper_port, int rin_port, int lin_port) :
intake(arm_port, arm_pivot_port, arm_pivot_pot_port, flipper_port, rin_port, lin_port),
elvtr(elv_port),
elvtr_enc(elv_ch_a, elv_ch_b){
    elvtr_enc.Reset();
    i = 0;
    desired_pos = 0;
}

void Elevator::UpdatePI(){
    i += elvtr_enc.Get()*.02;
    error = desired_pos - elvtr_enc.Get();
    change = ELEVATOR_P*error + ELEVATOR_I*i;
}

void Elevator::SetPosition(int pos){
    desired_pos = pos;
}
