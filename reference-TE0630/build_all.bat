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
copy /y implementation\system.bit system_lx45.bit
copy /y implementation\system_bd.bmm system_bd_lx45.bmm
copy /y implementation\download.bit download_lx45.bit
promgen -w -u 0 download_lx45.bit -o system_lx45.mcs
rm system_lx45.prm system_lx45.cfi
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
copy /y implementation\system.bit system_lx75.bit
copy /y implementation\system_bd.bmm system_bd_lx75.bmm
copy /y implementation\download.bit download_lx75.bit
promgen -w -u 0 download_lx75.bit -o system_lx75.mcs
rm system_lx75.prm system_lx75.cfi
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
copy /y implementation\system.bit system_lx100.bit
copy /y implementation\system_bd.bmm system_bd_lx100.bmm
copy /y implementation\download.bit download_lx100.bit
promgen -w -u 0 download_lx100.bit -o system_lx100.mcs
rm system_lx100.prm system_lx100.cfi
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
copy /y implementation\system.bit system_lx150.bit
copy /y implementation\system_bd.bmm system_bd_lx150.bmm
copy /y implementation\download.bit download_lx150.bit
promgen -w -u 0 download_lx150.bit -o system_lx150.mcs
rm system_lx150.prm system_lx150.cfi
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
copy PREPARE_FWU\bin\usb.bin .\
copy PREPARE_FWU\bin\Bootload.ini .\

@rem Generate FWU for LX45
promgen -w -p bin -u 0 download_lx45.bit -o fpga.bin
zip -q TE0630-45.zip fpga.bin Bootload.ini usb.bin
move TE0630-45.zip TE0630-45.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi

@rem Generate FWU for LX75
promgen -w -p bin -u 0 download_lx75.bit -o fpga.bin
zip -q TE0630-75.zip fpga.bin Bootload.ini usb.bin
move TE0630-75.zip TE0630-75.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi

@rem Generate FWU for LX100
promgen -w -p bin -u 0 download_lx100.bit -o fpga.bin
zip -q TE0630-100.zip fpga.bin Bootload.ini usb.bin
move TE0630-100.zip TE0630-100.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi

@rem Generate FWU for LX150
promgen -w -p bin -u 0 download_lx150.bit -o fpga.bin
zip -q TE0630-150.zip fpga.bin Bootload.ini usb.bin
move TE0630-150.zip TE0630-150.fwu
@rem Remove logs
@del fpga.bin fpga.prm fpga.cfi

@rem Remove files
@del usb.bin 
@del Bootload.ini

pause