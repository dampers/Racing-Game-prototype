#pragma once
#include <utility>

class KART
{
private:
    double speed = 0.0;
    double acc = 0.2;
    double dec = 0.3;
    double angle = 0.0;
    double turn_speed = 0.08;
    int pos_x;
    int pos_y;
    bool direction[6] = {};

    void acceleration();
    void friction();
    void drift();
    void checkBoundary();

public:

    KART(int pos_x, int pos_y, double speed) : pos_x(pos_x), pos_y(pos_y), speed(speed), angle(0.0) {}

    std::pair<int, int> getPosPair();
    bool getDrift();
    void putDirection(bool* direction);
    double getAngle();
    void drive();
    void checkMaterial(char material);

};