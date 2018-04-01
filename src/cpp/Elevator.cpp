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
	last_enc = 0;
	
}

void Elevator::UpdatePID(){
		static double max_change = -10;
		static double min_change = 10;
		SmartDashboard::PutNumber("max_change:", max_change);
		SmartDashboard::PutNumber("min_change:", min_change);

		
        error = desired_pos - GetElvtrEnc();
        change = ELEVATOR_P*error/ELEVATOR_SCALE_PT;
		
		/**   CHANGE @ .1 HOLDS ELEVATOR IN POSITION    **/
		/* if(change < 0){
			elvtr.SetNeutralMode(NeutralMode::Brake);
			change = 0;
		} */
		
		// TEST BELOW
		 /* if(change < 0){
			change = 0;
		 }  */
		
		/*if(arm_mode != prev_arm_mode){
			if(arm_mode == 0)
				elvtr.SetNeutralMode(NeutralMode::Brake);
			else if(arm_mode == 1)
				elvtr.SetNeutralMode(NeutralMode::Coast);
			prev_arm_mode = arm_mode;
		} */
		
		if(change < 0){
			change = 0;
			SmartDashboard::PutString("enc-last diff:", std::to_string(fabs(GetElvtrEnc() - last_enc)));
			if(GetElvtrEnc() < ELEVATOR_SWITCH_PT/3){
				arm_mode = 0;
			}else if(arm_mode && fabs(GetElvtrEnc() - last_enc) >= ELEVATOR_GAP_COAST){
				arm_mode = !arm_mode;
				last_enc = GetElvtrEnc();
			}else if(!arm_mode && fabs(GetElvtrEnc() - last_enc) >= ELEVATOR_GAP_BRAKE){
				arm_mode = !arm_mode;
				last_enc = GetElvtrEnc();
			}
			
			if(arm_mode){
				elvtr.SetNeutralMode(NeutralMode::Coast);
			}else{
				elvtr.SetNeutralMode(NeutralMode::Brake);
			}
		}else{
			elvtr.SetNeutralMode(NeutralMode::Brake);
		}
		 
		if(change > max_change) max_change = change;
		if(change < min_change) min_change = change;
		
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
