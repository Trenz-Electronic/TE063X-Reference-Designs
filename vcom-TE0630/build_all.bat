@rem Configure environment
set XILINX=C:\Xilinx\13.2\ISE_DS\ISE
set XILINX_DSP=%XILINX%
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%

@rem Run LX45 version build
xtclsh vcom-TE0630-LX45.tcl rebuild_project
bitgen -intstyle ise -f batch.ut top.ncd
promgen -w -u 0 top.bit -o vcom-TE0630-LX45.mcs
@del vcom-TE0630-LX45.prm vcom-TE0630-LX45.cfi
copy top.bit vcom-TE0630-LX45.bit
@del /F /q _ngo bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q top.bgn top.bit top.bld top.cmd_log top.drc top.lso top.ncd top.ngc
@del /F /q top.ngd top.ngr top.pad top.par top.pcf top.prj top.ptwx top.stx
@del /F /q top.syr top.twr top.twx top.unroutes top.xpi top.xst top_map.map
@del /F /q top_map.mrp top_map.ncd top_map.ngm top_map.xrpt top_ngdbuild.xrpt
@del /F /q top_pad.csv top_pad.txt top_par.xrpt top_summary.html top_summary.xml
@del /F /q top_usage.xml top_vhdl.prj top_xst.xrpt webtalk.log webtalk_pn.xml
@del /F /q xlnx_auto_0_xdb xst top_summary.html par_usage_statistics.html

@rem Run LX75 version build
xtclsh vcom-TE0630-LX75.tcl rebuild_project
bitgen -intstyle ise -f batch.ut top.ncd
promgen -w -u 0 top.bit -o vcom-TE0630-LX75.mcs
@del vcom-TE0630-LX75.prm vcom-TE0630-LX75.cfi
copy top.bit vcom-TE0630-LX75.bit
@del /F /q _ngo bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q top.bgn top.bit top.bld top.cmd_log top.drc top.lso top.ncd top.ngc
@del /F /q top.ngd top.ngr top.pad top.par top.pcf top.prj top.ptwx top.stx
@del /F /q top.syr top.twr top.twx top.unroutes top.xpi top.xst top_map.map
@del /F /q top_map.mrp top_map.ncd top_map.ngm top_map.xrpt top_ngdbuild.xrpt
@del /F /q top_pad.csv top_pad.txt top_par.xrpt top_summary.html top_summary.xml
@del /F /q top_usage.xml top_vhdl.prj top_xst.xrpt webtalk.log webtalk_pn.xml
@del /F /q xlnx_auto_0_xdb xst top_summary.html par_usage_statistics.html

@rem Run LX100 version build
xtclsh vcom-TE0630-LX100.tcl rebuild_project
bitgen -intstyle ise -f batch.ut top.ncd
promgen -w -u 0 top.bit -o vcom-TE0630-LX100.mcs
@del vcom-TE0630-LX100.prm vcom-TE0630-LX100.cfi
copy top.bit vcom-TE0630-LX100.bit
@del /F /q _ngo bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q top.bgn top.bit top.bld top.cmd_log top.drc top.lso top.ncd top.ngc
@del /F /q top.ngd top.ngr top.pad top.par top.pcf top.prj top.ptwx top.stx
@del /F /q top.syr top.twr top.twx top.unroutes top.xpi top.xst top_map.map
@del /F /q top_map.mrp top_map.ncd top_map.ngm top_map.xrpt top_ngdbuild.xrpt
@del /F /q top_pad.csv top_pad.txt top_par.xrpt top_summary.html top_summary.xml
@del /F /q top_usage.xml top_vhdl.prj top_xst.xrpt webtalk.log webtalk_pn.xml
@del /F /q xlnx_auto_0_xdb xst top_summary.html par_usage_statistics.html

@rem Run LX150 version build
xtclsh vcom-TE0630-LX150.tcl rebuild_project
bitgen -intstyle ise -f batch.ut top.ncd
promgen -w -u 0 top.bit -o vcom-TE0630-LX150.mcs
@del vcom-TE0630-LX150.prm vcom-TE0630-LX150.cfi
copy top.bit vcom-TE0630-LX150.bit
@del /F /q _ngo bitgen.xmsgs map.xmsgs ngdbuild.xmsgs par.xmsgs trce.xmsgs xst.xmsgs
@del /F /q top.bgn top.bit top.bld top.cmd_log top.drc top.lso top.ncd top.ngc
@del /F /q top.ngd top.ngr top.pad top.par top.pcf top.prj top.ptwx top.stx
@del /F /q top.syr top.twr top.twx top.unroutes top.xpi top.xst top_map.map
@del /F /q top_map.mrp top_map.ncd top_map.ngm top_map.xrpt top_ngdbuild.xrpt
@del /F /q top_pad.csv top_pad.txt top_par.xrpt top_summary.html top_summary.xml
@del /F /q top_usage.xml top_vhdl.prj top_xst.xrpt webtalk.log webtalk_pn.xml
@del /F /q xlnx_auto_0_xdb xst top_summary.html par_usage_statistics.html

