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
    DriveTrain driveTrain;
    Inputs in;
    Elevator elevator;
    Solenoid wings;
    AHRS *navx;
    
    bool climb_button_state, climb_button_state_prev, is_climbing;
    
public:
    
    Robot() :   driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_PORT),
                in(JOY_LEFT, JOY_RIGHT, JOY_OTHER),
                elevator(ELEVATOR_MOTOR_PORT, ELEVATOR_ENC_CHA, ELEVATOR_ENC_CHB, ARM_SOLENOID_PORT, ARM_PIVOT_PORT, ARM_PIVOT_POT_PORT, FLIPPER_PORT, RIGHT_ARM_IN_PORT, LEFT_ARM_IN_PORT),
                wings(WINGS_PORT){
                    
                    try{
                        navx = new AHRS(SPI::Port::kMXP);
                    }catch(std::exception& e){
                        // Don't get an error please thank you
                    }
                    
                    climb_button_state = 0;
                    climb_button_state_prev = 1;
                    is_climbing = 0;
                    
    }
    
    void RobotInit() {
        wings.Set(1);
    }

    void DisabledInit() { }
    
    void AutonomousInit() { }
    void TeleopInit() { }
    void TestInit() { }

    void DisabledPeriodic() { }
    void AutonomousPeriodic() {
        
    }
    
    void TeleopPeriodic() {
        climb_button_state = in.GetOtherButton(CLIMB_BUTTON);
        
        if(climb_button_state != climb_button_state_prev){
            is_climbing = !is_climbing;
            climb_button_state_prev = climb_button_state;
        }
        
        if(!is_climbing){
            wings.Set(0);
            driveTrain.Tank(in.GetLeftY(), in.GetRightY());
            driveTrain.AutoShift();
            
            elevator.Set(in.GetOtherY());
            elevator.UpdatePID();
        }else{ // if bot is climbing
            wings.Set(1);
            // button pushed enabling climb, as soon as unpushed stop climb, joy, only climb if joy is going forward
            if(in.GetOtherY() > 0){
                
                driveTrain.Tank(in.GetOtherY(), in.GetOtherY());
                elevator.Set(in.GetOtherY());
            }else{
                elevator.Set(0);
            }
        }
    }
    
    void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
