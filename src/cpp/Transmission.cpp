#include <Transmission.h>

Transmission::Transmission(int m1port, int m2port, int ech1, int ech2) : motor1(m1port), motor2(m2port), enc(ech1, ech2){
    
}

void Transmission::Set(double rate){
    motor1.Set(ControlMode::PercentOutput, rate);
    motor2.Set(ControlMode::PercentOutput, rate);
}

void Transmission::SetNeutralMode(NeutralMode nm){
    motor1.SetNeutralMode(nm);
    motor2.SetNeutralMode(nm);
}

double Transmission::GetM1Current(){
    return motor1.GetOutputCurrent();
}

double Transmission::GetM2Current(){
    return motor2.GetOutputCurrent();
}

double Transmission::GetAvgCurrent(){
    return (GetM1Current() + GetM2Current())/2;
}

void Transmission::ResetEnc(){
    enc.Reset();
}

double Transmission::GetDistance(){
    return enc.GetDistance();
}

double Transmission::GetRawEnc(){
    return enc.Get();
}

double Transmission::GetSpeed(){
    return enc.GetRate();
}
