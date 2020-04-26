# arduino-classes
Useful C++ classes for Arduino programming
</br>

## Dynamics Controller
Determines output position required to achieve a desired velocity profile. The class is useful for managing the velocity profile of servo motors and encoder-controlled DC motors. The controller has two modes:
1. **Position Control** - Uses constant acceleration/deceleration and a maximum velocity limit to move to a specified position.
1. **Velocity Control** - Uses constant acceleration to achieve the desired velocity.

Both modes can be used smoothly in sequence by setting the new position/velocity goal. For servo motors which only have a constrained range of motion, only the position control mode should be used. The controller causes the servo motor to smoothly accelerate to a desired maximum velocity and then decelerate smoothly again as it reached its target position. An image of the result position, velocity and acceleration profile is shown below:

![](/dynamics-controller/output-example.jpg)
*Image showing the position, velocity and acceleration profile for a servo motor moving from a 10° to a 180° position. The maximum velocity is 20°/s, acceleration is 10°/s^2 and deceleration is 8°/s^2.*
</br>


## PID Controller
Basic implementation of a Proportional, Integral and Derivative controller. The controller has several additional features, which can be enabled/disabled as required:
1. **Maximum output threshold** - the output value will be saturated if it exceeds this value.
1. **Cut-off threshold** - the controller will turn off if the error is below this threshold.
</br>

## Timer Interrupts
Code to set up timer interrupts for the Atmega 328 (Arduino Uno). The functions can calculate and set the timer0, timer1, and timer2 parameters for any valid frequency. 
</br>

## Generic Queue
A template queue class that can be set up to work with items of any data type. It has all the standard FIFO queue functions such as `push(newItem)`, `pop()`, and `peak()`. It can also be used as a LIFO queue by using `pop_back()` instead of `pop()`. By uncommenting the line `#define DYNAMIC_SIZE`, the class will automatically allocate more space to the queue if it becomes too full.
</br>

## Usage
1. Copy the header (*.hpp*) and class (*.cpp*) files into the same folder as your main program code. 
1. Include the library, substituting in the correct file name: `#include "header_filename.hpp"`.
1. Instantiate the objects: `ClassType newObjectName(variables)`. Make sure to follow the correct format of the constructor as defined in the relevant class code.

Additional example sketches are included with the *Dynamics Controller* and *Generic Queue* classes to show in more detail how the classes can be used.