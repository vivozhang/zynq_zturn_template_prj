# 
# Synthesis run script generated by Vivado
# 

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7z020clg400-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir C:/ver201604template/ver201604template.cache/wt [current_project]
set_property parent.project_path C:/ver201604template/ver201604template.xpr [current_project]
set_property XPM_LIBRARIES XPM_CDC [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property board_part myir.com:mys-7z020:part0:2.1 [current_project]
set_property ip_repo_paths {
  c:/Users/vivoz/Documents/WORK_FPGA/Xilinx/Vivado_project_on_Zturn/with_new_board_def_file/ip_repo/addition_1.0
  c:/Users/vivoz/Documents/WORK_FPGA/Xilinx/Vivado_project_on_Zturn/with_new_board_def_file/ip_repo/myip_add_1.0
  c:/Xilinx/Vivado/2016.4/data/ip/partners/diligent
} [current_project]
set_property ip_output_repo c:/ver201604template/ver201604template.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
read_verilog -library xil_defaultlib C:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/hdl/basicZturn_wrapper.v
add_files C:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/basicZturn.bd
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_processing_system7_0_0/basicZturn_processing_system7_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_0_0/basicZturn_axi_gpio_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_0_0/basicZturn_axi_gpio_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_0_0/basicZturn_axi_gpio_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_rst_ps7_0_100M_0/basicZturn_rst_ps7_0_100M_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_rst_ps7_0_100M_0/basicZturn_rst_ps7_0_100M_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_rst_ps7_0_100M_0/basicZturn_rst_ps7_0_100M_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_1_0/basicZturn_axi_gpio_1_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_1_0/basicZturn_axi_gpio_1_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_1_0/basicZturn_axi_gpio_1_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_xbar_0/basicZturn_xbar_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_2_0/basicZturn_axi_gpio_2_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_2_0/basicZturn_axi_gpio_2_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_gpio_2_0/basicZturn_axi_gpio_2_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_iic_0_0/basicZturn_axi_iic_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_iic_0_0/basicZturn_axi_iic_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_uartlite_0_0/basicZturn_axi_uartlite_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_uartlite_0_0/basicZturn_axi_uartlite_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_axi_uartlite_0_0/basicZturn_axi_uartlite_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_xadc_wiz_0_0/basicZturn_xadc_wiz_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_xadc_wiz_0_0/basicZturn_xadc_wiz_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_auto_pc_0/basicZturn_auto_pc_0_ooc.xdc]
set_property used_in_implementation false [get_files -all C:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/basicZturn_ooc.xdc]
set_property is_locked true [get_files C:/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/basicZturn.bd]

foreach dcp [get_files -quiet -all *.dcp] {
  set_property used_in_implementation false $dcp
}
read_xdc C:/ver201604template/ver201604template.srcs/constrs_2/new/rate.xdc
set_property used_in_implementation false [get_files C:/ver201604template/ver201604template.srcs/constrs_2/new/rate.xdc]

read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]

synth_design -top basicZturn_wrapper -part xc7z020clg400-1


write_checkpoint -force -noxdef basicZturn_wrapper.dcp

catch { report_utilization -file basicZturn_wrapper_utilization_synth.rpt -pb basicZturn_wrapper_utilization_synth.pb }
