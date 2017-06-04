connect -url tcp:127.0.0.1:3121
source C:/Users/vivoz/Documents/WORK_FPGA/Xilinx/ver201604template/ver201604template.sdk/basicZturn_wrapper_hw_platform_1/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Platform Cable USB 00000000000000"} -index 0
loadhw C:/Users/vivoz/Documents/WORK_FPGA/Xilinx/ver201604template/ver201604template.sdk/basicZturn_wrapper_hw_platform_1/system.hdf
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Platform Cable USB 00000000000000"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Platform Cable USB 00000000000000"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Platform Cable USB 00000000000000"} -index 0
dow C:/Users/vivoz/Documents/WORK_FPGA/Xilinx/ver201604template/ver201604template.sdk/test_sd/Debug/test_sd.elf
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Platform Cable USB 00000000000000"} -index 0
con
