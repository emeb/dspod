// wd_mix.v - wet/dry mix for 16-bit stereo audio
// 06-22-2026 E. Brombaugh

module wd_mix (
	input clk,
	input reset,
	input ena,
	input [11:0] mix,
	input mix_ena,
	input signed [15:0] left_dry, right_dry,
	input signed [15:0] left_wet, right_wet,
	output reg signed [15:0] left_out, right_out,
	output reg valid
);
	// register, compute and zero-extend wet & dry gains from mix
	reg [11:0] mix_hold;
	always @(posedge clk)
		if(reset)
			mix_hold <= 12'd0;
		else if(mix_ena)
			mix_hold <= mix;
		
	wire signed [15:0] wet_gain = {4'd0,mix_hold};
	wire signed [15:0] dry_gain = {4'd0,~mix_hold};
	
	// mux inputs into a MAC
	reg [6:0] ena_pipe;
	reg signed [15:0] mult_a, mult_b;
	reg signed [31:0] mult_out;
	reg signed [31:0] accum;
	wire signed [15:0] sat_acc;
	reg signed [15:0] pre_left_out;
	always @(posedge clk)
		if(reset)
		begin
			ena_pipe <= 7'd0;
			valid <= 1'b0;
			mult_a <= 16'd0;
			mult_b <= 16'd0;
			mult_out <= 32'd0;
			accum <= 32'd0;
		end
		else
		begin
			// one-hot enable pipeline
			ena_pipe <= {ena_pipe[5:0],ena};
			
			// MAC input muxing 
			case(ena_pipe[3:0])
				4'b0001:
				begin
					mult_a <= left_wet;
					mult_b <= wet_gain;
				end
				
				4'b0010:
				begin
					mult_a <= left_dry;
					mult_b <= dry_gain;
				end
				
				4'b0100:
				begin
					mult_a <= right_wet;
					mult_b <= wet_gain;
				end
				4'b1000:
				begin
					mult_a <= right_dry;
					mult_b <= dry_gain;
				end
			endcase
			
			// multiplier
			mult_out <= mult_a * mult_b;
			
			// accumulator with rounding
			if(ena_pipe[2] | ena_pipe[4])
				accum <= mult_out + {20'd0,(1'b1 << 11)};
			else if(ena_pipe[3] | ena_pipe[5])
				accum <= mult_out + accum;
			
			// output holding registers with saturation
			if(ena_pipe[4])
				pre_left_out <= sat_acc;
			
			if(ena_pipe[6])
			begin
				left_out <= pre_left_out;
				right_out <= sat_acc;
			end
			
			// output enable
			valid <= ena_pipe[6];
		end
	
	// drop lsbits and saturate accumulator
	sat #(
		.isz(20),
		.osz(16),
		.warn(1)
	) usat (
		.in(accum[31:12]),
		.out(sat_acc)
	);
endmodule
