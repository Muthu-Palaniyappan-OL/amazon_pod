#ifndef CONTROLLER_H
#define CONTROLLER_H

#define LEFT_FORWARD D5
#define RIGHT_FORWARD D6
#define LEFT_BACKWARD D7
#define RIGHT_BACKWARD D8

void setup_controller();
void move_forward();
void move_backward();
void turn_left();
void turn_right();
void stop();
#endif