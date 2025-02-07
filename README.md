# 🚀 BLDC Sensored Control using STM32 (180° Commutation)

## 📌 Overview

This repository contains a snapshot of the C firmware, we developed, and used for controlling a Brushless DC (BLDC) motor using an STM32 microcontroller with Hall effect sensors and 180° commutation.

Developed for my team's power inverter and BLDC of a small EV.

The project is ongoing and requires more enhancements and improvements for greater efficiency.

----------------------------------------------------------------------------------
## ✨ Features

  -  ✅ 180° Commutation: Ensures smooth torque delivery and improved motor performance.
  -  ✅ Hall Sensor-Based Feedback: Uses three Hall sensors for accurate rotor position detection.
  -  ✅ PWM Control: Generates precise PWM signals for motor phase switching to control speed.
  -  ✅ Complementary Switching: Configured to drive high-side and low-side switches efficiently.
  -  ✅ Dead time insertion can easily be implemented (although already safe for our inverter hardware).
  -  ✅ Hall Sensors Emulation is available for testing and development.
-------------------------------------------------------------------------------
## 🔧 Hardware
  -  🛠 STM32 Microcontroller (stm32f103c  Bluepill dev board)

  -  🔄 BLDC Motor (with Hall sensors)

  -  ⚡ Power Inverter and 48v DC Battery

## system specs:
  -> `hall_sensor` and `motor_utils` modules are intended to form some basic but useful API layer for motor control.

  [+]  Timer1: PWM generation for the commutation (10kHz switching)
  
  [+]  Timer2: Is used to emulate hall sensors.
  
  [+]  Timer3: Triggers ADC sampling of the throttle (at 200 ms rate).
  
  [+]  Timer4: ICU used to measure the speed of the Motor.
  
  [+]  EXTI is used on Rising/Falling Edges of Hall Sensors Inputs to determine the current position and take suitable commutation decision.
  
  ![Pin Outs](https://github.com/Mahmoussam/BLDC_180_commutation/blob/master/Screenshot%202025-02-07%20130101.png)

  ## 180 Commutaion?
  Our commutation is based on the one proposed [in this paper](https://www.researchgate.net/publication/232615872_180-Degree_Commutation_System_of_Permanent_Magnet_Brushless_DC_Motor_Drive_Based_on_Speed_and_Current_Control
  )
  The Idea is to utilize a phase for 180 degrees of the electrical period instead of only 120 degrees, below are demonstrated control signals for switching (Phases A, B, C => Blue, Orange, Green Respectly).
  
  ![.](https://github.com/Mahmoussam/BLDC_180_commutation/blob/master/Screenshot%202025-02-07%20121835.png)

  Complementary switching for low and high side switches (IGBJT)
  ![.](https://github.com/Mahmoussam/BLDC_180_commutation/blob/master/Screenshot%202025-02-07%20121904.png)
  
