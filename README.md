# Microcontroller Programming
This project concerns the ATMega4808 microcontroller programming for creating certain apps.

In order to run the C files and see the changes in the registers of ATMega4808 you will have to use a simulator. I used the Microchip Studio simulator which supports many AVR and SAM devices. Also in project files the Datasheet of ATMega4808 is contained.

The apps that are implemented with ATMega4808 are the following ones:
- **traffic_lights.c:** The purpose of this app is to simulate the operation of a motorway junction, which consists of a major road and a vertical railway. On the main road there is a traffic light for cars and a traffic light for pedestrians, which is only switched on after a push button is pressed. A tram passes by the railway at regular intervals t = T1 and interrupts normal traffic, i.e. the red traffic light for the main road and the green traffic light for pedestrians is switched on for a period of time t = T2. Also, when the pedestrian button is pressed, the red traffic light for cars, for the major road along with the green traffic light for pedestrians, is switched on for the same time interval t = T2. After the interval t = T2, the red light for pedestrians and the green light for the major road are activated. Before the pedestrian button can be pressed again a time interval t = T3 must have elapsed. In the case where the pedestrian light button is pressed and is already active (either it has already been pressed once or the tram is crossing the railway) the operation in progress will not be interrupted and the active time intervals will be maintained until they are completed. The ports of ATMega4808 which are used are the following ones:
  - PORTD0: LED of the pedestrians traffic light
  - PORTD1: LED of the motorway traffic light
  - PORTD2: LED of the railway traffic light
  - PORTF5: Pedestrians traffic light button
  - TCA0: Timer for counting the T1, T2 and T3 time intervals.

- **smart_home_device.c:** The purpose of app is to simulate the operation of a smart home device that moves in the space of an empty room. It starts from a corner of the room and its purpose is to draw the outline of the room. As it moves through the space by some sensors it has that measure the distance of the device from obstacles in its path(here the sensor values are measured by the Analog to Digital Converter(ADC)). If the value is below the allowable value the device will stop and move left. The device will choose to move right if the right sensor indicates that there is no wall to the right of the device, i.e. the value is above the allowable value. When it reaches the corner from which it started, the device must stop. Also, if a switch is pressed, the device must perform a 180 degree turn and return to its position following the path it has taken so far. The ports of ATMega4808 which are used are the following ones:
  - PORTD0: Right-hand drive LED
  - PORTD1: Front drive LED
  - PORTD2: Left-hand drive LED
  - PORTF5: Switch for activating reverse course
  - TCA0: Timer for counting the T1 and T2 time intervals.
  - ADC0: Analog-to-Digital-Converter that measures the distance of the device from obstacles

- **fan_movement_control.c:** The purpose of this app is to simulate the motion of a fan. A fan consists of two rotary motions, a circular motion of the blades and a circular motion of the base, so that the fan can rotate and cover more space. These two circular motions will be simulated at two different rates, determined by two different Pulse-Width Modulators(PWMs). The rate of each movement will be displayed on an LED, which will be turned on when the pulse is at the rising edge and turned off when the rising edge of the next pulse follows. When the switch is activated, the fan is activated. The base cycling is activated, simulated by a pulse period Tb = 2 ms and a duty cycle Db = 60%. This pulse drives the PORTD LED1 through the rising edges. The blade cycling is activated, which is simulated by a pulse period Tl = 1ms and duty cycle Dl = 50%. This pulse drives the PORTD LED0 through the rising edges. When the same switch is activated a second time after the fan has started to run, the period of the blade cycling pulse is doubled with a duty cycle Tl = 50%. The base cycling pulse continues to operate as described above. If the switch is pressed a third time, the fan is switched off and both pulses stop operating. The ports of ATMega4808 which are used are the following ones:
  - PORTD0: Fan blades motion LED
  - PORTD1: Fan base motion LED
  - PORTF5: Switch for fan activation/deactivation
  - TCA0: Timer that is used as PWM in split mode

- **smart_greenhouse_simulation.c:**
