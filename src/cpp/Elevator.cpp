#include <Elevator.h>

Elevator::Elevator(int elv_port, int elv_ch_a, int elv_ch_b, int arm_pivot_port, int arm_pivot_pot_port, int flipper_port, int rin_port, int lin_port) :
intake(arm_pivot_port, arm_pivot_pot_port, flipper_port, rin_port, lin_port),
elvtr(elv_port),
elvtr_enc(elv_ch_a, elv_ch_b){
    elvtr_enc.Reset();
    i = 0;
    desired_pos = 0;
    derivative = 0, last_err = 0;
    reaching_pos = 0;
}

void Elevator::UpdatePID(){
    if(desired_pos == -1){
        i += elvtr_enc.Get()*.02;
        last_err = error;
        error = desired_pos - elvtr_enc.Get();
        if(error > 1)
            reaching_pos = 1;
        change = ELEVATOR_P*error + ELEVATOR_I*i + ELEVATOR_D*((error - last_err)/.02);
        
        elvtr.Set(ControlMode::PercentOutput, change);
    }
}

void Elevator::SetPosition(int pos){
    desired_pos = pos;
}
                                                                                                                                            // Sarah wuz hier //
void Elevator::Set(double rate){
    if(reaching_pos){
        if(elvtr_enc.Get() < ELEVATOR_LOW_PT){
            SetPosition((ELEVATOR_HIGH_PT+ELEVATOR_LOW_PT)/2);  // Temporary midpoint value -> change during testing
        }else if(elvtr_enc.Get() > ELEVATOR_HIGH_PT){
            SetPosition((ELEVATOR_LOW_PT+ELEVATOR_HIGH_PT)/2);  // Temporary midpoint value -> change during testing
        }else{
            SetPosition(-1);
            elvtr.Set(ControlMode::PercentOutput, rate);
        }
    }
}

void Elevator::SetForJoy(double rate){
    if(elvtr_enc.Get() > ELEVATOR_HIGH_PT){
        if(rate < 0)
            elvtr.Set(ControlMode::PercentOutput, rate);
    }else if(elvtr_enc.Get() < ELEVATOR_LOW_PT){
        if(rate > 0)
            elvtr.Set(ControlMode::PercentOutput, rate);
    }
}
