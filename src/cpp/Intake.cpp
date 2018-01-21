#include <Intake.h>

Intake::Intake(int larm_port, int harm_port, int flipper_port, int rin_port, int lin_port, int carm_port) :
larm(larm_port), harm(harm_port),
flipper(flipper_port),
lin(lin_port), rin(rin_port),
controller_arm(carm_port){
    // *TO-DO: add default state once IntakeUpdate is complete -> low pressure arms out
}

void Intake::IntakeUpdate(ARM_STATE as, ARM_PRES ap){
    // *TO-DO: add intake arm code once the process is drawn out and understood
}

void Intake::SetIntakeWheels(double rate){
    rin.Set(ControlMode::PercentOutput, rate);
    lin.Set(ControlMode::PercentOutput, -rate);
}

void Intake::SetFlipper(bool flip){
    if(flip == 0){
        flipper.Set(flip);
    }else{
        // *TO-DO: add condition: if flipping down (extending) set intake to LOW arms IN
    }
}
