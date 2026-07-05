// riscv_top.v - RISC-V top level for dspod_up5k
// 06-04-26 E. Brombaugh

`default_nettype none

// single bit of debounce logic
module db_bit #(
	parameter LEN = 5
)(
	input clk,
	input reset,
	input ena,
	input raw,
	output reg dbnc
);
	reg [LEN-1:0] shift;
	always @(posedge clk)
		if(reset)
		begin
			shift <= 0;
			dbnc <= 1'b0;
		end
		else if(ena)
		begin
			// shift in inverse of raw bits because buttons assert low
			shift <= {shift[LEN-2:0],~raw};
			
			// detect all ones or all zeros cases and set output
			if(&shift)
				dbnc <= 1'b1;
			else if(&~shift)
				dbnc <= 1'b0;
		end
endmodule

// top-level debounce with decimation
module debounce #(
	parameter WIDTH = 5,
	parameter LEN = 5,
	parameter DECIM = 2667
) (
	input clk,
	input reset,
	input raw_valid,
	input [WIDTH-1:0] raw_in,
	output [WIDTH-1:0] dbnc_out,
	output reg dbnc_valid
);
	// decimate input rate
	localparam integer DEC_WID = $clog2(DECIM);
	reg [DEC_WID-1:0] dec_cnt;
	reg dec_ena;
	always @(posedge clk)
		if(reset)
		begin
			dec_cnt <= 0;
			dec_ena <= 1'b0;
			dbnc_valid <= 1'b0;
		end
		else
		begin
			dbnc_valid <= raw_valid;
			
			if(raw_valid)
			begin
				if(dec_cnt == (DECIM-1))
				begin
					dec_cnt <= 0;
					dec_ena <= 1'b1;
				end
				else
				begin
					dec_cnt <= dec_cnt + 1;
					dec_ena <= 1'b0;
				end
			end
		end
	// debounce all bits
	genvar i;
	generate
		for(i=0;i<WIDTH;i++)
		begin
			db_bit #(
				.LEN(LEN)
			) udb (
				.clk(clk),
				.reset(reset),
				.ena(dec_ena),
				.raw(raw_in[i]),
				.dbnc(dbnc_out[i])
			);
		end
	endgenerate
endmodule
