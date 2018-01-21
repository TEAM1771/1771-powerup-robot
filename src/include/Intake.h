/* * * * * * * * * * * * * *
 *         Intake.h        *
 * by Justin DeSimpliciis  *
 * * * * * * * * * * * * * */

#ifndef INTAKE_H_
#define INTAKE_H_

#include "WPILib.h"
#include <ctre/Phoenix.h>

class Intake {
private:
    // Unsure of how many solenoids needed at the moment for the arms, will update soon
    Solenoid larm, harm, controller_arm;
    
    
    Solenoid flipper;
    TalonSRX rin, lin;
    
public:
    Intake(int larm_port, int harm_port, int flipper_port, int rin_port, int lin_port, int carm_port);
    
    enum ARM_PRES { LOW, HIGH };
    enum ARM_STATE { IN, OUT };
    
    void IntakeUpdate(ARM_STATE as, ARM_PRES ap);
    
    void SetIntakeWheels(double rate);
    void SetFlipper(bool flip);
    
};

#endif // INTAKE_H_
