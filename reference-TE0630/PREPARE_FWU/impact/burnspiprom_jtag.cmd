setMode -bs
addDevice -p 1 -file "_download.bit"
attachflash -position 1 -spi "M25P32"
assignfiletoattachedflash -position 1 -file "_combined.mcs"
setCable -port auto
Program -p 1 -spionly -e -v 
exit