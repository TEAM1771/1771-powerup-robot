#include <Inputs.h>

Inputs::Inputs(int lport, int rport, int oport) : left(lport), right(rport), other(oport){
    
}

double Inputs::GetLeftX(){
    return left.GetX();
}

double Inputs::GetLeftY(){
    return left.GetY();
}

double Inputs::GetRightX(){
    return right.GetX();
}

double Inputs::GetRightY(){
    return right.GetY();
}

double Inputs::GetOtherX(){
    return other.GetX();
}

double Inputs::GetOtherY(){
    return other.GetY();
}

bool Inputs::GetLeftButton(int bn){
    return left.GetRawButton(bn);
}

bool Inputs::GetRightButton(int bn){
    return right.GetRawButton(bn);
}

bool Inputs::GetOtherButton(int bn){
    return other.GetRawButton(bn);
}
