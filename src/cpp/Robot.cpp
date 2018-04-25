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
    frc::SendableChooser<std::string> *chooser;
    DriveTrain driveTrain;
    Inputs in;
    Elevator elevator;
    Solenoid wings;
    AHRS *navx;
    frc::DigitalInput hall_effect_sensor;
	
	std::string starting_pos;
	std::string auton_type;
	std::string fms_data;
	bool fms_data_err = 0;
    
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
    
    Robot() :   driveTrain(LTR_MOTOR_A, LTR_MOTOR_B, RTR_MOTOR_A, RTR_MOTOR_B, L_ENC_CHA, L_ENC_CHB, R_ENC_CHA, R_ENC_CHB, SHIFTER_PORT, PTO_ENABLE_PORT, PTO_DISABLE_PORT),
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
					
					SmartDashboard::PutString("AUTONOMOUS POSITION", "L");
					SmartDashboard::PutString("AUTONOMOUS TYPE", "DRSTR");
                    starting_pos = SmartDashboard::GetString("AUTONOMOUS POSITION", "L");
					auton_type = SmartDashboard::GetString("AUTONOMOUS TYPE", "DRSTR");
					fms_data = frc::DriverStation::GetInstance().GetGameSpecificMessage();
					if(fms_data.length() == 0)
						fms_data_err = 1;
					
                    navx->ZeroYaw();
					//chooser = new SendableChooser();
					//chooser->AddDefault("Drive Straight", new AUTO_DriveStraight());
                    
    }
    
    void RobotInit() {
        PutNumbers();
        wings.Set(1);
        navx->ZeroYaw();
        navrollinit = navx->GetRoll();
        navpitchinit = navx->GetPitch();
		driveTrain.DisableClimb();
		driveTrain.SetNeutralMode(NeutralMode::Coast);
		driveTrain.ResetEnc();
    }

    void DisabledInit() {
		elevator.SetNeutralMode(NeutralMode::Brake);
	}
    
    void AutonomousInit() {
		driveTrain.ResetEnc();
		elevator.SetNeutralMode(NeutralMode::Brake);
		elevator.intake.SetFlipper(1);
		driveTrain.DisableClimb();
		driveTrain.SetNeutralMode(NeutralMode::Coast);
		starting_pos = SmartDashboard::GetString("AUTONOMOUS POSITION", "L");
		auton_type = SmartDashboard::GetString("AUTONOMOUS TYPE", "DRSTR");
		fms_data = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	}
    
    void TeleopInit() {
		// REMOVE LATER
		driveTrain.ResetEnc();
		// REMOVE ^^ LATER
        navrollinit = navx->GetRoll();
        navpitchinit = navx->GetPitch();
		elevator.SetNeutralMode(NeutralMode::Brake);
        elevator.intake.SetFlipper(1);
		driveTrain.DisableClimb();
		elevatorPosition = ELEVATOR_DOWN_PT;
		driveTrain.SetNeutralMode(NeutralMode::Coast);
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
        /* Point p;
        double avgds = driveTrain.GetAvgRaw() - lastdist;
        p.Set(avgds*cos(navx->GetAngle()*PI/180.0), avgds*sin(navx->GetAngle()*PI/180.0));
        currentPoint += p;
        lastdist = driveTrain.GetAvgRaw(); */
        
		
		SmartDashboard::PutString("Match Time:", std::to_string(Timer::GetMatchTime()));
		driveTrain.DisableClimb();
/* 		if(auton_type.compare("DRSTR") == 0)
			AUTO_DriveStraight();
		else if(auton_type.compare("SW5050") == 0)
			AUTO_Switch5050();
		else
			SmartDashboard::PutString("Be sad?", "yes"); */
		//AUTO_Switch5050();
		AUTO_SwitchCenter();
    }
    
    void TeleopPeriodic() {
        /* SPIN CODE
         * driveTrain.Tank(-1, -1);
         */
        
        PutNumbers();
        //Ur mom gey
		
		
        if(!driveTrain.IsClimbing()){
            
			if(ENABLE_CLIMB_BUTTON){
				driveTrain.EnableClimb();
				return;
			}
			
			driveTrain.DisableClimb();
            driveTrain.Tank(in.GetLeftY(),in.GetRightY());
            
            IntakePosition();
            IntakeWheelControl();
            
			
            ControlElevator();
            elevator.UpdatePID();
            elevator.SetPosition(elevatorPosition);
			
            // Sarah is the best :3
            driveTrain.AutoShift();
        }else{																/*** CLIMBING CODE ***/
			driveTrain.SetNeutralMode(NeutralMode::Brake);
			elevator.SetNeutralMode(NeutralMode::Coast);
			elevator.intake.SetPivotArm(INTAKE_DOWN_PT);
			elevator.Set(0);
			
			
			if(in.GetRightY() < 0)
				driveTrain.Tank(in.GetRightY(), in.GetRightY());
			
			
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
        }else if(CLIMB_INTAKE_BUTTON){
			elevatorPosition = ELEVATOR_CLIMB_PT;
		}
    }
    
    // Intake Positioning
    void IntakePosition(){
        if(INTAKE_POS_BUTTON){
			elevator.intake.SetPivotArm(INTAKE_IN_PT);
		}else if(INTAKE_PLACE_BUTTON){
			elevator.intake.SetPivotArm(INTAKE_IN_PT);
		}else if(INTAKE_FARDOWN){
			elevator.intake.SetPivotArm(INTAKE_DOWN_PT);
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
	
	void AUTO_DriveStraight(){
		if(Timer::GetMatchTime() <= 9){
            driveTrain.Tank(-0.22, -0.22);
        }
		
        elevator.intake.SetPivotArm(INTAKE_UP_PT);
		elevator.intake.SetIntakeWheels(INTAKE_WHEEL_SPEED * 0.15);
	}
	
	void AUTO_Switch5050(){
		elevatorPosition = ELEVATOR_SWITCH_PT;
		elevator.SetPosition(elevatorPosition);
		elevator.UpdatePID();
		if(Timer::GetMatchTime() >= 11){
			elevator.intake.SetPivotArm(INTAKE_UP_PT);
			elevator.intake.SetIntakeWheels(INTAKE_WHEEL_SPEED * 0.15);
			driveTrain.Tank(-.22, -.22);
		}else if(Timer::GetMatchTime() >= 6 && Timer::GetMatchTime() <= 11 
					&& fms_data.c_str()[0] == 'L' && !fms_data_err){
			elevator.intake.SetPivotArm(INTAKE_IN_PT);
			elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED);
		}else{
			elevator.intake.SetPivotArm(INTAKE_UP_PT);
			elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED*.1);
		}
		
	}
	
	void AUTO_SwitchCenter(){
		static double secs_passed = 0;
		secs_passed = 15.0 - Timer::GetMatchTime();
		elevatorPosition = ELEVATOR_SWITCH_PT;
		elevator.SetPosition(elevatorPosition);
		elevator.UpdatePID();
		if(fms_data.c_str()[0] == 'L'){
			if(secs_passed < 1.2){
				driveTrain.Tank(.40, -.40);
				elevator.intake.SetPivotArm(INTAKE_UP_PT);
				elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED*.15);
			}else if(secs_passed > 1.2 && secs_passed < 5.2){
				elevator.intake.SetPivotArm(INTAKE_UP_PT);
				elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED*.15);
				driveTrain.Tank(-.70, -.70);
			}else{
				driveTrain.Tank(0, 0);
				elevator.intake.SetPivotArm(INTAKE_UP_PT);
				elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED);
			}
				
		}else if( fms_data.c_str()[0] == 'R'){
			if(secs_passed < 4){
				elevator.intake.SetPivotArm(INTAKE_UP_PT);
				elevator.intake.SetIntakeWheels(INTAKE_WHEEL_SPEED*.15);
				
				driveTrain.Tank(-.70, -.70);
			}else{
				driveTrain.Tank(0, 0);
				elevator.intake.SetPivotArm(INTAKE_UP_PT);
				elevator.intake.SetIntakeWheels(-INTAKE_WHEEL_SPEED);
			}
		} 
		/* if(secs_passed < 2.0)
			driveTrain.Tank(.33, .67);
		else if(secs_passed >= 2.0 &&  secs_passed < 4.0)
			driveTrain.Tank(.67, .33); */
		
		
		
		
	}
    
    
    void PutNumbers(){
        SmartDashboard::PutNumber("Climb Status: ", (driveTrain.IsClimbing()));
        SmartDashboard::PutNumber("NavX Angle: ",(navx->GetAngle()));
        SmartDashboard::PutNumber("NavX Pitch Corrected: ", (navx->GetRoll()-navrollinit));
        SmartDashboard::PutNumber("NavX Pitch Raw: ", (navx->GetRoll()));
        SmartDashboard::PutNumber("NavX Error Status: ",(naverr));
        SmartDashboard::PutNumber("Left Joystick Y: ", (in.GetLeftY()));
        SmartDashboard::PutNumber("Right Joystick Y: ", (in.GetRightY()));
        SmartDashboard::PutNumber("Other Joystick Y: ", (in.GetOtherY()));
        SmartDashboard::PutNumber("X:", (currentPoint.GetX()));
        SmartDashboard::PutNumber("Y:", (currentPoint.GetY()));
        SmartDashboard::PutNumber("Hall Effect Status:", (hall_effect_sensor.Get()));
        SmartDashboard::PutNumber("Pivot Arm Enc:", (elevator.intake.GetPivotEnc()));
        SmartDashboard::PutNumber("dRoll:", (dRoll));
        SmartDashboard::PutNumber("Elevator Encoder Value:", (elevator.GetElvtrEnc()));
		SmartDashboard::PutNumber("Transmission Avg Raw:", driveTrain.GetAvgRaw());
		SmartDashboard::PutNumber("Transmission Avg Dist:", (driveTrain.GetAvgDistance()));
		SmartDashboard::PutNumber("Transmission Speeds:", (driveTrain.GetSpeed()));
		SmartDashboard::PutString("FMS Switch Side:", fms_data.substr(0,1));
		SmartDashboard::PutString("Auton User-Given Side:", starting_pos.substr(0,1));
		SmartDashboard::PutNumber("FMS Switch == Auton User?:", fms_data.c_str()[0] == starting_pos.c_str()[0]);
		SmartDashboard::PutNumber("FMS Data Error Val:", fms_data_err);
		SmartDashboard::PutString("Raw FMS Data:", frc::DriverStation::GetInstance().GetGameSpecificMessage());
		SmartDashboard::PutNumber("Left Transmission Enc:", driveTrain.GetLeftEnc());
		SmartDashboard::PutNumber("Left Halfed Trans Enc:", driveTrain.GetLeftEnc()/2.0);
		SmartDashboard::PutNumber("Right Transmission Enc:", driveTrain.GetRightEnc());
    }
};

START_ROBOT_CLASS(Robot)
