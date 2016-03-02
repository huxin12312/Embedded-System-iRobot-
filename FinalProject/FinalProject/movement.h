/*
 * movement.h
 *
 * Created: 5/23/2013 12:32:14 PM
 *  Author: newagain
 */ 


#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_forward(oi_t *sensor, int centimeters);

void turn_clockwise(oi_t *sensor, int degrees);

void turn_counterclockwise(oi_t *sensor, int degrees);

void move_backforward(oi_t *sensor, int centimeters);

#endif /* MOVEMENT_H_ */