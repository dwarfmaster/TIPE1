
#include "robot.hpp"
/* TODO fill it with the arduino-specific code */

Robot::Robot()
{
}

Robot::~Robot()
{
}

void Robot::forward()
{
}

float Robot::stop()
{
    return 0.0f;
}

void Robot::turn(float rad)
{
}

bool Robot::discover()
{
    return false;
}

void Robot::send(char *buffer, int size)
{
}

int Robot::receive(char *buffer, int size)
{
    return 0;
}

void Robot::close()
{
}

float Robot::scan()
{
    return 0.0f;
}

float Robot::rand()
{
    return 0.0f;
}

void* Robot::alloc(unsigned long size)
{
    return 0;
}

void Robot::free(void* mem)
{
}

void* Robot::realloc(void* p, unsigned long nsize)
{
    return 0;
}

void Robot::memcpy(void* dest, void* src, unsigned long size)
{
}

