@echo off


rem **************************************************
rem * build_all
rem **************************************************
 
 impact -batch bin\bit2bin.cmd


rem **************************************************
rem * pack FWU
rem **************************************************
 copy bin\Bootload_swap.ini Bootload.ini
 copy bin\usb.bin usb.bin

 if exist FIRMWARE.FWU del FIRMWARE.FWU

 if exist fpga.bin zip firmware.fwu fpga.bin Bootload.ini usb.bin

 del _impactbatch.log
 del fpga.cfi
 del fpga.prm
 del fpga.sig
 del fpga.bin
 del Bootload.ini
 del usb.bin
