# Cookie Detection and Removal System

This project uses a **Raspberry Pi** and **TI C2000 microcontroller** to detect and sort cookies on a conveyor belt. Cookies are identified based on their color. If an underbaked (white) cookie is detected the system commands a robotic arm to pick it up and remove it.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)

## Project Overview
This system combines image processing, real-time data transmission, and robotics to detect and remove underbaked cookies from a conveyor belt. A Raspberry Pi identifies cookies based on color and position and communicates their coordinates to a TI C2000 microcontroller. If a white (underbaked) cookie reaches the center, the TI C2000 commands a robotic arm to remove it from the belt.

## Features
- **Color and Center Detection**: Uses OpenCV on a Raspberry Pi to detect the center and color of each cookie.
- **Position Transmission**: Sends the detected center coordinates to a TI C2000 microcontroller via SCI.
- **Robotic Arm Control**: Commands a robotic arm to pick up white (underbaked) cookies when they reach the center.

## Hardware Requirements
- **Raspberry Pi** (e.g., Raspberry Pi 4)
- **TI C2000 Microcontroller**
- Conveyor Belt System
- Camera (compatible with Raspberry Pi)
- **Robotic Arm** (capable of receiving commands from TI C2000)
- Cookies (or test objects with distinguishable color)

## Software Requirements
- **Python** 3.x
- **OpenCV** (version 4.10 or compatible)
- **TI Code Composer Studio** for RTOS development
- **RTOS** support on TI C2000

## Installation and Setup
1. **Raspberry Pi Setup**:
    - Install Python and OpenCV on the Raspberry Pi.
    - Clone this repository:
      ```bash
      git clone https://github.com/TUBTUB34/cookie-detection.git
      ```
    - Navigate to the project directory:
      ```bash
      cd cookie-detection
      ```

2. **TI C2000 Setup**:
    - Open the project in **TI Code Composer Studio**.
    - Build and upload the RTOS code to the C2000.

## Demo Video
**Initial test of the system**

![Demo Video](first_test.gif)

## Usage
1. Start the Python script on the Raspberry Pi:
    ```bash
    python CookieProjectPi.py
    ```
2. The Raspberry Pi camera will detect cookies, identify their center, and send their coordinates via SCI to the TI C2000.
3. When a white cookie is centered on the conveyor, the TI C2000 microcontroller sends a command to the robotic arm to pick up and remove the cookie from the conveyor.


