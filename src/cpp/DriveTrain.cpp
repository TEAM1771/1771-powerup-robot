#include <DriveTrain.h>
#include <Definitions.h>

DriveTrain::DriveTrain(int lm1, int lm2, int rm1, int rm2, int le1, int le2, int re1, int re2, int shift_port, int pto_port) :
    ltrm(lm1, lm2, le1, le2),
    rtrm(rm1, rm2, re1, re2),
    shifter(shift_port),
    pto(pto_port) {
    isClimbing = false;
}

void DriveTrain::Tank(double lrate, double rrate){
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
    pto.Set((isClimbing = 1));
}

void DriveTrain::DisableClimb(){
    pto.Set((isClimbing = 0));
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

double DriveTrain::GetAvgRaw(){
    return (ltrm.GetRawEnc() + rtrm.GetRawEnc())/2.0;
}
