rem Configure environment
call C:\Xilinx\13.2\ISE_DS\settings64.bat

@rem Prepare lx45 project
@copy /y system.xmp.lx45 system.xmp
@copy /y system_incl.make.lx45 system_incl.make
@copy /y data\system.ucf.lx45 data\system.ucf
@rem Clean project
make -f system.make hwclean
@rem Generate bitstream
make -f system.make init_bram
@rem Copy result
@rem copy /y implementation\system.bit system_lx45.bit
@rem copy /y implementation\system_bd.bmm system_bd_lx45.bmm
copy /y implementation\download.bit TE0630_LX45.bit
@rem Clean project
make -f system.make hwclean

@rem Prepare lx75 project
@copy /y system.xmp.lx75 system.xmp
@copy /y system_incl.make.lx75 system_incl.make
@copy /y data\system.ucf.lx75 data\system.ucf
@rem Clean project
make -f system.make hwclean
@rem Generate bitstream
make -f system.make init_bram
@rem Copy result
@rem copy /y implementation\system.bit system_lx75.bit
@rem copy /y implementation\system_bd.bmm system_bd_lx75.bmm
copy /y implementation\download.bit TE0630_LX75.bit
@rem Clean project
make -f system.make hwclean

@rem Prepare lx100 project
@copy /y system.xmp.lx100 system.xmp
@copy /y system_incl.make.lx100 system_incl.make
@rem Clean project
make -f system.make hwclean
@rem Generate bitstream
make -f system.make init_bram
@rem Copy result
@rem copy /y implementation\system.bit system_lx100.bit
@rem copy /y implementation\system_bd.bmm system_bd_lx100.bmm
copy /y implementation\download.bit TE0630_LX100.bit
@rem Clean project
make -f system.make hwclean

@rem Prepare lx150 project
@copy /y system.xmp.lx150 system.xmp
@copy /y system_incl.make.lx150 system_incl.make
@rem Clean project
make -f system.make hwclean
@rem Generate bitstream
make -f system.make init_bram
@rem Copy result
@rem copy /y implementation\system.bit system_lx150.bit
@rem copy /y implementation\system_bd.bmm system_bd_lx150.bmm
copy /y implementation\download.bit TE0630_LX150.bit
@rem Clean project
make -f system.make hwclean
@rem Remove logs
@del *.log

@rem Making FWUs
@rem Configure environment
set XILINX=C:\Xilinx\13.2\ISE_DS\ISE
set XILINX_DSP=%XILINX%
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%
@rem Copy needed files
@copy ..\..\TE_USB_FX2.firmware\ready_for_download\current_te.iic PREPARE_FWU\usb.bin
@copy PREPARE_FWU\usb.bin .\
@copy PREPARE_FWU\Bootload.ini .\

@rem Generate FWU for LX45
@copy TE0630_LX45.bit fpga.bit
@impact -batch etc\bit2bin.cmd
@copy fpga.bin TE0630-LX45.bin
@impact -batch etc\bit2mcs.cmd
@move fpga.mcs TE0630-LX45.mcs
@PREPARE_FWU\zip -q TE0630-LX45.zip fpga.bin Bootload.ini usb.bin
@move TE0630-LX45.zip TE0630-LX45.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi fpga.bit fpga.sig

@rem Generate FWU for LX75
@copy TE0630_LX75.bit fpga.bit
@impact -batch etc\bit2bin.cmd
@copy fpga.bin TE0630-LX75.bin
@impact -batch etc\bit2mcs.cmd
@move fpga.mcs TE0630-LX75.mcs
@PREPARE_FWU\zip -q TE0630-LX75.zip fpga.bin Bootload.ini usb.bin
@move TE0630-LX75.zip TE0630-LX75.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi fpga.bit fpga.sig

@rem Generate FWU for LX100
@copy TE0630_LX100.bit fpga.bit
@impact -batch etc\bit2bin.cmd
@copy fpga.bin TE0630-LX100.bin
@impact -batch etc\bit2mcs.cmd
@move fpga.mcs TE0630-LX100.mcs
@PREPARE_FWU\zip -q TE0630-LX100.zip fpga.bin Bootload.ini usb.bin
@move TE0630-LX100.zip TE0630-LX100.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi fpga.bit fpga.sig

@rem Generate FWU for LX150
@copy TE0630_lx150.bit fpga.bit
@impact -batch etc\bit2bin.cmd
@copy fpga.bin TE0630-LX150.bin
@impact -batch etc\bit2mcs.cmd
@move fpga.mcs TE0630-LX150.mcs
@PREPARE_FWU\zip -q TE0630-LX150.zip fpga.bin Bootload.ini usb.bin
@move TE0630-LX150.zip TE0630-LX150.fwu
@rem Remove logs
@del fpga.prm fpga.cfi fpga.bit fpga.sig fpga.bin

@rem Remove files
@del usb.bin 
@del Bootload.ini
@del _impactbatch.log

pause