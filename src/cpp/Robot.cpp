#include "WPILib.h"

#include <IterativeRobot.h>

/* Dashboard Libraries */
#include <SmartDashboard/SmartDashboard.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>

/* Our classes */
#include <Definitions.h>
#include <DriveTrain.h>
#include <Inputs.h>

using namespace frc;

class Robot: public IterativeRobot {
private:
    DriveTrain driveTrain;
    Inputs in;
public:
    
    Robot() :   driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_PORT),
                in(JOY_LEFT, JOY_RIGHT){
        
    }
    
    void RobotInit() { }

    void DisabledInit() { }
    void AutonomousInit() { }
    void TeleopInit() { }
    void TestInit() { }

    void DisabledPeriodic() { }
    void AutonomousPeriodic() { }
    
    void TeleopPeriodic() {
        driveTrain.Tank(in.GetLeftY(), in.GetRightY());
        driveTrain.AutoShift();
    }
    
    void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
