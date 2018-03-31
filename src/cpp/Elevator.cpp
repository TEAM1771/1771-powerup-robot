#include <Elevator.h>

Elevator::Elevator(int elv_port, int elv_ch_a, int elv_ch_b, int arm_pivot_port, int enc_a, int enc_b, int flipper_port, int rin_port, int lin_port) :
intake(arm_pivot_port, flipper_port, rin_port, lin_port, enc_a, enc_b),
elvtr(elv_port),
elvtr_enc(elv_ch_a, elv_ch_b){
    elvtr_enc.Reset();
    i = 0;
    desired_pos = 0;
    derivative = 0, last_err = 0;
    reaching_pos = 0;
}

void Elevator::UpdatePID(){
    if(/*desired_pos == -1*/ 1){
        i += elvtr_enc.Get()*RIO_CYCLE;
        last_err = error;
        error = desired_pos - elvtr_enc.Get();
        if(error > 1)
            reaching_pos = 1;
        //change = ELEVATOR_P*error + ELEVATOR_I*i + ELEVATOR_D*((error - last_err)/RIO_CYCLE);
        change = error/ELEVATOR_SCALE_PT;
        elvtr.Set(ControlMode::PercentOutput, change);
    }
}

void Elevator::SetPosition(int pos){
    desired_pos = pos;
}
                                                                                                                                            // Sarah wuz hier //
void Elevator::Set(double rate){
    if(reaching_pos){
        if(elvtr_enc.Get() < ELEVATOR_DOWN_PT){
            SetPosition((ELEVATOR_SCALE_PT+ELEVATOR_DOWN_PT)/2);  // Temporary midpoint value -> change during testing
        }else if(elvtr_enc.Get() > ELEVATOR_SCALE_PT){
            SetPosition((ELEVATOR_DOWN_PT+ELEVATOR_SCALE_PT)/2);  // Temporary midpoint value -> change during testing
        }else{
            SetPosition(-1);
            elvtr.Set(ControlMode::PercentOutput, rate);
        }
    }
}

void Elevator::SetForJoy(double rate){
    SmartDashboard::PutString("Current Elevator Encoder Val:", std::to_string(elvtr_enc.Get()));
    if(elvtr_enc.Get() > ELEVATOR_SCALE_PT){
        if(rate < 0)
            elvtr.Set(ControlMode::PercentOutput, rate);
    }else if(elvtr_enc.Get() < ELEVATOR_DOWN_PT){
        if(rate > 0)
            elvtr.Set(ControlMode::PercentOutput, rate);
    }
}
