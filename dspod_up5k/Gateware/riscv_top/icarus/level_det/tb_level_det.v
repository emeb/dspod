// tb_level_det.v - testbench for level_det
// 07-01-26 E. Brombaugh

`timescale 1ns/1ps
`default_nettype none

module tb_level_det;
    reg clk;
    reg reset;
	reg signed [15:0] left_tx, right_tx;
	reg signed [15:0] left_rx, right_rx;
	reg [5:0] enacnt;
	reg i2s_load;
	reg lvl_ena;
	reg [1:0] lvl_addr;
	wire [15:0] lvl_do;
	
    // 12MHz clock source
    always
        #42 clk = ~clk;
    
    // reset
    initial
    begin
`ifdef icarus
  		$dumpfile("tb_level_det.vcd");
		$dumpvars;
`endif
        
        // init regs
        clk = 1'b0;
        reset = 1'b1;
        lvl_addr = 1'b0;
		lvl_ena = 1'b0;
		
        // release reset
        #100
        reset = 1'b0;
		
 `ifdef icarus
        // stop after 1 sec
		#2000000 $finish;
`endif
    end
	
	// periodic load signal
	always @(posedge clk)
		if(reset)
		begin
			enacnt <= 6'd0;
			i2s_load <= 1'b0;
		end
		else
		begin
			enacnt <= enacnt + 6'd1;
			i2s_load <= &enacnt;
		end

	// data generation
	always @(posedge clk)
		if(reset)
		begin
			left_rx <= 16'h0000;
			right_rx <= 16'h0000;
			left_tx <= 16'h0000;
			right_tx <= 16'h0000;
		end
		else if(i2s_load)
		begin
			left_rx <= left_rx + 16'd1;
			right_rx <= right_rx - 16'd2;
			left_tx <= left_tx + 16'd4;
			right_tx <= right_tx - 16'd8;
		end
			
    // Unit under test
	level_det uut(
		.clk(clk),
		.reset(reset),
		.ena(i2s_load),
		.left_rx(left_rx),
		.right_rx(right_rx),
		.left_tx(left_tx),
		.right_tx(right_tx),
		.lvl_ena(lvl_ena),
		.lvl_addr(lvl_addr),
		.lvl_do(lvl_do)
	);
endmodule
