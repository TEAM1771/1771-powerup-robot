#include <DriveTrain.h>
#include <Definitions.h>

DriveTrain::DriveTrain(int lm1, int lm2, int rm1, int rm2, int le1, int le2, int re1, int re2, int shift_port, int pto_en_port, int pto_dis_port) :
    ltrm(lm1, lm2, le1, le2),
    rtrm(rm1, rm2, re1, re2),
    shifter(shift_port),
    pto_en(pto_en_port),
	pto_dis(pto_dis_port){
    isClimbing = false;
}

void DriveTrain::Tank(double lrate, double rrate){
	// DEBUG
	SmartDashboard::PutNumber("LM1 Current", ltrm.GetM1Current());
	SmartDashboard::PutNumber("LM2 Current", ltrm.GetM2Current());
	SmartDashboard::PutNumber("RM1 Current", rtrm.GetM1Current());
	SmartDashboard::PutNumber("RM2 Current", rtrm.GetM2Current());
	
    if(isClimbing){
        double avg = (lrate+rrate)/2.0;
        ltrm.Set(avg);
        rtrm.Set(-avg);
    }else{
        ltrm.Set(lrate);
        rtrm.Set(-rrate);
    }
}



void DriveTrain::Shift(bool toShiftOrNotToShift){
    shifter.Set(toShiftOrNotToShift);
}

double DriveTrain::GetSpeed(){
    return fabs(ltrm.GetSpeed() - rtrm.GetSpeed()) / 2;
}

void DriveTrain::EnableClimb(){
	pto_dis.Set(0);
    pto_en.Set((isClimbing = 1));
}

void DriveTrain::DisableClimb(){
	isClimbing = 0;
	pto_en.Set(0);
    pto_dis.Set(1);
}

void DriveTrain::AutoShift(){
    if(!isClimbing){
        if(GetSpeed() > SHIFT_UP_SPEED)
            Shift(1);
        else if(GetSpeed() < SHIFT_DOWN_SPEED)
            Shift(0);
    }else{
        /*if(GetSpeed() > CL_SHIFT_UP_SPEED)
            Shift(1);
        else if(GetSpeed() <CL_SHIFT_UP_SPEED)
            Shift(0);*/
    }
        
}

bool DriveTrain::IsClimbing(){
    return isClimbing;
}

double DriveTrain::GetAvgDistance(){
    return (ltrm.GetDistance() + rtrm.GetDistance())/2.0;
}

double DriveTrain::GetLeftEnc(){
	return ltrm.GetRawEnc();
}

double DriveTrain::GetRightEnc(){
	return rtrm.GetRawEnc();
}

double DriveTrain::GetAvgRaw(){
    return (ltrm.GetRawEnc() + rtrm.GetRawEnc())/2.0;
}

void DriveTrain::ResetEnc(){
	ltrm.ResetEnc();
	rtrm.ResetEnc();
}

void DriveTrain::SetNeutralMode(NeutralMode nm){
	ltrm.SetNeutralMode(nm);
	rtrm.SetNeutralMode(nm);
}