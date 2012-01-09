setMode -spi
addDevice -p 1 -spi M25P32 -file "_combined.mcs"
setCable -port auto
Program -p 1 -e -v 
exit