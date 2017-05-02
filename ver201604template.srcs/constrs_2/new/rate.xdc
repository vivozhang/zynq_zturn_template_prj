set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports {sws_4bits_tri_i[*]}]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {sws_4bits_tri_i[*]}]
set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_scl_io]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_scl_io]
set_input_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_sda_io]
set_input_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_sda_io]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports {rgb_led_tri_o[*]}]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {rgb_led_tri_o[*]}]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports buzzer_tri_o]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports buzzer_tri_o]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_scl_io]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_scl_io]
set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.100 [get_ports i2c0_sda_io]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports i2c0_sda_io]


set_output_delay -clock [get_clocks clk_fpga_0] -min -add_delay 0.200 [get_ports {pwm[*]}]
set_output_delay -clock [get_clocks clk_fpga_0] -max -add_delay 0.100 [get_ports {pwm[*]}]

set_property IOSTANDARD LVCMOS33 [get_ports {pwm[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pwm[2]}]


set_property PACKAGE_PIN M15 [get_ports {rgb_led_tri_o[1]}]
set_property PACKAGE_PIN K16 [get_ports {rgb_led_tri_o[2]}]


set_max_delay -from [get_clocks clk_fpga_0] -to [get_clocks clk_fpga_0] 20.000
set_max_delay -from [get_clocks clk_fpga_0] -to [get_clocks clk_fpga_0] 20.000


set_property PACKAGE_PIN M14 [get_ports {rgb_led_tri_o[0]}]


set_property PACKAGE_PIN Y16 [get_ports {pwm[0]}]
set_property PACKAGE_PIN Y17 [get_ports {pwm[1]}]
set_property PACKAGE_PIN R14 [get_ports {pwm[2]}]
