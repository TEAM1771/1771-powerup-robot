#include <Intake.h>

Intake::Intake(int arm_port, int flipper_port, int rin_port, int lin_port) :
arm(arm_port),
flipper(flipper_port),
lin(lin_port), rin(rin_port),{
}

void Intake::SetArms(bool toBeOrNotToBe){
    arms.Set(toBeOrNotToBe);
}

void Intake::SetIntakeWheels(double rate){
    rin.Set(ControlMode::PercentOutput, rate);
    lin.Set(ControlMode::PercentOutput, -rate);
}

void Intake::SetFlipper(bool flip){
    if(flip)
        SetArms(0);
    flipper.Set(flip);
}
