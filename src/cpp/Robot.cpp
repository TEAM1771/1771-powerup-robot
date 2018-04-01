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
#include <Point.h>

class Robot: public IterativeRobot {
private:
    frc::LiveWindow* lw = LiveWindow::GetInstance();
    frc::SendableChooser<std::string> chooser;
    DriveTrain driveTrain;
    Inputs in;
    Elevator elevator;
    Solenoid wings;
    AHRS *navx;
    frc::DigitalInput hall_effect_sensor;
    
    // Error correction due to diagonal electronics board
    double navrollinit, navpitchinit;
    
    bool naverr = 0;
    double lastdist = 0;
    double start_angle;
    double navlastroll = 0.0;
    double dRoll = 0.0;
    
    // Point for auton
    Point currentPoint;
    
    int intake_val = 0;
    
    enum ELEVATOR_POSITIONS { ELEVATOR_DOWN, ELEVATOR_SWITCH, ELEVATOR_SCALE };
    int elevatorPosition = ELEVATOR_DOWN;
    
public:
    
    Robot() :   driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_PORT),
                in(JOY_LEFT, JOY_RIGHT, JOY_OTHER),
                elevator(ELEVATOR_MOTOR_PORT, ELEVATOR_ENC_CHA, ELEVATOR_ENC_CHB, ARM_PIVOT_PORT, ARM_PIVOT_ENC_A, ARM_PIVOT_ENC_B, FLIPPER_PORT, RIGHT_ARM_IN_PORT, LEFT_ARM_IN_PORT),
                wings(WINGS_PORT),
                hall_effect_sensor(0){
                    
                    try{
                        navx = new AHRS(SPI::Port::kMXP);
                    }catch(std::exception& e){
                        // Don't get an error please thank you
                        naverr = 1;
                    }
                    
                    navx->ZeroYaw();
                    
    }
    
    void RobotInit() {
        PutNumbers();
        wings.Set(1);
        navx->ZeroYaw();
        navrollinit = navx->GetRoll();
        navpitchinit = navx->GetPitch();
    }

    void DisabledInit() {
		elevator.SetNeutralMode(NeutralMode::Brake);
	}
    
    void AutonomousInit() {
		elevator.intake.SetFlipper(1);
	}
    
    void TeleopInit() {
        navrollinit = navx->GetRoll();
        navpitchinit = navx->GetPitch();
		elevator.SetNeutralMode(NeutralMode::Brake);
        elevator.intake.SetFlipper(1);
		elevatorPosition = ELEVATOR_DOWN_PT;
    }
    
    void TestInit() {
        navrollinit = navx->GetRoll();
        navpitchinit = navx->GetPitch();
        elevator.intake.SetFlipper(1);
    }

    void DisabledPeriodic() {
        PutNumbers();
    }
    
    void AutonomousPeriodic() {
        Point p;
        double avgds = driveTrain.GetAvgRaw() - lastdist;
        p.Set(avgds*cos(navx->GetAngle()*PI/180.0), avgds*sin(navx->GetAngle()*PI/180.0));
        currentPoint += p;
        lastdist = driveTrain.GetAvgRaw();
        
		
		SmartDashboard::PutString("Match Time:", std::to_string(Timer::GetMatchTime()));
		
        if(Timer::GetMatchTime() <= 9){
            driveTrain.Tank(-0.2, -0.2);
        }
		
        elevator.intake.SetPivotArm(INTAKE_UP_PT);
		elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED*.1);
    }
    
    void TeleopPeriodic() {
        /* SPIN CODE
         * driveTrain.Tank(-1, -1);
         */
        
        PutNumbers();
        
        if(!driveTrain.IsClimbing()){
            
            driveTrain.Tank(in.GetLeftY(),in.GetRightY());
            
            IntakePosition();
            IntakeWheelControl();
            
			
            ControlElevator();
            elevator.UpdatePID();
            elevator.SetPosition(elevatorPosition);
			
            // Sarah is the best :3
            driveTrain.AutoShift();
        }else{
            driveTrain.Tank(in.GetLeftY(), in.GetRightY());
            /** CLIMB SHIFTING MIGHT BREAK EVERYTHING BEWARE **/
            // driveTrain.AutoShift();
        }
    }
    
    void TestPeriodic() {
        PutNumbers();
    }
    
    
    /** User-Defined Functions **/
    
    void ControlElevator(){
        if(ELVTR_DOWN_BUTTON){
            elevatorPosition = ELEVATOR_DOWN_PT;
        }else if(ELVTR_SCALE_BUTTON){
            elevatorPosition = ELEVATOR_SCALE_PT;
        }else if(ELVTR_SWITCH_BUTTON){
            elevatorPosition = ELEVATOR_SWITCH_PT;
        }
    }
    
    
    
    // Intake Positioning
    void IntakePosition(){
        if(INTAKE_POS_BUTTON){
			elevator.intake.SetPivotArm(INTAKE_IN_PT);
		}else if(INTAKE_PLACE_BUTTON){
			elevator.intake.SetPivotArm(INTAKE_IN_PT);
		}else{
			elevator.intake.SetPivotArm(INTAKE_UP_PT);
		}
		
		if(INTAKE_MANFL_BUTTON || INTAKE_PLACE_BUTTON){
			elevator.intake.SetFlipper(0);
		}else{
			elevator.intake.SetFlipper(1);
		}
    }
    
    // Intake Wheel Control
    void IntakeWheelControl(){
        if(INTAKE_IN_BUTTON){
            elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED);
        }else if(INTAKE_OUT_BUTTON){
            elevator.intake.SetIntakeWheels(INTAKE_WHEEL_SPEED);
        }else{
            elevator.intake.SetIntakeWheels(INTAKE_WHEEL_SPEED * 0.15);
        }
    }
    
    // AntiTip -> Uses the derivative of the angle (the rate of change in angle) to engage motors to prevent tipping
    void AntiTip(){
        /* DO NOT REMOVE INTAKE UP ANTI-TIP */
        dRoll = (navlastroll - (navx->GetRoll()-navrollinit))/RIO_CYCLE;
        navrollinit = navx->GetRoll();
        if(fabs(dRoll) > TIP_RATE_MAX){
            elevator.intake.SetPivotArm(INTAKE_UP_PT);
        }
        
    }
    
    
    void PutNumbers(){
        SmartDashboard::PutString("Climb Status: ", std::to_string(driveTrain.IsClimbing()));
        SmartDashboard::PutString("NavX Angle: ",std::to_string(navx->GetAngle()));
        SmartDashboard::PutString("NavX Pitch: ", std::to_string(navx->GetRoll()-navrollinit));
        SmartDashboard::PutString("NavX Pitch Raw: ", std::to_string(navx->GetRoll()));
        SmartDashboard::PutString("NavX Error Status: ",std::to_string(naverr));
        SmartDashboard::PutString("Left Joystick Y: ", std::to_string(in.GetLeftY()));
        SmartDashboard::PutString("Right Joystick Y: ", std::to_string(in.GetRightY()));
        SmartDashboard::PutString("Other Joystick Y: ", std::to_string(in.GetOtherY()));
        SmartDashboard::PutString("X:", std::to_string(currentPoint.GetX()));
        SmartDashboard::PutString("Y:", std::to_string(currentPoint.GetY()));
        SmartDashboard::PutString("Hall Effect Status:", std::to_string(hall_effect_sensor.Get()));
        SmartDashboard::PutString("Intake Wheel Direction:", std::to_string(intake_val));
        SmartDashboard::PutString("Pivot Arm Enc:", std::to_string(elevator.intake.GetPivotEnc()));
        SmartDashboard::PutString("dRoll:", std::to_string(dRoll));
        SmartDashboard::PutString("Elevator Encoder Value:", std::to_string(elevator.GetElvtrEnc()));
		SmartDashboard::PutString("Transmission Avg Raw:", std::to_string(driveTrain.GetAvgRaw()));
		SmartDashboard::PutString("Transmission Avg Dist:", std::to_string(driveTrain.GetAvgDistance()));
		SmartDashboard::PutString("Transmission Speeds:", std::to_string(driveTrain.GetSpeed()));
    }
};

START_ROBOT_CLASS(Robot)
