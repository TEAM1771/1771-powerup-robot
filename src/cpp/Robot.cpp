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
#include <Elevator.h>
#include <AHRS.h>


using namespace frc;

class Robot: public IterativeRobot {
private:
    frc::LiveWindow* lw = LiveWindow::GetInstance();
    frc::SendableChooser<std::string> chooser;
    DriveTrain driveTrain;
    Inputs in;
    Elevator elevator;
    Solenoid wings;
    AHRS *navx;
    bool naverr = 0;
    bool climb = 0;
public:
    
    Robot() :   driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_PORT),
                in(JOY_LEFT, JOY_RIGHT, JOY_OTHER),
                elevator(ELEVATOR_MOTOR_PORT, ELEVATOR_ENC_CHA, ELEVATOR_ENC_CHB, ARM_PIVOT_PORT, ARM_PIVOT_POT_PORT, FLIPPER_PORT, RIGHT_ARM_IN_PORT, LEFT_ARM_IN_PORT),
                wings(WINGS_PORT){
                    
                    try{
                        navx = new AHRS(SPI::Port::kMXP);
                    }catch(std::exception& e){
                        // Don't get an error please thank you
                        naverr = 1;
                    }
                    
                    navx->ZeroYaw();
                    
    }
    
    void RobotInit() {
        wings.Set(1);
        climb = 0;
    }

    void DisabledInit() { }
    
    void AutonomousInit() {
        navx->ZeroYaw();
        
    }
    void TeleopInit() {
        climb = 0;
    }
    void TestInit() { }

    void DisabledPeriodic() { }
    void AutonomousPeriodic() {
        
    }
    
    void TeleopPeriodic() {
        PutNumbers();
        if(in.GetLeftButton(2) && in.GetLeftButton(7) && in.GetRightButton(2) && in.GetRightButton(10)){
            driveTrain.EnableClimb();
            climb = 1;
        }
        
        if(!driveTrain.IsClimbing()){
            driveTrain.Tank(in.GetLeftY(),in.GetRightY());
            driveTrain.AutoShift();
        }else{
            driveTrain.Tank(in.GetLeftY(), in.GetRightY());
            /** CLIMB SHIFTING MIGHT BREAK EVERYTHING BEWARE **/
            driveTrain.AutoShift();
        }
    }
    
    void TestPeriodic() { }
    
    void PutNumbers(){
        SmartDashboard::PutString("Climb Status: ", std::to_string(climb));
        SmartDashboard::PutString("NavX Angle: ",std::to_string(navx->GetAngle()));
        SmartDashboard::PutString("NavX Error Status: ",std::to_string(naverr));
    }
};

START_ROBOT_CLASS(Robot)
