# AI-Driven Biomimetic Flower
A robotic flower that responds to stimuli in real time.

## How It Works
The flower reacts to stimuli through 3 main components: 
1. AI Model - detects a flame using an object detection model 
2. Bluetooth Interface - connects NVIDIA Jetson AGX Orin Computer and STM32 microcontroller
3. Motor Control - drives servo-based motion to bloom or close the flower

### AI Application
The NVIDIA Jetson AGX Orin Developer Kit enables high-performance AI applications. Using the Jetson Inference libraries, I trained an object detection model on the Open Images candle dataset. To fully take advantage of the Jetson AGX Orin’s capabilities, the model was trained using CUDA acceleration for faster convergence. To ensure robustness, the model only classifies a flame if the detection confidence is 80%. 

The inference runs inside a Docker container, minimizing setup complexity and ensuring portability.

### Bluetooth Interface
Bluetooth was chosen for communication between the NVIDIA Jetson AGX Orin and STM32 microcontroller, as only a short-range connection was needed and I already had an HC-05 module available. (Wi-Fi would be more suitable for long-range applications.)

Because the Docker container did not support Bluetooth, I ran the Bluetooth process outside the container. Using tmux, I opened 2 concurrent sessions:
- Session 1: ran the object detection and wrote either ‘C’ (close) or ‘B’ (bloom) to a text file
- Session 2: read the text file and transmit the command to the STM32 via Bluetooth

Since both sessions work simultaneously, updating every 1ms, the transmission occurs in near real time. 

### Motor Control
The STM32 microcontroller drives an MG90S servo motor with ~180° range of motion. A UART connection enables Bluetooth communication through the HC-05 module.

The motor control is fully interrupt-based:
- When the STM32 receives either C or B, the USART3_IRQHandler is triggered.
- The interrupt handler reads from the USART3->RDR register and passes the value to UART_motor_control.
- UART_motor_control sets the servo to the corresponding position, creating the effect of a blooming or closing flower.

Servo motors use PWM signals to determine their range of motion. The PWM signal for the servo is generated using TIM2. The timer is configured with a prescaler of 79 (giving a 1 MHz clock), an auto-reload period of 20,000 (for a 20 ms period / 50 Hz PWM), and standard up-counting mode. The duty cycle (CCR value) is adjusted to set the servo angle.

## Building the Flower
The physical design was made to look realistic to inspire a sense of awe. I combined 3D-printed parts with artificial petals from Michaels.

As 3D modeling was new to me, this stage required multiple iterations. Specifically, four design versions were created before achieving a functional prototype.

Petal Gear Design:
<img width="2146" height="1196" alt="image" src="https://github.com/user-attachments/assets/b47e656f-402b-4d13-b4ac-e1a2cf0f067b" />

Gear Rack:
<img width="1511" height="1203" alt="image" src="https://github.com/user-attachments/assets/68d9700f-e702-4f32-b5de-7ff89bd31129" />

Gear for MG90S Motor:
<img width="1032" height="1070" alt="image" src="https://github.com/user-attachments/assets/72907a5d-c503-4a65-ba19-d80d4bdac28f" />

Base Container:
<img width="1705" height="1256" alt="Screenshot 2025-08-20 015105" src="https://github.com/user-attachments/assets/7edfee57-ab33-4dc5-9411-42c50ac0a44e" />

Full assembly (3D Mockup):
<img width="2013" height="1095" alt="image" src="https://github.com/user-attachments/assets/59b17230-a9e8-40f6-9e99-aa8d3259c17e" />

## Tools
- NVIDIA Jetson AGX Orin Developer Kit 
- Wireless camera
- STM32 microcontroller
- MG90S servo motor
- HC-05 Bluetooth module
- Kiel Vision (for STM32 programming)
- Fusion 360 (for 3D modeling)

