-- Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2016.4 (win64) Build 1756540 Mon Jan 23 19:11:23 MST 2017
-- Date        : Fri Apr 21 08:03:51 2017
-- Host        : DESKTOP-UTEFEFF running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               c:/Users/vivoz/Documents/WORK_FPGA/Xilinx/Vivado_project_on_Zturn/with_new_board_def_file/ver201604template/ver201604template.srcs/sources_1/bd/basicZturn/ip/basicZturn_PWM_0_0/basicZturn_PWM_0_0_stub.vhdl
-- Design      : basicZturn_PWM_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity basicZturn_PWM_0_0 is
  Port ( 
    pwm : out STD_LOGIC_VECTOR ( 2 downto 0 );
    pwm_axi_awaddr : in STD_LOGIC_VECTOR ( 6 downto 0 );
    pwm_axi_awprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    pwm_axi_awvalid : in STD_LOGIC;
    pwm_axi_awready : out STD_LOGIC;
    pwm_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    pwm_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
    pwm_axi_wvalid : in STD_LOGIC;
    pwm_axi_wready : out STD_LOGIC;
    pwm_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    pwm_axi_bvalid : out STD_LOGIC;
    pwm_axi_bready : in STD_LOGIC;
    pwm_axi_araddr : in STD_LOGIC_VECTOR ( 6 downto 0 );
    pwm_axi_arprot : in STD_LOGIC_VECTOR ( 2 downto 0 );
    pwm_axi_arvalid : in STD_LOGIC;
    pwm_axi_arready : out STD_LOGIC;
    pwm_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    pwm_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
    pwm_axi_rvalid : out STD_LOGIC;
    pwm_axi_rready : in STD_LOGIC;
    pwm_axi_aclk : in STD_LOGIC;
    pwm_axi_aresetn : in STD_LOGIC
  );

end basicZturn_PWM_0_0;

architecture stub of basicZturn_PWM_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "pwm[2:0],pwm_axi_awaddr[6:0],pwm_axi_awprot[2:0],pwm_axi_awvalid,pwm_axi_awready,pwm_axi_wdata[31:0],pwm_axi_wstrb[3:0],pwm_axi_wvalid,pwm_axi_wready,pwm_axi_bresp[1:0],pwm_axi_bvalid,pwm_axi_bready,pwm_axi_araddr[6:0],pwm_axi_arprot[2:0],pwm_axi_arvalid,pwm_axi_arready,pwm_axi_rdata[31:0],pwm_axi_rresp[1:0],pwm_axi_rvalid,pwm_axi_rready,pwm_axi_aclk,pwm_axi_aresetn";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "PWM_v2_0,Vivado 2016.4";
begin
end;
