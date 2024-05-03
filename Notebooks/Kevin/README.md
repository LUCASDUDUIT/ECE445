# Kevin's Notebook

## WEEK 2/12/24 
This week, we started designing the dog feeder and what material we should be using as the design document review is coming up next week. 
We discussed the necessary components. We had a rough idea of what we need.

## WEEK 2/19/24 
This week, we were instructed writing the design document, like what essential things need to be included. I made a block diagram on lucidchart.com. Here's how it looks like: ![blockdiagram](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-02%20at%208.32.26%20PM.png)

An important discussion Lucas made with me was about the use of infrared emitter and sensor for detecting food overflow. At first, I thought this was not a good idea. I insisted of using a small weight sensor to detect the weight of the food and display to the user because the user wants to know how much food they want their dog to have for each meal quantitatively. However, Lucas told me about the mechanical disadvantage of usign a weight sensor: to prevent food falling out from the food receptacle, the food receptacle has to cover cross-sectional area of the feeder while allowing some room to move up and down because the weight sensor will have some concavity when some weight is on the scale. We concluded that this would be very hard to fabricate mechanical-wise. Therefore, we switched to the idea of letting the user hold down a button to run the DC motor, and food will be dispensed. It stops until user release the botton. This way, not only have we reduced the complexity of the design with the weight sensor, but also made it more simple and straightforward. In reality, most pet owners feed their pets with visual reference instead of quantitative measurement. So long as the user observe how much food is dispensed, they will have a good control over how much food they want to feed. The IR sensor is just like a cherry-on-top. It prevents food overfill but detecting if the food blocks the IR sensor, if so, the feeder will not dispense. I think this is a great idea!

## WEEK 2/26/24 
This week was an important week as we got our design reviewed by the professor, TAs, and peers. We were able to get valuble feedbacks from them. I will conclude a few important notes:
  * Our PCB design was not yet completed, our TA suggested us to hurry up and finish to catch the first couple rounds of PCB order.
  * Jason confirmed that we are allowed to use breakboards for the stepper motor driver as we cannot practically build that.
  * Design document has some flaws like our verification method was not feasible for demo purpose, we need to make them more "demoable".
Besides these feedbacks, at this stage, we ARE a little bit behind!

## WEEK 3/4/24 
At the start of this week, Lucas and I finished our intial PCB design with the schematic since we did not actually get our PCB reviewed last week. Here is what we have so far:
![Programmable Subsystem](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-01%20at%2012.25.29%20AM.png)
![Sensor Subsystem](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-01%20at%209.05.24%20PM.png)
![Power Subsystem](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-01%20at%209.08.25%20PM.png)
![Control Subsystem](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-01%20at%209.17.14%20PM.png)
It took us a lot of time by just deciding connectors, peripherals, and finding appropriate symbols and footprints for them. STM32 series does not have a simple configuration set up. We went on different websites and even on YouTube to look for tutorials and examples. At that time, we did not know there was an example board on the ECE445 Web Wiki. The reason why we chose STM32 was that Lucas said it was the only one that's robust enough to do ADC/DAC. 

## WEEK 3/18/24
N/A

## WEEK 3/25/24
This week, I finished my individual progress report.

## WEEK 4/1/24
Throughout this week, I finished off the PCB layout and sucessfully placed the PCB order, which is the fourth round. I realized this was very late considering how slow PCBs are shiped. It may take another two weeks to arrive since the TA said the second round just arrived. Unbelievable.
Anyways, right before submitting the order, we have found some essential components missing. In particular, for the SWD connector, signals SWDIO, SWDCLK, and NRST all need pull-up resistors in order to work. We actually just did not know that, but Jason told us that upon reviewing our design. Here is what the overall PCB looks like:
![PCB Layout](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-04-30%20at%2011.27.07%20PM.png)

## WEEK 4/8/24
This week, we are supposed to prepare for the mock demostration. During the meeting with our TA, we were told that to have at least the physical feeder built so that we will have an idea what should be done next. We are indeed a little behind the schedule. 

## WEEK 4/15/24
This week, Lucas and I breadboarded the RFID modules as well as stepper motor with Arduino Uno. We verified that both the stepper motor and RFID works modularly. More specifically, the stepper motor recieves two squre waves (one signal leads the other by 90 degree) from the ArduinoIDE. It was lucky we were able to find code and tutorial online. We did run into some issues though. For example, we could not get the shaft rotating for about an hour because the way we connect the 5V pin was wrong. Lucas said the tutorial he found says the 5V pin should be put a jumper with, so he did. In reality, we fixed this issue by connecting it to the arduino uno 5V supply. 
The RFID module was functional, proven by sending a wave signal with the waveform genertor from the transmitter and probing the RX pin of the receiver on oscilliscope. We saw the exact same signal. What's more important is that the default communication range these RF modules provide without soldering antennas seems pretty solid. Although we did not actually measure the maximum distance at which the signal disappear from the receiver, this range is a good and appropriate proximity for the dog to get access to food. 
Besides that, we have got our PCBs but our PCB components are still missing. We have to get them ordered soon!

## WEEK 4/22/24
This week was our demo week. It has been a tough week for all of us, and we've learnt a hard lesson. During the building process, Lucas accidentally wired 12V power to the 5V rail, which damaged the STM32 nucleo development board, one reciever module, and T'Andra's laptop (at the time of wiring, the nucleo board was connected to her laptop, and 12V fried everything presumably). So we did not actually have the whole thing functional at the time of demo. Besides this tragedy, I just want to note down what we have accomplished before the demo:
  * PCB components were finally all gathered, and I solder them very carefully and neatly since we only have components to solder one copy of the PCB, mainly due to the fact that we only had one STM32 chip. Here's how the PCB looks like: ![Soldered PCB](https://github.com/LUCASDUDUIT/ECE445/blob/main/Notebooks/Kevin/Screenshot%202024-05-02%20at%209.52.12%20PM.png)
  * While building the feeder, we ran into problems with the IR sensor. The IR sensor could not read a solid logic high or low value on the program end because the IR signal received on the “OUT” pin had an amplitude of about 1.7V, which is in between 0V and 3.3V. Once the sensor is blocked by something, like hands, paper, credit cards, or sheet metal, the signal read was about 0.1V above of that when the sensor is not blocked. We have thought about two solutions to this: 1) using an operational amplifier to amplify the amplitude difference between two states; 2) reading analog values so we could distinguish them.  After some careful assessment of the difficulty of the proposed solutions, we decided to remove this feature from the feeder and move on building other important components.
  * Another drastic change that we have made was our PCB (Printed Circuit Board). After soldering the PCB, we realized the 3.3V and 5V voltage regulators footprint that we used in the original PCB design does not fit the actual components that we got. Missing the critical power management, we could not actually run the board. Since we placed our PCB order late, we could not make changes and order another version in time. Therefore, we had to adjust our design. We finished the rest of the circuitry on the breadboard and ran the program off the provided STM32 nucleo development board. 

## WEEK 4/29/24
This is last week for this course, we prepared presentation thoroughly and made up the demo, a second chance given to us.
For this demo, we had everything else working besides the stepper motor. One thing that we were unsure of was how the stepper motor functions. The lid could not open smoothly, and it makes rattling noises as the motor tries to drive the lid open. We have observed and confirmed the functionality of the stepper motor when the program is only running stepper motor functions alone. Therefore, we diagnosed that it was a mechanical issue that caused the unsmooth motion of the lid. Since we used sheet metal fabricating the receptacle and lid, the bending and shaping of the lid and receptacle presented quite of a challenge to us. 
