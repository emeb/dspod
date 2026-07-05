// ps_input.v - Parallel/Serial input via 74HC165 for dspod_up5k
// 06-05-26 E. Brombaugh

`default_nettype none

module ps_input(
	input clk,
	input reset,
	input SER_DAT,
	output SER_CLK,
	output reg SER_SHF,
	output reg[7:0] ps_data,
	output reg ps_valid
);
	reg [3:0] ps_state;
	reg [7:0] ps_sr;
	always @(posedge clk)
	begin
		if(reset)
		begin
			ps_state <= 4'h0;
			ps_data <= 8'h00;
			SER_SHF <= 1'b1;
			ps_valid <= 1'b0;
		end
		else
		begin
			// state counter runs 0-8
			if(ps_state == 4'h8)
				ps_state <= 4'h0;
			else
				ps_state <= ps_state + 4'h1;
			
			// shift output asserts during state 8
			if(ps_state == 4'h7)
				SER_SHF <= 1'b0;
			else
				SER_SHF <= 1'b1;
			
			// shift register
			if(ps_state != 4'h8)
				ps_sr <= {ps_sr[6:0],SER_DAT};
			
			// hold register
			if(ps_state == 4'h8)
			begin
				ps_data <= ps_sr;
				ps_valid <= 1'b1;
			end
			else
				ps_valid <= 1'b0;
		end
	end
	assign SER_CLK = clk;
endmodule
			
