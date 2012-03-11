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
@rem Clean project
make -f system.make hwclean
@rem Remove logs
@del *.log
