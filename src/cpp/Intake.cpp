#include <Intake.h>

Intake::Intake(int arm_pivot_port, int flipper_port, int rin_port, int lin_port, int enc_a, int enc_b) :
arm_pivot(arm_pivot_port),
flipper(flipper_port),
lin(lin_port), rin(rin_port){
	arm_pivot.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
	arm_pivot.SetSensorPhase(0);
	arm_pivot.SetNeutralMode(NeutralMode::Brake);
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
	double speed = INTAKE_P*fabs((pos-GetPivotEnc())/fabs(INTAKE_IN_PT));
	if(fabs(pos) - fabs(GetPivotEnc()) > 10){
		SetPivotSpeed(-speed);
	}else if(fabs(pos) - fabs(GetPivotEnc()) < -10){
		SetPivotSpeed(speed);
	}
}

void Intake::SetPivotSpeed(double speed){
	//if(!(GetPivotEnc()-20 < INTAKE_UP_PT) && !(GetPivotEnc()+20 > INTAKE_IN_PT))
	arm_pivot.Set(ControlMode::PercentOutput, speed);
}

double Intake::GetPivotEnc(){
	return arm_pivot.GetSelectedSensorPosition(0);
}
