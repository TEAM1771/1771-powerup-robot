#include <Intake.h>

Intake::Intake(int arm_pivot_port, int flipper_port, int rin_port, int lin_port, int enc_a, int enc_b) :
arm_pivot(arm_pivot_port),
flipper(flipper_port),
lin(lin_port), rin(rin_port){
	arm_pivot.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
	arm_pivot.SetSensorPhase(0);
}

void Intake::ZeroEnc(){
	arm_pivot.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
	arm_pivot.SetSensorPhase(0);
}

void Intake::SetIntakeWheels(double rate){
    rin.Set(ControlMode::PercentOutput, rate);
    lin.Set(ControlMode::PercentOutput, -rate);
}

void Intake::SetFlipper(bool flip){
    flipper.Set(flip);
}

int Intake::SetPivotArm(double pos){
	
    SmartDashboard::PutString("EncVal:", std::to_string(arm_pivot.GetSelectedSensorPosition(0)));
	double speed = fabs((pos-GetPivotEnc())/INTAKE_IN_PT);
    if(GetPivotEnc() > pos-25){
    	arm_pivot.Set(ControlMode::PercentOutput, -speed);
	return 0;
    }else if(arm_pivot.GetSelectedSensorPosition(0) < pos+25){
        arm_pivot.Set(ControlMode::PercentOutput, speed);
        return 0;
    }else{
        return 1;
    }
}

void Intake::SetPivotSpeed(double speed){
	if(!(GetPivotEnc()-20 < INTAKE_UP_PT) && !(GetPivotEnc()+20 > INTAKE_IN_PT))
	arm_pivot.Set(ControlMode::PercentOutput, speed);
}

double Intake::GetPivotEnc(){
	return arm_pivot.GetSelectedSensorPosition(0);
}
