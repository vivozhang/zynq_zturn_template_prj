`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2017/04/23 15:20:09
// Design Name: 
// Module Name: addition_main
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module addition_main(
    input clk,
    input [15:0] inputa,
    input [15:0] inputb,
    output [31:0] product
    );
    
 reg [31:0] productReg;
    always@(posedge clk) begin
    productReg <= inputa *inputb;
    end
    
 assign product = productReg;
 
endmodule
