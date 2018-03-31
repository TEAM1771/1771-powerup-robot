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
	elvtr.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
	elvtr.SetSensorPhase(0);
	elvtr.SetNeutralMode(NeutralMode::Brake);
	arm_mode = 0;
}

void Elevator::UpdatePID(){
		
        error = desired_pos - GetElvtrEnc();
        change = ELEVATOR_P*error/ELEVATOR_SCALE_PT;
		//if(GetElvtrEnc() < ELEVATOR_SWITCH_PT)
		//	if(change < 0) change = 0;
		//else
		//	if(change < 0) change = 0.05;
	
		//if(change < -.01 && GetElvtrEnc() > ELEVATOR_SWITCH_PT) change = .05;
		
		if(change < .075){
			change = .075;
			elvtr.SetNeutralMode(NeutralMode::Coast);
		}else{
			elvtr.SetNeutralMode(NeutralMode::Brake);
		}
		
		
		/* if(change < .01){
			change = 0;
			arm_mode = ~arm_mode;
			if(arm_mode)
				elvtr.SetNeutralMode(NeutralMode::Brake);
			else
				elvtr.SetNeutralMode(NeutralMode::Coast);
		}else{
			elvtr.SetNeutralMode(NeutralMode::Brake);
		} */
		
		
		//if(change < 0)
		//	change = 0;
		
		elvtr.Set(ControlMode::PercentOutput, change);
}

void Elevator::SetPosition(int pos){
    desired_pos = pos;
}

double Elevator::GetElvtrEnc(){
	return elvtr.GetSelectedSensorPosition(0);
}
                                                                                                                                            // Sarah wuz hier //
void Elevator::Set(double rate){
	elvtr.Set(ControlMode::PercentOutput, rate);
}

void Elevator::SetNeutralMode(NeutralMode nm){
	elvtr.SetNeutralMode(nm);
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
