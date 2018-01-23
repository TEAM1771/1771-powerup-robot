#include <Intake.h>

Intake::Intake(int larm_port, int harm_port, int flipper_port, int rin_port, int lin_port, int carm_port) :
larm(larm_port), harm(harm_port),
flipper(flipper_port),
lin(lin_port), rin(rin_port),
controller_arm(carm_port){
    IntakeUpdate(LOW_OUT);
}

void Intake::IntakeUpdate(ARM_STATE as){
    // Should work in theory possibly maybe must test
    switch(as){
        case LOW_IN:
            harm.Set(0);
            larm.Set(1);
            controller_arm.Set(0);
            break;
        case LOW_OUT:
            harm.Set(0);
            larm.Set(1);
            controller_arm.Set(1);
            break;
        case HIGH_IN:
            larm.Set(0);
            harm.Set(1);
            controller_arm.Set(0);
            break;
        case HIGH_OUT:
            larm.Set(0);
            harm.Set(1);
            controller_arm.Set(1);
    }
}

void Intake::SetIntakeWheels(double rate){
    rin.Set(ControlMode::PercentOutput, rate);
    lin.Set(ControlMode::PercentOutput, -rate);
}

void Intake::SetFlipper(bool flip){
    if(flip == 0){
        flipper.Set(flip);
    }else{
        // *TO-DO: CONFIRM that on flip down Intake arms must be LOW PRESSURE and IN
        IntakeUpdate(LOW_IN);
        flipper.Set(flip);
    }
}
