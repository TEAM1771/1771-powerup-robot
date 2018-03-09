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
    frc::Joystick other;
    
public:
    Inputs(int lport, int rport, int oport);
    
    double GetLeftX();
    double GetLeftY();
    double GetRightX();
    double GetRightY();
    double GetOtherX();
    double GetOtherY();
    
    bool GetLeftButton(int bn);
    bool GetRightButton(int bn);
    bool GetOtherButton(int bn);
    
};

#endif // INPUTS_H_
