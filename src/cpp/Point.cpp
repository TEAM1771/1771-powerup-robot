#include <Point.h>

Point::Point(){
    x = 0;
    y = 0;
}

double Point::GetX(){
    return x;
}

double Point::GetY(){
    return y;
}

void Point::SetX(double x){
    this->x = x;
}

void Point::SetY(double y){
    this->y = y;
}

void Point::Set(double x, double y){
    SetX(x);
    SetY(y);
}

Point Point::operator+(Point p){
    Point n;
    n.SetX(p.GetX()+this->GetX());
    n.SetY(p.GetY()+this->GetY());
    return n;
}

Point* Point::operator+=(Point p){
    this->SetX(this->GetX()+p.GetX());
    this->SetY(this->GetY()+p.GetY());
    return this;
}
