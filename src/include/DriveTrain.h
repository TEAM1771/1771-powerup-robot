/* * * * * * * * * * * * * *
 *       DriveTrain.h      *
 * by Justin DeSimpliciis  *
 * * * * * * * * * * * * * */

#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include "WPILib.h"
#include <Transmission.h>

class DriveTrain {
private:
    Transmission ltrm;
    Transmission rtrm;
    
    // Ball shifter
    Solenoid shifter;
    
    // Power takeoff
    Solenoid pto_en;
	Solenoid pto_dis;
    bool isClimbing;
public:
    DriveTrain(int lm1, int lm2, int rm1, int rm2, int le1, int le2, int re1, int re2, int shift_port, int pto_en_port, int pto_dis_port);
    
    void Tank(double ljoy, double rjoy);
    
    double GetSpeed();
    
    void Shift(bool toShift);
    void AutoShift();
    
    // Seperate functions to avoid code accidental code
    void EnableClimb();
    void DisableClimb();
    bool IsClimbing();
    
    // Encoder functions
    double GetAvgDistance();
    double GetAvgRaw();
	void ResetEnc();
	double GetLeftEnc();
	double GetRightEnc();
	
	void SetNeutralMode(NeutralMode nm);
};

#endif /* DRIVETRAIN_H_ */
