# E.U.R.O

### INTODUCTION

> This is the Electronic Unidentified Robotic Object, or E.U.R.O for short. It is designed and implemented to mimic a real-time scheduled embedded system using an Arduino Uno board and the onboard ATMEGA238-B microcontroller. The system schedules several individual tasks of varying severities and types, ranging from hard to soft real-time systems. It ensures that all tasks attempt to meet their respective deadlines while prioritizing tasks of higher priority in the event of conflicts.

![Image](/EURO.png)

### FEATURES
* Lane Keeping Assistance that keeps car on track Lane Keeping Assistance had the highest `priority (3)` because missing its deadline can be fatal for the driver.

* Light Indicator that turns on light when it is dark. A gear display that displays current gear. Gear Display and Light Indicator both have same `priority (2)` as them missing a deadline is not fatal for the driver.

* Sound system thay plays music. Sound system has lowest `priority (1)` as if it misses deadline, it will only cause annoyance to the driver

### HARDWARE

|Sensors||
|------|---------|
|IR-Sensor|An analog /digital sensor that detects light intensity and returns a value between 0 (Detecting) and 1024 (not detecting).|
|Light Sensor Module|A digital sensor module that detects the existence of light 0(Detecting) and 1(not detecting)in the surrounding environment using the on board photo-resistor.|
|Joy Stick Module|An analog sensor that detects motion in both x and Y direction and returns value between 0 and 1024|

|Actuators||
|---------|------|
|5V DC Motor and H-Bridge L298N|These components are used in combination with PWM (Pulse Width Modulation) to simulate motion at different speeds.|
|4-Digit 7-Segment Display|This display is utilized for presenting data that represents the system state.|
|8-Ohm Speaker and SD Card Module|An 8-ohm speaker and an SD card module are employed to play various sounds and audio.wav files. The speaker produces audible output based on the digital audio signals received from the SD card.|
|RGB LED|The RGB LED module, equipped with an onboard resistor, includes red, green, and blue LEDs. It is utilized to indicate different system states through different colors of light.

### LIBRARIES

|Libraries||
|---------|------|
|TM1637Display.h|This library is used to communicate with the 7-segment display mentioned above. It maps the value of the desired character to different segments of the specified segment display. |
|SdFat.h|The SdFat library is utilized to communicate with the SD Card module. It enables reading data from the SD Card and sending it through the Arduino board.|
|TMRpcm.h|The TMRpcm library facilitates communication with the 8-ohm speaker. It sends values read from the SD card to the speaker as a digital audio signal, allowing control over functions such as pausing, playing, and skipping to the next audio files.|
|Arduino_FreeRTOS.h|The Arduino_FreeRTOS library is employed for creating and managing various tasks within the system. It enables assigning priorities to these tasks based on the developer's intentions and scheduling them accordingly.|

### HANDLING INPUTS

* Lane Keeping Assist (LKA):

    The LKA system utilizes two infrared (IR) sensors to read the road. If these sensors detect a high value (indicating a black line), the input is sent to adjust the speed of one side of the car, allowing it to return to the lane. For example, if the right IR sensor detects a black line, the input value becomes greater than 700, indicating that the car is sliding to the left and leaving the lane. As a result, the right motor slows down (while the left motor remains at the same speed), causing the car to move back into the lane.

* Control Indicators:

    * A joystick is used to change the gears displayed on the seven-segment display. By inputting values such as X > 1000, X < 50, Y > 1000, or Y < 50 to the Arduino, the Arduino sends the corresponding output to the seven-segment display.

    * The light sensor is employed to control the RGB LED module through the Arduino board. When the light sensor detects light (represented by a value of 1), it sends a signal to the Arduino. The microcontroller on the Arduino then generates a high signal to illuminate the LED.

* Sound System:

    The SD Card module reads data from the installed 4GB SD Card and sends it to the Arduino board, which then routes the audio to the 8-ohm speaker.

___
