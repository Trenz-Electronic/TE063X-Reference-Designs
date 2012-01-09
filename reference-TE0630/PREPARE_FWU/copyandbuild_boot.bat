echo Created by Costantino Grana - University of Modena and Reggio Emilia, Italy
echo Edited by Ales Gorkic - Optomotive d.o.o., Slovenia

copy ..\implementation\download.bit .
copy ..\sw\demo.elf .

call build_boot download.bit demo.elf 0x1C000000

del download.bit
del demo.elf
