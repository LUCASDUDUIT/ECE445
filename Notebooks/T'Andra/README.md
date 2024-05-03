# T'Andra Newby's Worklog

## WEEK 2/12/24 
This week's notes consist of possible materials for dog feeder as we brainstorm the structure of our design. We are also beginning to research components for our design document.
  * Sheetmetal container formed into a box/silo that holds 50-60 lbs of dog food (can be ordered through service shop for free)
  * The reciprocal is sheltered and protected by a hinged and motor driven lid.
  * LCD screen and button/encoder for user to unit communication
  * Infrared sensor for food control in reciprocal
  *Servo motor dynamic loads
  *Stepper motor for holding torque
  * Grove - 125KHz RFID Reader
  *  two resistors
  *  Rotary encoder
     ![sketch of dog feeder functionality](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20125332.png)
  * some datasheets:
     *  [Transmitter](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Transmitter%20TX118SA_DATASHEET.pdf)
     *  [Stepper Controller](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Motor_DriverL298N.pdf)
     *  [OLED](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/OLED_DISPLAY_WEA012864DBPP3N00003.pdf)
     *  [Rotary Encoder](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Rotary_Encoder_PEC11R.pdf)
     *  [stm32 mcu](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/stm32f205rc.pdf)
## WEEK 2/19/24
This week's objective is to complete the design review and record feedback from the TA about our proposal.
  * High level requirements:
       * try to make high level requirements more testable.
       * specify how far the rfid has to be for receptacle to 
         open 
         * specify which rfid to use, bandwidth
       * be quantitative with how much food is dispensed
       * depending on how much the battery is at currently, 
         when does the solar power begin to charge the 
         battery?
  * block diagram:
     * specify pins that’s used on micro controller
     * receptacle should be under food container 
     * what solar panel is being used? and how much energy is being dispensed
     * what communication is being used (bluetooth, wifi)
     * specify voltage on arrows
  * subsystems
     * be specific on requirements, mentioning measurements, 
     * the motor will be running based on time, how much food 
       should be dispensed by that time?
     * Refer to spring 2023 project 40, rfid door lock
      ![original block diagram](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20125406.png)
       
## WEEK 2/26/24
This week's objective is to prepare for the design review and work on design the PCB based on our schematic. I contributed to creating the rfid module, which was later revised by another teammate.
*	![transmitter](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122007.png)	
* ![receiver](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122015.png)	

## WEEK 3/4/24
This week's objective is to work on the team evaluation assignment. By this point, out team has specified what tasks we will partake in. My responsibility will be programming the dog feeder based on the skills I have learned. 
* Brainstorming team rules:
    * Everyone should respond to each other in a timely manner so there would not be any delays with completing assignments.
    * Everyone should ask for help when it is needed, or everyone should help when it seems like it is needed.
    * Everyone should be willing to participate in something regarding the project, no one should be slacking.

## WEEK 3/18/24
This week's objective is to start building the program for the dog feeder. We have not completed an official design of our PCB, so I received a stm32nucleo to practice programming for our stm32f205rct7 mcu. I have also taken notes on feedback from the TA regarding out design document.
* I will use stm32duino application to program our pcb and the stm32nucleo devboard
* Objective is to initialize the base of the functionality: pins, RTC, and memory.
* ![image](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20142338.png)
* Improvements:
    * rv table should suffice for all subsystems
       * elaborate/explain results in rv 
    * More detail for each subsystem 
       * Schematics of the components for the subsystem
    * Verification for the requirments
    * More resources, supporting material
    * More math for tolerance, rfid 

## WEEK 3/25/24
This week's objective is to complete the individual progress report and continue to program the stm32nucleo devboard. The stm32cubeide application seems more helpful and understanding when finding libraries and generating code for stm32nucleo, so I switched to that. RTC has been implemented and tested, peripherals such as UART has been implemented, and pins have been specified for components of the project, such as the DC and stepper motor, and infrared sensors.

* After some research and YouTube videos, I have written code for the NEMA 17 stepper motor, DC. waiting to receive the components to test these.

* ![stepper](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122703.png)
* ![dc_motor](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122721.png)

## WEEK 4/1/24
This week's objective is to continue programming the dog feeder, implementing the IR sensors, OLED, and rotary encoder.
With some help and research, I created base code for our ssd1306 OLED, rotary encoder, and select button. This base code should just get the components running and working. then I will incorporate them into each other to get the dog feeder's functionality. 
* The oled uses i2c communication
* These will each be tested once the OLED, rotary encoder, and select button have arrived.

## WEEK 4/8/24
This week's objective is to prepare for the mock demonstration and continue programming the dog feeder via stm32cubeide.
* The TA advised that since we don't have our PCB in yet, we should show something else physically worked on such as a breadboard with our intended circuit, and preferably the structural aspect of the dog feeder.
* I implemented base code for the last components in our design, the rfid and the infrared emitters and detectors.
* ![IR CODE](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122509.png)
* ![IR_INITIALIZE](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20122149.png)
* The rfid uses UART2 communication. The HAL_UART2_Transmit function is supposed to transmit a signal of a string "range" from the dog feeder to the dog collar. And the feeder uses HAL_UART2_Receive function to receive the string signal " open" from the dog collar to open the dog feeder.

## WEEK 4/15/24
This week's objective is to test the code of each integral since we have received all of the parts. Once they work, integrate them into each other.

* Stepper motor did not work, had to use a stepper.h/.c library that similar to the Arduino library due to the certain sequences of bits applied to each pin of the stepper motor controller. stepper works now.
* ![stepper motor video](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/20240417_081609%20(1).mp4)
    * Warning: the video must be downloaded to view the video
oled did not seems to work using u82g library, so I switched to a fonts.h/.c and ssd1306.h/.c files. those functions worked.
    * ![image](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20142747.png)
* Rotary encoder and select button worked. Rotary encoder had some glitches when incrementing and decrementing. So we will just increment and loop back around when selection options.
    * ![rotary encoder video](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/20240418_155050.mp4)
         * Warning: the video must be downloaded to view the video
* Dc motor worked as it was just writing the output pin to 1 to run the motor. 
* In all, the user interface high level requirement has been fulfilled. The dispensing has slightly been fulfilled as I have not tested the IR sensors.
  * ![user interface](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/IMG_2213.jpg)
    * ![user interface video](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Snapchat-437174761%20(1).mp4)
        * Warning: the video must be downloaded to view the video

## WEEK 4/22/24
This week’s objective is to finish testing code and integrate them to fulfill our dispensing and rfid requirements. We will also have our final demonstration and mock presentation.
* We did not have the infrared emitters, only the detectors. I went to buy some from the ECE service shop. The detectors did not work. Damaged the emitters and bought new ones. The detectors still did not work even with assistance from my other teammates. I decided to try to buy different IR detectors. though, we still could not get them to work. Since this was not a part of our high level, we decided to not attempt the sensors due to time.
* The RFID did not work after multiple attempts during the past few days. I did further research about the stm32nucleo and discovered that the stm32nucleo is soldered in a way that UART communication only transmits to the console via usb (sb13 and sb14 soldered) for ST-LINK, and not transmit to the TX pins on the devboard (sb62 and sb63 not soldered).
  
  * [stm32nucleo manual](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)
     * On page 25, it mentions what the usart2 communication based on its soldering,
* In addition rh_ask, a Arduino library, is in c++ and the stm32cubeide project was initialized in c. So, we tried to mimic a signal sent by sending and receiving sequence of 1's and 0's. This did not work due to noise that interrupts the sequence, and the difficulties in measuring delays for the signal to be received.
     *  Therefore, we decided to use Arduino’s to communicate the two rfid modules. We would send a high from the dev board to one rfid module to start communicating with another and receive a high to the dev board once the other rfid module is in range, to open the dog feeder.

* Everything seems to be working in terms of the programming. This could not be transferred over to our PCB due to time, and we also did not have the ST-LINK v2 programmer for our PCB. the finished project is uploaded to my notebook folder.
* UPDATE: after rewiring the breadboard and Arduino board for a more pleasing visual, we accidentally supplied 12v to a 5v power rail, which damaged an rfid module, an Arduino board, and my laptop. This occurred right before demo, so we were not able to present our functionality.
## WEEK 4/29/24
This week's objective is to present our improved slides from the mock presentation and attempt another demonstration as our previous one did not go as expected. 
* Update: I received a temporary laptop from a family member, so I will have to download the project, application, and rerun it to the stm32nucleo devboard.
     * There were some errors that were fixed. However, the dog feeder's door did not open due to lack of friction to keep the lid up after opening. This was the final result before demoing.

![flow chat for code](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/T'Andra/Screenshot%202024-05-02%20125418.png)

