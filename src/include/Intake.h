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
    Solenoid arms;
    
    Solenoid flipper;
    TalonSRX rin, lin;
    
public:
    Intake(int arm_port, int flipper_port, int rin_port, int lin_port);
    
    void SetArms(bool state);
    
    void SetIntakeWheels(double rate);
    void SetFlipper(bool flip);
    
};

#endif // INTAKE_H_
