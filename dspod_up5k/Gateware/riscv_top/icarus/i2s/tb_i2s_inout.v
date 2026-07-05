// tb_i2s_inout.v - testbench for I2S interface
// 06-08-26 E. Brombaugh

`timescale 1ns/1ps
`default_nettype none

module tb_i2s_inout;
    reg clk;
    reg reset;
	wire I2S_DI;
    wire I2S_MCLK, I2S_LRCK, I2S_SCLK, I2S_DO;
	reg signed [15:0] left_tx, right_tx;
	wire signed [15:0] left_rx, right_rx;
	wire i2s_load;
	
    // 24MHz clock source
    always
        #21 clk = ~clk;
    
    // reset
    initial
    begin
`ifdef icarus
  		$dumpfile("tb_i2s_inout.vcd");
		$dumpvars;
`endif
        
        // init regs
        clk = 1'b0;
        reset = 1'b1;
        //I2S_DI = 1'b0;
        
        // release reset
        #100
        reset = 1'b0;
 `ifdef icarus
        // stop after 1 sec
		#2000000 $finish;
`endif
    end

	// data generation
	always @(posedge clk)
		if(reset)
		begin
			left_tx <= 16'h0000;
			right_tx <= 16'h0000;
		end
		else if(i2s_load)
		begin
			left_tx <= left_tx + 16'd1;
			right_tx <= right_tx - 16'd1;
		end
	
	//assign I2S_DI = I2S_DO;
	assign I2S_DI = I2S_LRCK;
		
    // Unit under test
	i2s_inout uut(
		.clk(clk),
		.reset(reset),
		.l_in(left_tx),
		.r_in(right_tx),
		.l_out(left_rx),
		.r_out(right_rx),
		.sdout(I2S_DO),
		.sdin(I2S_DI),
		.sclk(I2S_SCLK),
		.lrclk(I2S_LRCK),
		.mclk(I2S_MCLK),
		.load(i2s_load)
	);
endmodule
