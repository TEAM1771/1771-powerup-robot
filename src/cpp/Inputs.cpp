#include <Inputs.h>

Inputs::Inputs(int lport, int rport) : left(lport), right(rport){
    
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
