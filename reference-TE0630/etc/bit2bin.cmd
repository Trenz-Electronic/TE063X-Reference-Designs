setMode -pff
setSubmode -pffserial
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -p 1 -file "fpga.bit"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr autoSize -value "TRUE"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setAttribute -configdevice -attr flashDataWidth -value "8"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr swapBit -value "TRUE"
setAttribute -configdevice -attr fileFormat -value "bin"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value ".\/"
setAttribute -configdevice -attr name -value "fpga"
generate -generic
exit
