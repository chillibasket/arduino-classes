# arduino-classes
This repository contains a variety of useful C++ classes/libraries for Arduino programming. 
<br />
<br />

## Servo Trajectory Controller
Hobby servo motors are usually controlled by sending a position command using the "Servo" Arduino library. However, controlling the speed/velocity and acceleration of the servo motor as it moves is not as trivial. This servo trajectory class makes this controlling the motion of servo motors easy. You can supply a new target position to the class, and it automatically determines what the position of the servo motor should be at regular time intervals in order to achieve a desired velocity and acceleration profile. While the class is useful for managing the velocity profile of servo motors and encoder-controlled DC motors, it theoretically can also be used to control the trajectories of other systems. The class uses a simple "trapezoidal" velocity profile for all moves. The controller has three main modes:
1. **Position Control** - Uses constant acceleration/deceleration and a maximum velocity limit to move to a specified position.
1. **Position and Time Control** - Same as position control, but automatically adapts the maximum velocity so that the movement is completed in the desired amount of time.
1. **Velocity Control** - Uses constant acceleration to achieve the desired velocity; this mode cannot be used with hobby servo motors, but is useful for systems using DC motors with encoders and a PID controller.

The controller causes the servo motor to smoothly accelerate to a desired maximum velocity and then decelerate smoothly again as it reached its target position. An image of the resulting position, velocity and acceleration profile is shown below. For more information about this class, please have a look at the tutorial I wrote on my website: [https://wired.chillibasket.com/2020/05/servo-trajectory](https://wired.chillibasket.com/2020/05/servo-trajectory)

![](/servo-trajectory/trajectory-example.jpg)
*The graphs show the position, velocity and acceleration profile for a servo motor moving from a 20° to a 180° position and back again. The code used to obtain this result can be seen in the example sketch: "trajectory-example.ino"*
<br />
<br />

## PID Controller
Basic implementation of a Proportional, Integral and Derivative controller. The controller has several additional features, which can be enabled/disabled as required:
1. **Maximum output threshold** - the output value will be saturated if it exceeds this value.
1. **Cut-off threshold** - the controller will turn off if the error is below this threshold.
<br />
<br />

## Timer Interrupts
Code to set up timer interrupts for the Atmega 328 (Arduino Uno). The functions can calculate and set the timer0, timer1, and timer2 parameters for any valid frequency. 
<br />
<br />

## Generic Queue
A template queue class that can be set up to work with items of any data type. It has all the standard FIFO queue functions such as `push(newItem)`, `pop()`, and `peak()`. It can also be used as a LIFO queue by using `pop_back()` instead of `pop()`. By uncommenting the line `#define DYNAMIC_SIZE`, the class will automatically allocate more space to the queue if it becomes too full.
<br />
<br />

## Usage
1. Copy the header (*.hpp*) and class (*.cpp*) files into the same folder as your main program code. 
1. Include the library, substituting in the correct file name: `#include "header_filename.hpp"`.
1. Instantiate the objects: `ClassType newObjectName(variables)`. Make sure to follow the correct format of the constructor as defined in the relevant class code.

Additional example sketches are included with the *Dynamics Controller* and *Generic Queue* classes to show in more detail how the classes can be used.