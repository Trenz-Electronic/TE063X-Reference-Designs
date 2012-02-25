rem Configure environment
set XILINX=C:\Xilinx\13.2\ISE_DS\ISE
set XILINX_DSP=%XILINX%
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%

rem Run 45 version build
xtclsh blinkin-TE0630-45.tcl rebuild_project
rem bitgen -intstyle ise -f blinkin.ut blinkin.ncd
promgen -w -u 0 blinkin.bit -o TE0630-45.mcs
promgen -w -p bin -u 0 blinkin.bit -o fpga.bin
copy blinkin.bit TE0630-45.bit
zip -q TE0630-45.zip fpga.bin Bootload.ini usb.bin
move TE0630-45.zip TE0630-45.fwu

rem Clean
@del /F /q fpga.bin fpga.cfi fpga.prm
@del /F /q TE0630-45.cfi TE0630-45.prm
@del /F /q bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q blinkin.bgn blinkin.bit blinkin.bld blinkin.cmd_log blinkin.drc blinkin.lso
@del /F /q blinkin.ncd blinkin.ngc blinkin.ngd blinkin.ngr blinkin.pad blinkin.par
@del /F /q blinkin.pcf blinkin.prj blinkin.ptwx blinkin.stx blinkin.syr blinkin.twr
@del /F /q blinkin.twx blinkin.unroutes blinkin.xpi blinkin.xst
@del /F /q blinkin_map.map blinkin_map.mrp blinkin_map.ncd blinkin_map.ngm
@del /F /q blinkin_map.xrpt blinkin_ngdbuild.xrpt blinkin_pad.csv blinkin_pad.txt
@del /F /q blinkin_par.xrpt blinkin_summary.html blinkin_summary.xml blinkin_usage.xml
@del /F /q blinkin_vhdl.prj blinkin_xst.xrpt webtalk.log webtalk_pn.xml
@rmdir /S /Q xst _ngo xlnx_auto_0_xdb
@del /F /q blinkin_summary.html blinkin_guide.ncd

rem Run 75 version build
xtclsh blinkin-TE0630-75.tcl rebuild_project
rem bitgen -intstyle ise -f blinkin.ut blinkin.ncd
promgen -w -u 0 blinkin.bit -o TE0630-75.mcs
promgen -w -p bin -u 0 blinkin.bit -o fpga.bin
copy blinkin.bit TE0630-75.bit
zip -q TE0630-75.zip fpga.bin Bootload.ini usb.bin
move TE0630-75.zip TE0630-75.fwu

rem Clean
@del /F /q fpga.bin fpga.cfi fpga.prm
@del /F /q TE0630-75.cfi TE0630-75.prm
@del /F /q bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q blinkin.bgn blinkin.bit blinkin.bld blinkin.cmd_log blinkin.drc blinkin.lso
@del /F /q blinkin.ncd blinkin.ngc blinkin.ngd blinkin.ngr blinkin.pad blinkin.par
@del /F /q blinkin.pcf blinkin.prj blinkin.ptwx blinkin.stx blinkin.syr blinkin.twr
@del /F /q blinkin.twx blinkin.unroutes blinkin.xpi blinkin.xst
@del /F /q blinkin_map.map blinkin_map.mrp blinkin_map.ncd blinkin_map.ngm
@del /F /q blinkin_map.xrpt blinkin_ngdbuild.xrpt blinkin_pad.csv blinkin_pad.txt
@del /F /q blinkin_par.xrpt blinkin_summary.html blinkin_summary.xml blinkin_usage.xml
@del /F /q blinkin_vhdl.prj blinkin_xst.xrpt webtalk.log webtalk_pn.xml
@rmdir /S /Q xst _ngo xlnx_auto_0_xdb
@del /F /q blinkin_summary.html blinkin_guide.ncd

rem Run 100 version build
xtclsh blinkin-TE0630-100.tcl rebuild_project
rem bitgen -intstyle ise -f blinkin.ut blinkin.ncd
promgen -w -u 0 blinkin.bit -o TE0630-100.mcs
promgen -w -p bin -u 0 blinkin.bit -o fpga.bin
copy blinkin.bit TE0630-100.bit
zip -q TE0630-100.zip fpga.bin Bootload.ini usb.bin
move TE0630-100.zip TE0630-100.fwu

rem Clean
@del /F /q fpga.bin fpga.cfi fpga.prm
@del /F /q TE0630-100.cfi TE0630-100.prm
@del /F /q bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q blinkin.bgn blinkin.bit blinkin.bld blinkin.cmd_log blinkin.drc blinkin.lso
@del /F /q blinkin.ncd blinkin.ngc blinkin.ngd blinkin.ngr blinkin.pad blinkin.par
@del /F /q blinkin.pcf blinkin.prj blinkin.ptwx blinkin.stx blinkin.syr blinkin.twr
@del /F /q blinkin.twx blinkin.unroutes blinkin.xpi blinkin.xst
@del /F /q blinkin_map.map blinkin_map.mrp blinkin_map.ncd blinkin_map.ngm
@del /F /q blinkin_map.xrpt blinkin_ngdbuild.xrpt blinkin_pad.csv blinkin_pad.txt
@del /F /q blinkin_par.xrpt blinkin_summary.html blinkin_summary.xml blinkin_usage.xml
@del /F /q blinkin_vhdl.prj blinkin_xst.xrpt webtalk.log webtalk_pn.xml
@rmdir /S /Q xst _ngo xlnx_auto_0_xdb
@del /F /q blinkin_summary.html blinkin_guide.ncd

rem Run 150 version build
xtclsh blinkin-TE0630-150.tcl rebuild_project
rem bitgen -intstyle ise -f blinkin.ut blinkin.ncd
promgen -w -u 0 blinkin.bit -o TE0630-150.mcs
promgen -w -p bin -u 0 blinkin.bit -o fpga.bin
copy blinkin.bit TE0630-150.bit
zip -q TE0630-150.zip fpga.bin Bootload.ini usb.bin
move TE0630-150.zip TE0630-150.fwu

rem Clean
@del /F /q fpga.bin fpga.cfi fpga.prm
@del /F /q TE0630-150.cfi TE0630-150.prm
@del /F /q bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q blinkin.bgn blinkin.bit blinkin.bld blinkin.cmd_log blinkin.drc blinkin.lso
@del /F /q blinkin.ncd blinkin.ngc blinkin.ngd blinkin.ngr blinkin.pad blinkin.par
@del /F /q blinkin.pcf blinkin.prj blinkin.ptwx blinkin.stx blinkin.syr blinkin.twr
@del /F /q blinkin.twx blinkin.unroutes blinkin.xpi blinkin.xst
@del /F /q blinkin_map.map blinkin_map.mrp blinkin_map.ncd blinkin_map.ngm
@del /F /q blinkin_map.xrpt blinkin_ngdbuild.xrpt blinkin_pad.csv blinkin_pad.txt
@del /F /q blinkin_par.xrpt blinkin_summary.html blinkin_summary.xml blinkin_usage.xml
@del /F /q blinkin_vhdl.prj blinkin_xst.xrpt webtalk.log webtalk_pn.xml
@rmdir /S /Q xst _ngo xlnx_auto_0_xdb
@del /F /q blinkin_summary.html blinkin_guide.ncd

@del /F /q blinkin_summary.html par_usage_statistics.html

