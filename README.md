# arduino-classes
Usefull C++ classes for Arduino programming

## Controller Classes

###### Dynamics Controller
Determines output position required to achieve a desired velocity profile. The class is useful for managing the velocity profile of servo motors and encoder-controlled DC motors. The controller has two modes:
1. **Position Control** - Uses constant acceleration/deceleration and a maximum velocity limit to move to a specified position.
1. **Velocity Control** - Uses constant acceleration to achieve the desired velocity.

Both modes can be used smoothly in sequence by setting the new position/velocity goal.

###### PID Controller
Basic implementation of a Proportional, Integral and Derivative controller. The controller has several additional features, which can be enabled/disabled as required:
1. **Maximum output threshold** - the output value will be saturated if it exceeds this value.
1. **Cut-off threshold** - the controller will turn off if the error is below this threshold.


## Usage
1. Copy the header (*.hpp) and class (*.cpp) files into the same folder as your main program code. 
1. Include the library, substituting in the correct file name: `#include "header_filename.hpp"`.
1. Instantiate the objects: `ClassType newObjectName(variables)`. Make sure to follow the correct format of the constructor as defined in the relevant class code.