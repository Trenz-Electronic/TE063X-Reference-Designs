proc run_coregen {mhsinst} {
 set ngcfolder "./pcores/axi_fx2_v1_00_a/netlist"
 set tx_fifo_ngcfile $ngcfolder/tx_fifo.ngc
 set rx_fifo_ngcfile $ngcfolder/rx_fifo.ngc
 set cgpfile $ngcfolder/coregen.cgp
 set tx_fifo_xcofile $ngcfolder/tx_fifo.xco
 set rx_fifo_xcofile $ngcfolder/rx_fifo.xco
 set tx_fifo_ngcexists [file exists $tx_fifo_ngcfile]
 set rx_fifo_ngcexists [file exists $rx_fifo_ngcfile]
 if {$tx_fifo_ngcexists != 1} {
  puts "axi_fx2: Generating tx_fifo netlists using Coregen\n"
  set result [catch {exec coregen -p $cgpfile -b $tx_fifo_xcofile -intstyle xflow}]
 }
 if {$rx_fifo_ngcexists != 1} {
  puts "axi_fx2: Generating rx_fifo netlists using Coregen\n"
  set result [catch {exec coregen -p $cgpfile -b $rx_fifo_xcofile -intstyle xflow}]
 }
}
