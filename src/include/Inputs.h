/* * * * * * * * * * * * * *
 *         Inputs.h        *
 * by Justin DeSimpliciis  *
 * * * * * * * * * * * * * */

#ifndef INPUTS_H_
#define INPUTS_H_

#include "WPIlib.h"

class Inputs {
private:
    frc::Joystick left;
    frc::Joystick right;
    
public:
    Inputs(int lport, int rport);
    
    double GetLeftX();
    double GetLeftY();
    double GetRightX();
    double GetRightY();
};

#endif // INPUTS_H_
