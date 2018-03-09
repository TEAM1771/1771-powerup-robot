#include <Intake.h>

Intake::Intake(int arm_pivot_port, int flipper_port, int rin_port, int lin_port, int enc_a, int enc_b) :
arm_pivot(arm_pivot_port),
pivot_enc(enc_a, enc_b),
flipper(flipper_port),
lin(lin_port), rin(rin_port){
}

void Intake::SetIntakeWheels(double rate){
    rin.Set(ControlMode::PercentOutput, rate);
    lin.Set(ControlMode::PercentOutput, -rate);
}

void Intake::SetFlipper(bool flip){
    flipper.Set(flip);
}

int Intake::SetPivotArm(double pos){
    if(pivot_enc.Get() < pos + 10){
        arm_pivot.Set(ControlMode::PercentOutput, 0.5);
        return 0;
    }else if(pivot_enc.Get() > pos - 10){
        arm_pivot.Set(ControlMode::PercentOutput, -0.5);
        return 0;
    }else{
        return 1;
    }
}
