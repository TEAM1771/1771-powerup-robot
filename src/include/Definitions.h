#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define TBD                 1771

// *TO-DO: CHANGE TBDs to actual values

#define PI                  3.1415926535897932384623383279
#define sigmoid(x)			1.0/(1+exp(x))
#define RIO_CYCLE           .02

/*========== Button Mappings ==========*/
#define INTAKE_POS_BUTTON   in.GetOtherButton(5)
#define INTAKE_IN_BUTTON    in.GetOtherButton(3)
#define INTAKE_OUT_BUTTON   in.GetOtherButton(4)
#define INTAKE_FLAP_BUTTON  in.GetOtherButton(1)
#define ELVTR_SCALE_BUTTON  in.GetOtherButton(7)
#define ELVTR_DOWN_BUTTON   in.GetOtherButton(11)
#define ELVTR_SWITCH_BUTTON in.GetOtherButton(9)
#define INTAKE_PLACE_BUTTON	in.GetOtherButton(6)
#define INTAKE_MANFL_BUTTON	in.GetRightButton(11)
#define ENABLE_CLIMB_BUTTON	in.GetRightButton(1) && in.GetLeftButton(1)
#define CLIMB_MOTOR_BUTTON	in.GetRightButton(3)
#define CLIMB_INTAKE_BUTTON	in.GetOtherButton(8)
#define INTAKE_FARDOWN		in.GetOtherButton(10)


/*========== Ports and Encoder channels ==========*/
#define JOY_LEFT            0
#define JOY_RIGHT           1
#define JOY_OTHER           2

// Motors
#define LTR_MOTOR_A         4
#define LTR_MOTOR_B         2
#define RTR_MOTOR_A         5
#define RTR_MOTOR_B         10
#define ELEVATOR_MOTOR_PORT 1
#define RIGHT_ARM_IN_PORT   3
#define LEFT_ARM_IN_PORT    6
#define ARM_PIVOT_PORT      7

// Encoders (and a single potentiometer)
#define L_ENC_CHA           0
#define L_ENC_CHB           1
#define R_ENC_CHA           2
#define R_ENC_CHB           3
#define ELEVATOR_ENC_CHA    TBD
#define ELEVATOR_ENC_CHB    TBD
#define ARM_PIVOT_ENC_A	    4
#define ARM_PIVOT_ENC_B     5

// Solenoids
#define SHIFTER_PORT        0
#define PTO_ENABLE_PORT     2
#define PTO_DISABLE_PORT	3
#define FLIPPER_PORT        1
#define WINGS_PORT          TBD

/*========== Speed constants and Encoder thresholds ==========*/
#define CL_SPEED            -.25

#define SHIFT_UP_SPEED      2900
#define SHIFT_DOWN_SPEED    2200
#define CL_SHIFT_UP_SPEED   TBD
#define CL_SHIFT_DOWN_SPEED TBD
#define ELEVATOR_SCALE_PT   39460.0
#define ELEVATOR_DOWN_PT    0.0
#define ELEVATOR_SWITCH_PT  10644.0
#define ELEVATOR_CLIMB_PT	34500.0
#define INTAKE_WHEEL_SPEED  1
#define INTAKE_IN_PT        -4750.0
#define INTAKE_UP_PT        -1000
#define INTAKE_DOWN_PT		-8400.0
#define INTAKE_SHOOT_PT     TBD

#define ARM_PIVOT_SPEED     TBD

#define ELEVATOR_GAP_COAST	2500
#define ELEVATOR_GAP_BRAKE	1000

/*========== Coefficients for PID controllers ==========*/
#define ELEVATOR_P          2
#define ELEVATOR_I          0
#define ELEVATOR_D          0
#define INTAKE_P			1.4

/*========== Tipping Angle Constants ==========*/
#define TIP_RATE_MAX        18


#endif // DEFINITIONS_H_
