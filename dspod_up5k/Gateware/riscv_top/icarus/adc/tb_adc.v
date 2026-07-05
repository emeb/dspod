// tb_system.v - testbench for adc
// 06-07-26 E. Brombaugh

`timescale 1ns/1ps
`default_nettype none

module tb_adc;
    reg clk;
    reg reset;
	reg ADC_DIN;
    wire ADC_CS, ADC_CLK, ADC_DOUT;
	wire [11:0] adc_data;
	wire a0_v, a1_v, a2_v, a3_v;
	
    // 24MHz clock source
    always
        #21 clk = ~clk;
    
    // reset
    initial
    begin
`ifdef icarus
  		$dumpfile("tb_adc.vcd");
		$dumpvars;
`endif
        
        // init regs
        clk = 1'b0;
        reset = 1'b1;
        ADC_DIN = 1'b0;
        
        // release reset
        #100
        reset = 1'b0;
        
`ifdef icarus
        // stop after 1 sec
		#2000000 $finish;
`endif
    end
    
    // Unit under test
	adc uut(
		.clk(clk),
		.reset(reset),
    
		.adc_cs(ADC_CS),
		.adc_si(ADC_DOUT),
		.adc_clko(ADC_CLK),
		.adc_so(ADC_DIN),
	
		.adc_data(adc_data),
	
		.a0_v(a0_v),
		.a1_v(a1_v),
		.a2_v(a2_v),
		.a3_v(a3_v)
	);
endmodule
