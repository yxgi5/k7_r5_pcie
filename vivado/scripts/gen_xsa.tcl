set project_path [lindex $argv 0]
set project_name [lindex $argv 1]
set bd_filename [lindex $argv 2]
 
set BD_name [file rootname [file tail $bd_filename]]

 
open_project $project_path/$project_name
 
if {[get_property PROGRESS [get_runs impl_1]] != "100%"} {
  launch_runs impl_1 -to_step write_bitstream -jobs 20
  wait_on_run impl_1
  open_run impl_1
  report_timing_summary
}
 
open_run [get_runs impl_1]

write_debug_probes -force ${BD_name}_wrapper.ltx
write_hw_platform -verbose -fixed -include_bit -force -file ${BD_name}_wrapper.xsa
open_bd_design [get_files $project_path/$project_name.srcs/sources_1/bd/$BD_name/$BD_name.bd]
write_cfgmem -force -format mcs -size 32 -interface SPIx4 -loadbit [subst {up 0x00000000 "${project_path}/vivado_proj.runs/impl_1/system_wrapper.bit" }] -file "$project_path/../PCIE_XDMA.mcs"

close_project
