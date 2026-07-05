// rotary_decode.v - decoder logic for debounced rotary encoder
// 06-19-26 E. Brombaugh

`default_nettype none

module rotary_decode(
	input clk,
	input reset,
	input ena,
	input enc_a, enc_b,
	input clr,
	output reg signed [7:0] value
);

	// detect edges of B
	reg prev_b, re_b, fe_b;
	always @(posedge clk)
		if(reset)
		begin
			re_b <= 1'b0;
			fe_b <= 1'b1;
		end
		else if(ena)
		begin
			prev_b <= enc_b;
			re_b <= (~prev_b & enc_b);
			fe_b <= (prev_b & ~enc_b);
		end
	
	// decode
	always @(posedge clk)
		if(reset | clr)
			value <= 8'h00;
		else if(ena)
		begin
			if(re_b)
			begin
				if(enc_a)
					value <= value + 8'h01;
				else
					value <= value - 8'h01;
			end
			else if(fe_b)
			begin
				if(~enc_a)
					value <= value + 8'h01;
				else
					value <= value - 8'h01;
			end
		end
endmodule

