//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.4 (win64) Build 1756540 Mon Jan 23 19:11:23 MST 2017
//Date        : Fri May 05 01:54:04 2017
//Host        : DESKTOP-UTEFEFF running 64-bit major release  (build 9200)
//Command     : generate_target basicZturn_wrapper.bd
//Design      : basicZturn_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module basicZturn_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    buzzer_tri_o,
    i2c0_scl_io,
    i2c0_sda_io,
    pwm,
    rgb_led_tri_o,
    sws_4bits_tri_i,
    uart_rtl_rxd,
    uart_rtl_txd);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  output buzzer_tri_o;
  inout i2c0_scl_io;
  inout i2c0_sda_io;
  output [2:0]pwm;
  output [2:0]rgb_led_tri_o;
  input [3:0]sws_4bits_tri_i;
  input uart_rtl_rxd;
  output uart_rtl_txd;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire buzzer_tri_o;
  wire i2c0_scl_i;
  wire i2c0_scl_io;
  wire i2c0_scl_o;
  wire i2c0_scl_t;
  wire i2c0_sda_i;
  wire i2c0_sda_io;
  wire i2c0_sda_o;
  wire i2c0_sda_t;
  wire [2:0]pwm;
  wire [2:0]rgb_led_tri_o;
  wire [3:0]sws_4bits_tri_i;
  wire uart_rtl_rxd;
  wire uart_rtl_txd;

  basicZturn basicZturn_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .buzzer_tri_o(buzzer_tri_o),
        .i2c0_scl_i(i2c0_scl_i),
        .i2c0_scl_o(i2c0_scl_o),
        .i2c0_scl_t(i2c0_scl_t),
        .i2c0_sda_i(i2c0_sda_i),
        .i2c0_sda_o(i2c0_sda_o),
        .i2c0_sda_t(i2c0_sda_t),
        .pwm(pwm),
        .rgb_led_tri_o(rgb_led_tri_o),
        .sws_4bits_tri_i(sws_4bits_tri_i),
        .uart_rtl_rxd(uart_rtl_rxd),
        .uart_rtl_txd(uart_rtl_txd));
  IOBUF i2c0_scl_iobuf
       (.I(i2c0_scl_o),
        .IO(i2c0_scl_io),
        .O(i2c0_scl_i),
        .T(i2c0_scl_t));
  IOBUF i2c0_sda_iobuf
       (.I(i2c0_sda_o),
        .IO(i2c0_sda_io),
        .O(i2c0_sda_i),
        .T(i2c0_sda_t));
endmodule
