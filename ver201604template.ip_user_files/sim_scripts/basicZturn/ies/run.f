-makelib ies/xil_defaultlib -sv \
  "C:/Xilinx/Vivado/2016.4/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
-endlib
-makelib ies/xpm \
  "C:/Xilinx/Vivado/2016.4/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies/processing_system7_bfm_v2_0_5 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/7dd0/hdl/processing_system7_bfm_v2_0_vl_rfs.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_processing_system7_0_0/sim/basicZturn_processing_system7_0_0.v" \
-endlib
-makelib ies/axi_lite_ipif_v3_0_4 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/0ba0/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \
-endlib
-makelib ies/lib_cdc_v1_0_2 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/52cb/hdl/lib_cdc_v1_0_rfs.vhd" \
-endlib
-makelib ies/interrupt_control_v3_1_4 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/e956/hdl/interrupt_control_v3_1_vh_rfs.vhd" \
-endlib
-makelib ies/axi_gpio_v2_0_13 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/4f16/hdl/axi_gpio_v2_0_vh_rfs.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_axi_gpio_0_0/sim/basicZturn_axi_gpio_0_0.vhd" \
-endlib
-makelib ies/proc_sys_reset_v5_0_10 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/04b4/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_rst_ps7_0_100M_0/sim/basicZturn_rst_ps7_0_100M_0.vhd" \
  "../../../bd/basicZturn/ip/basicZturn_axi_gpio_1_0/sim/basicZturn_axi_gpio_1_0.vhd" \
-endlib
-makelib ies/generic_baseblocks_v2_1_0 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/7ee0/hdl/generic_baseblocks_v2_1_vl_rfs.v" \
-endlib
-makelib ies/axi_infrastructure_v1_1_0 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/7e3a/hdl/axi_infrastructure_v1_1_vl_rfs.v" \
-endlib
-makelib ies/axi_register_slice_v2_1_11 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/0b6b/hdl/axi_register_slice_v2_1_vl_rfs.v" \
-endlib
-makelib ies/fifo_generator_v13_1_3 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/564d/simulation/fifo_generator_vlog_beh.v" \
-endlib
-makelib ies/fifo_generator_v13_1_3 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/564d/hdl/fifo_generator_v13_1_rfs.vhd" \
-endlib
-makelib ies/fifo_generator_v13_1_3 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/564d/hdl/fifo_generator_v13_1_rfs.v" \
-endlib
-makelib ies/axi_data_fifo_v2_1_10 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/39ba/hdl/axi_data_fifo_v2_1_vl_rfs.v" \
-endlib
-makelib ies/axi_crossbar_v2_1_12 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/896d/hdl/axi_crossbar_v2_1_vl_rfs.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_xbar_0/sim/basicZturn_xbar_0.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_axi_gpio_2_0/sim/basicZturn_axi_gpio_2_0.vhd" \
-endlib
-makelib ies/lib_pkg_v1_0_2 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/832a/hdl/lib_pkg_v1_0_rfs.vhd" \
-endlib
-makelib ies/axi_iic_v2_0_14 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/efc9/hdl/axi_iic_v2_0_vh_rfs.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_axi_iic_0_0/sim/basicZturn_axi_iic_0_0.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/hdl/basicZturn.v" \
-endlib
-makelib ies/xil_defaultlib -sv \
  "../../../bd/basicZturn/ipshared/eddc/hdl/PWM_AXI.sv" \
  "../../../bd/basicZturn/ipshared/eddc/hdl/PWM_v2_0.sv" \
  "../../../bd/basicZturn/ip/basicZturn_PWM_0_0/sim/basicZturn_PWM_0_0.sv" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ver201604template/addition_main.v" \
  "../../../bd/basicZturn/ipshared/2080/hdl/addition_v1_0_S00_AXI.v" \
  "../../../bd/basicZturn/ipshared/2080/hdl/addition_v1_0.v" \
  "../../../bd/basicZturn/ip/basicZturn_addition_0_0/sim/basicZturn_addition_0_0.v" \
-endlib
-makelib ies/axi_protocol_converter_v2_1_11 \
  "../../../../ver201604template.srcs/sources_1/bd/basicZturn/ipshared/df1b/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/basicZturn/ip/basicZturn_auto_pc_0/sim/basicZturn_auto_pc_0.v" \
-endlib
-makelib ies/xil_defaultlib \
  glbl.v
-endlib

