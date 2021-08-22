#include "stdafx.h"

void KART::acceleration()
{
    if (direction[UP] && speed < MAX_SPEED)
    {
        if (speed < 0) speed += dec; // deceleration
        else speed += acc; // acceleration
    }
    if (direction[DOWN] && speed > -MAX_SPEED)
    {
        if (speed > 0) speed -= dec; // deceleration
        else speed -= acc; // acceleration
    }
}

void KART::friction()
{
    if (!direction[UP] && !direction[DOWN])
    {
        if (speed - dec > 0) speed -= dec;
        else if (speed + dec < 0) speed += dec;
        else speed = 0;
    }
    if (direction[LEFT] && speed != 0.0) angle -= turn_speed * speed / MAX_SPEED;
    if (direction[RIGHT] && speed != 0.0) angle += turn_speed * speed / MAX_SPEED;
}

void KART::drift()
{
    if (direction[DRIFT])
    {
        if (direction[LEFT] && speed != 0.0) angle -= turn_speed * (MAX_SPEED - speed);
        if (direction[RIGHT] && speed != 0.0) angle += turn_speed * (MAX_SPEED - speed);
        if (speed - dec > 0) speed -= dec;
    }
}

void KART::checkBoundary()
{
    if (pos_x < CAMERA_WIDTH * 2)
    {
        speed = 0.0;
        pos_x = CAMERA_WIDTH * 2 + 1;
    }
    if (pos_x > MAP_WIDTH - 2 * CAMERA_WIDTH)
    {
        speed = 0.0;
        pos_x = MAP_WIDTH - 2 * CAMERA_WIDTH - 1;
    }
    if (pos_y < CAMERA_HEIGHT * 2)
    {
        speed = 0.0;
        pos_y = CAMERA_HEIGHT * 2 + 1;
    }
    if (pos_y > MAP_HEIGHT - 2 * CAMERA_HEIGHT)
    {
        speed = 0.0;
        pos_y = MAP_HEIGHT - 2 * CAMERA_HEIGHT - 1;
    }
}

std::pair<int, int> KART::getPosPair()
{
    return std::make_pair(pos_x, pos_y);
}

bool KART::getDrift()
{
    return direction[DRIFT];
}
void KART::putDirection(bool* direction)
{
    for (int i = 0; i < 6; i++)
        this->direction[i] = direction[i];
}

double KART::getAngle() { return angle; }

void KART::drive()
{
    /*curr = clock();*/

    acceleration();
    friction();
    drift();

    pos_x += sin(angle) * speed;
    pos_y -= cos(angle) * speed;

    if (angle >= 2 * PI) angle -= 2 * PI;
    if (angle <= -2 * PI) angle += 2 * PI;

    checkBoundary();

    /*fprintf(fileDrive, "%lf\n", ((double)clock() - curr) / 1000);*/
}

void KART::checkMaterial(char material)
{
    if (speed > 1.0 && (material == 'G' || material == 'Y'))
        speed = 1.0;
}