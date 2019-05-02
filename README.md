# arduino-classes
Usefull C++ classes for Arduino programming

## Dynamics Controller
Determines output position required to achieve a certain velocity profile. The class is useful for managing the velocity profile of servo motors and encoder-controlled DC motors. The controller has two modes:
1. Position Control - Uses constant acceleration, deceleration, and a maximum velocity limit to move to the specified position.
1. Velocity Control - Uses constant acceleration to achieve the desired velocity.

Both modes can be used smoothly in sequence by setting the new position/velocity goal.

## PID Controller
A basic implementation of a Proportional, Integral and Derivative controller. The controller has several additional features, which can be enabled/disabled as required:
1. A maximum output value threshold - the output will be saturated if it exceeds this value.
1. A cut-off threshold - the controller will turn off if the error is below this threshold.