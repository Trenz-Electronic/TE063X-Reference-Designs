echo Created by Costantino Grana - University of Modena and Reggio Emilia, Italy
echo Edited by Ales Gorkic - Optomotive d.o.o., Slovenia

@echo off

REM Check if three parameters are given

if "%3"=="" goto errorParameters

REM Check if files exist

if not exist %1 goto errorNotExist1
if not exist %2 goto errorNotExist2

REM Set filenames for scripts

set program_elf=_program.elf
set fpga_config_bit=_download.bit
set boot_addr=%3

REM Copy user files to right filenames

for %%d in (%1,%2) do (call :copy_files %%d)

if %errorlevel%==-1 goto fine

rem **************************************************
rem * 			main
rem **************************************************

call :build_all

goto fine

rem **************************************************

:errorParameters
echo Error: You need to provide two filenames with extensions .ELF, .BIT and a booting DDR address
goto :eof

:errorNotExist1
echo Error: file %1 does not exist
goto :eof

:errorNotExist2
echo Error: file %2 does not exist
goto :eof

:errorNotExist3
echo Error: address %3 does not exist
goto :eof

:errorExtension
echo Filename format error (missing exstension)
goto :eof

rem **************************************************
rem * copy_files
rem **************************************************
:copy_files
 set errorlevel=0
 set var=%1 
 set str=%var:~-4%
 if %str%==elf goto copy_elf
 if %str%==bit goto copy_bit

 set errorlevel=-1
 goto :errorExtension

:copy_elf
 copy %1 %program_elf%
 goto :eof
:copy_bit
 copy %1 %fpga_config_bit%
 goto :eof
rem **************************************************

rem **************************************************
rem * build_all
rem **************************************************
:build_all
 if exist _combined.mcs del _combined.mcs
 if exist _fpga_bootloader.mcs del _fpga_bootloader.mcs

rem **************************************************
rem * change the bitstream to MCS
rem **************************************************
 impact -batch impact\bit2mcs.cmd


rem **************************************************
rem * remove the vectors table from the executable and save as binary
rem **************************************************
 bin\objcopy -I elf32-big -O binary -R .vectors.reset -R .vectors.sw_exception -R .vectors.interrupt -R .vectors.debug_sw_break -R .vectors.hw_exception %program_elf% executable.bin


rem **************************************************
rem * save vectors table to binary files
rem **************************************************
 bin\objcopy -I elf32-big -O binary -j .vectors.reset -j .vectors.sw_exception -j .vectors.interrupt -j .vectors.hw_exception %program_elf% vectors.bin


rem **************************************************
rem * insert header to executable binary
rem **************************************************
 bin\prepare_bootdata %boot_addr%

rem **************************************************
rem * convert all flash files to MCS and combine in single file
rem **************************************************
 bin\xmcsutil -accept_notice -29 -i bootdata.bin -o _bootdata.mcs
 bin\xmcsutil -accept_notice -16 -i _fpga_bootloader.mcs _bootdata.mcs -o _combined.mcs -segaddr 0x00 0x100000 -usedataaddr -padff

rem **************************************************
rem * build flash binary and pack FWU
rem **************************************************
 bin\xmcsutil -accept_notice -1 -i _combined.mcs -o _combined.hex
 bin\xmcsutil -accept_notice -18 -i _combined.hex -o fpga.bin

 copy bin\usb.bin usb.bin
 copy bin\Bootload.ini Bootload.ini
 zip firmware.fwu fpga.bin Bootload.ini usb.bin
 del usb.bin
 del Bootload.ini

rem **************************************************
rem * burn_flash through JTAG
rem **************************************************
 impact -batch impact\burnspiprom_jtag.cmd

rem goto :eof

 del _combined.mcs
 del _combined.hex
 del fpga.bin
 del executable.bin
 del vectors.bin
 del bootdata.bin
 del _bootdata.mcs
 del _download_cclktemp.bit
 del _fpga_bootloader.mcs
 del _fpga_bootloader.prm
 del _impactbatch.log
 del _fpga_bootloader.cfi

 goto :eof
rem **************************************************

:fine

REM free variables space

if exist %program_elf% del %program_elf%
if exist %fpga_config_bit% del %fpga_config_bit%

set program_elf=
set fpga_config_bit=
set var=
set str=
