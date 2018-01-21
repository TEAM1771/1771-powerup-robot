#include "WPILib.h"

#include <IterativeRobot.h>

/* Dashboard Libraries */
#include <SmartDashboard/SmartDashboard.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>

/* Our classes */
#include <Definitions.h>
#include <DriveTrain.h>

using namespace frc;

class Robot: public IterativeRobot {
private:
    DriveTrain driveTrain;
public:
    
    Robot() :  driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_PORT){
        
    }
    
    void RobotInit() { }

    void DisabledInit() { }
    void AutonomousInit() { }
    void TeleopInit() { }
    void TestInit() { }

    void DisabledPeriodic() { }
    void AutonomousPeriodic() { }
    
    void TeleopPeriodic() {
        // *TO-DO: Change to joystick val functions
        driveTrain.Tank(TBD, TBD);
        driveTrain.AutoShift();
    }
    
    void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
