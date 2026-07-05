// i2s_inout.v:
// 2016-12-18 E. Brombaugh

module i2s_inout(clk, reset,
				l_in, r_in,
				l_out, r_out,
				sdout, sdin, sclk, lrclk, mclk,
				load);
	
	input clk;								// System clock
	input reset;							// System POR
	input signed [15:0] l_in, r_in;			// parallel inputs
	output reg signed [15:0] l_out, r_out;	// parallel outputs
	output sdout;							// I2S serial data out
	input sdin;								// I2S serial data in
	output sclk;							// I2S serial clock
	output lrclk;							// I2S Left/Right clock
    output mclk;                            // I2S Master Clock
	output load;							// Sample rate enable output
	
    // clock generator creates
    // clk/2 mclk (12MHz)
    // clk/16 sclk & shift (1.5MHz)
    // clk/512 lrck & load
    // -> 46.875kHz sample rate for 24MHz clk
	reg [8:0] clkdiv;
	reg shift;
    reg load;
	always @(posedge clk)
        if(reset == 1'b1)
        begin
            clkdiv <= 9'd0;
            shift <= 1'b0;
            load <= 1'b0;
        end
        else
        begin
            clkdiv <= clkdiv + 9'd1;
            shift <= 1'b0;
            load <= 1'b0;
            if(clkdiv[3:0] == 4'd14)
            begin
                if(clkdiv[8:4]==5'd0)
                    load <= 1'b1;
                else
                    shift <= 1'b1;
            end
        end
    assign mclk = !clkdiv[0];
    assign sclk = clkdiv[3];        
    assign lrclk = clkdiv[8];

		// output Shift register advances on serial clock
	reg [31:0] sreg;
	always @(posedge clk)
        if(load)
        begin
            sreg <= {l_in,r_in};
            {l_out,r_out} <= {sreg[30:0],sdin};
        end
        else if(shift)
        begin
            sreg <= {sreg[30:0],sdin};
        end
    assign sdout = sreg[31];
endmodule
