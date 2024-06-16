# Microcontroller Programming
This project concerns the ATMega4808 microcontroller programming for creating certain apps.

In order to run the C files and see the changes in the registers of ATMega4808 you will have to use a simulator. I used the Microchip Studio simulator which supports many AVR and SAM devices. Also in project files the Datasheet of ATMega4808 is contained.

The apps that are implemented with ATMega4808 are the following ones:
- **traffic_lights.c:** The purpose of this app is to simulate the operation of a motorway junction, which consists of a major road and a vertical railway. On the main road there is a traffic light for cars and a traffic light for pedestrians, which is only switched on after a push button is pressed. A tram passes by the railway at regular intervals t = T1 and interrupts normal traffic, i.e. the red traffic light for the main road and the green traffic light for pedestrians is switched on for a period of time t = T2. Also, when the pedestrian button is pressed, the red traffic light for cars, for the major road along with the green traffic light for pedestrians, is switched on for the same time interval t = T2. After the interval t = T2, the red light for pedestrians and the green light for the major road are activated. Before the pedestrian button can be pressed again a time interval t = T3 must have elapsed. In the case where the pedestrian light button is pressed and is already active (either it has already been pressed once or the tram is crossing the railway) the operation in progress will not be interrupted and the active time intervals will be maintained until they are completed.
The ports which are used are the following ones:
  - PORTD0: Led of the pedestrians traffic light
  - PORTD1: Led of the motorway traffic light
  - PORTD2: Led of the railway traffic light
  - PORTF5: Pedestrians traffic light button
  - TCA0: Timer for counting the T1, T2 and T3 time intervals.

- **smart_home_device.c:** The purpose of app is to simulate the operation of a smart home device that moves in the space of an empty room. It starts from a corner of the room and its purpose is to draw the outline of the room. As it moves through the space by some sensors it has that measure the distance of the device from obstacles in its path(here the sensor values are measured by the Analog to Digital Converter(ADC)). If the value is below the allowable value the device will stop and move left. The device will choose to move right if the right sensor indicates that there is no wall to the right of the device (also to be simulated with the ADC), i.e. the value is above the allowable value. When it reaches the corner from which it started, the device must stop. Also, if a switch is pressed, the device must perform a 180 degree turn and return to its position following the path it has taken so far.

- **fan_movement_control.c:**

- **smart_greenhouse_simulation.c:**
