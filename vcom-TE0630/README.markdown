# Virtual COM port interface sample project
## General Description
Project made for Trenz Electronic USB equipped modules.
This project show communication between FPGA and Cypress FX2 microcontroller. 
FX2 Microcontroller use custom firmware, based on Cypress application 
note AN58764. 

## Requirements:
* Xilinx ISE 13.2 

## Build projects
All projects contain already implemented result files ready to download.  

* To build ISE projects - Open project for your module type in "Project Navigator"
and run "Generate Programming File"
* To build project for all FPGA variants, run build_all.bat in projet directory.

## Driver
Project use example virtual COM port driver from AN58764. Driver inf file 
located in driver folder.

## FX2 Firmware
FX2 firmware file in iic format can be found in firmware folder. To update FX2 
firmware on your module:

* Put EEPROM switch on your module to "OFF" state.
* Connect module to PC, using USB cable.
* Install Cypress generic driver if needed.
* Put EEPROM switch on your module to "ON" state.
* Run Cypress USB Console.
* Go Options->EZ-USB Interface
* Press "Lg EEPROM" button
* Select VirtualCom.iic from firmware folder
* Wait till operation completed
* Reconnect module from USB
* Install Cypress Virtual COM port driver if needed

## Detailed description
Custom firmware used for FX2 microcontroller used Virtual COM port driver. COM 
port interface give user simple and convenient way to communicate with module.
From FPGA side FX2 microcontroller configured to use Slave FIFO interface. Data
from PC is going to EP2 FIFO, data from FPGA should be writed to EP8 FIFO. Flags
pins used to show state of this FIFO:

* Flag A - EP2 Empty Flag
* Flag B - EP8 Full Flag
* Flag C - EP8 Programmable Full Flag (show half of the buffer level)
* Flag D - Receive indicator - this flag toggle each time FX2 receive byte from PC

All signals configured to active high level.
Example FPGA project act as:

* Read data from EP2 when Flag A show that EP2 FIFO is not empty.
* Process data (simple increment received byte).
* Send data to EP8 when Flag B show that EP8 FIFO is not full.
