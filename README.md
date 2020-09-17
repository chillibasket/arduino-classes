[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Issues](https://img.shields.io/github/issues-raw/chillibasket/arduino-classes.svg?maxAge=25000)](https://github.com/chillibasket/arduino-classes/issues)
[![GitHub last commit](https://img.shields.io/github/last-commit/chillibasket/arduino-classes.svg?style=flat)](https://github.com/chillibasket/arduino-classes/commits/master)

# Useful Libraries and Classes for Arduino
This repository contains a variety of useful C++ classes/libraries which I wrote for my Arduino projects. Each class is designed so that it can be easily reused in different projects; after all, there is no point writing the same code over and over again for each new application!
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

## Button Debounce
When pressing a button and reading the signal with the digital pins of an Arduino, the signal may be noisy during the transition. Often when pressing or releasing the button, the voltage signal oscillates (bounces) a few times before settling at the new state. This can cause the Arduino to think that the button was pressed multiple times! To avoid this, we need to use a "Debounce" algorithm to remove these spurious signals and get a more stable reading of the current button state. To achieve this, I've put together two classes which achieve the same goal but in slightly different ways...
1. **<TimeDebounce.hpp>** This class implements a traditional approach, using a timer. The digital reading of the button needs to remain the same for the entire duration of the timer (default time is 50 milliseconds) before the button state is updated. If the state bounces during this time period, the timer is reset and starts again.
1. **<BitDebounce.hpp>** This class is slightly faster and more efficient, looking at the history of button readings to determine whether to change the button state. The class saves the last 8 button readings (which can conveniently be stored in one byte), and changes the state if it detects three consecutive readings of the new value.

Both classes have the exact same functions and can be used in the same way (apart from the constructor). Check out the `<button-debounce.ino>` sample sketch to see how the classes can be used in practice. The functions are:
1. `update()` Read the latest digital value of the button and figure out if it has changed state. This functions returns TRUE/FALSE depending on whether the class thinks the (debounced) button state is HIGH/LOW. This function should be called at regular intervals and before calling any of the read()/onChange()/onRisingEdge()/onFallingEdge() functions.
1. `read()` This function does not test/update the digital readings from the button, but simple returns TRUE/FALSE depending on whether the class thinks the (debounced) button state is HIGH/LOW.
1. `onChange()` Return TRUE if a rising or falling edge was detected. Note that this function cannot be used at the same time as `onRisingEdge()` or `onFallingEdge()`, since the change detection flag is reset to prevent the same edge from being detected multiple times.
1. `onRisingEdge()` Returns TRUE if a rising edge was detected (transition from LOW state to HIGH state)
1. `onFallingEdge()` Returns TRUE if a falling edge was detected (transition from HIGH state to LOW state)
1. `count()` Returns the total number of button clicks `(presses + releases) / 2` recorded. Note that this value reset to 0 after `16383` clicks using <BitDebounce.hpp> and after `8191` clicks using <TimeDebounce.hpp>.
1. `reset()` Resets the click counter back to zero.
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

Additional example sketches are included with the *Servo Trajectory Controller*, *Button Debounce* and *Generic Queue* classes to show in more detail how the classes can be used.