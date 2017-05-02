set_property SRC_FILE_INFO {cfile:C:/Users/vivoz/Documents/WORK_FPGA/Xilinx/Vivado_project_on_Zturn/with_new_board_def_file/ver201604template/ver201604template.srcs/constrs_2/new/rate.xdc rfile:../../../ver201604template.srcs/constrs_2/new/rate.xdc id:1} [current_design]
set_property src_info {type:XDC file:1 line:1 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports {sws_4bits_tri_i[*]}]
set_property src_info {type:XDC file:1 line:2 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {sws_4bits_tri_i[*]}]
set_property src_info {type:XDC file:1 line:3 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_scl_io]
set_property src_info {type:XDC file:1 line:4 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_scl_io]
set_property src_info {type:XDC file:1 line:5 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_sda_io]
set_property src_info {type:XDC file:1 line:6 export:INPUT save:INPUT read:READ} [current_design]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_sda_io]
set_property src_info {type:XDC file:1 line:7 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports {rgb_led_tri_o[*]}]
set_property src_info {type:XDC file:1 line:8 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {rgb_led_tri_o[*]}]
set_property src_info {type:XDC file:1 line:9 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports buzzer_tri_o]
set_property src_info {type:XDC file:1 line:10 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports buzzer_tri_o]
set_property src_info {type:XDC file:1 line:11 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_scl_io]
set_property src_info {type:XDC file:1 line:12 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_scl_io]
set_property src_info {type:XDC file:1 line:13 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_sda_io]
set_property src_info {type:XDC file:1 line:14 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_sda_io]
set_property src_info {type:XDC file:1 line:17 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.200 [get_ports {pwm[*]}]
set_property src_info {type:XDC file:1 line:18 export:INPUT save:INPUT read:READ} [current_design]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {pwm[*]}]
set_property src_info {type:XDC file:1 line:25 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN M15 [get_ports {rgb_led_tri_o[1]}]
set_property src_info {type:XDC file:1 line:26 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN K16 [get_ports {rgb_led_tri_o[2]}]
set_property src_info {type:XDC file:1 line:29 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -from [get_clocks clk_fpga_0] -to [get_clocks clk_fpga_0] 20.000
set_property src_info {type:XDC file:1 line:30 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -from [get_clocks clk_fpga_0] -to [get_clocks clk_fpga_0] 20.000
set_property src_info {type:XDC file:1 line:33 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN M14 [get_ports {rgb_led_tri_o[0]}]
set_property src_info {type:XDC file:1 line:36 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN Y16 [get_ports {pwm[0]}]
set_property src_info {type:XDC file:1 line:37 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN Y17 [get_ports {pwm[1]}]
set_property src_info {type:XDC file:1 line:38 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN R14 [get_ports {pwm[2]}]