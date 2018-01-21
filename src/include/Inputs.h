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
    
    bool GetLeftButton(int bn);
    bool GetRightButton(int bn);
    
};

#endif // INPUTS_H_
