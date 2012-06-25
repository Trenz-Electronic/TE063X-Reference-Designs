##############################################################################
## Filename:          C:\Projects\Trenz\repos\TE063X-Reference-Designs\axi-TE0630/drivers/axi_fx2_v1_00_a/data/axi_fx2_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Jun 25 15:20:38 2012 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "axi_fx2" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
