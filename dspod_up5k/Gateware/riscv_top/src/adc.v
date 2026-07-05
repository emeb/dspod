// adc.v - driver logic for adc124s051 4-chl 12-bit 500kSPS ADC
// 03-18-21 E. Brombaugh

`default_nettype none

module adc#(
	parameter [2:0] PRESCALE = 0,	// control clock division
) (
    input clk,              // 24MHz system clock
    input reset,            // Low-true reset
    
	output reg adc_cs, adc_si,
	output adc_clko,
	input adc_so,			// si, so from ADC perspective
	
	output reg [11:0] adc_data,
	output reg a0_v, a1_v, a2_v, a3_v
);
	// divide 24MHz clock down - more Tacq to reduce crosstalk
	localparam CLKDIV_BITS = PRESCALE+3;
	reg clk_ena, clk_ena_180, adc_clk;
	reg [CLKDIV_BITS-1:0] clk_div;
	always @(posedge clk)
		if(reset)
		begin
			clk_div <= 0;
			clk_ena <= 1'b1;
			clk_ena_180 <= 1'b0;
			adc_clk <= 1'b1;
		end
		else
		begin
			clk_div <= clk_div + 1;
			clk_ena <= 1'b0;
			clk_ena_180 <= 1'b0;
			
			if(clk_div == (3'd0<<PRESCALE))
				adc_clk <= 1'b1;
				
			if(clk_div == (3'd2<<PRESCALE))
				clk_ena_180 <= 1'b1;
			
			if(clk_div == (3'd3<<PRESCALE))
				adc_clk <= 1'b0;
			
			if(clk_div == {3'd5,{PRESCALE{1'b1}}})	// end of sequence
			begin
				clk_ena <= 1'b1;
				clk_div <= 3'd0;
			end
		end
		
	// state machine generates cs, si (input to ADC) and assembles result
	reg [3:0] adc_cnt;
	reg [1:0] adc_chl, adc_pchl, data_chl;
	reg [11:0] sreg, data_curr, data_prev;
	reg data_valid;
	always @(posedge clk)
		if(reset)
		begin
			adc_cs <= 1'b1;
			adc_si <= 1'b0;
			adc_cnt <= 4'h0;
			adc_chl <= 2'b11;
			adc_pchl <= 2'b10;
			sreg <= 12'h000;
			data_curr <= 12'h000;
			data_prev <= 12'h000;
			data_chl <= 2'b00;
			data_valid <= 1'b0;
		end
		else
		begin
			// count always advances
			if(clk_ena)
				adc_cnt <= adc_cnt + 1;
			
			// channel advances at end of cycle
			if(clk_ena && (adc_cnt == 4'hf))
			begin
				adc_chl <= adc_chl + 1;
				adc_pchl <= adc_chl;
			end
			
			// cs drops after first count cycle
			if(clk_ena_180 && (adc_cnt == 4'hf) && adc_cs)
				adc_cs <= 1'b0;
			
			// si shifts out chl addr on cycles 3,4 (bits 4, 3)
			if(clk_ena)
				case(adc_cnt)
					4'h1: adc_si <= 1'b0;
					4'h2: adc_si <= adc_chl[1];
					4'h3: adc_si <= adc_chl[0];
					default: adc_si <= 1'b0;
				endcase
			
			// input shift register
			if(clk_ena_180 & (adc_cnt[3:2]!=2'b00))
				sreg[11:0] <= {sreg[10:0],adc_so};
			
			// output holding registers with inversion
			if(clk_ena_180 && (adc_cnt == 4'hf))
			begin
				data_curr <= ~{sreg[10:0],adc_so};
				data_prev <= data_curr;
				data_chl <= adc_pchl;
				data_valid <= 1'b1;
			end
			else
				data_valid <= 1'b0;
		end
	// bypass correction
	wire corrected_valid = data_valid;
	wire [11:0] corrected = data_curr;
		
	// demux and final output result
	always @(posedge clk)
		if(reset)
		begin
			adc_data <= 12'd0;
			a0_v <= 1'b0;
			a1_v <= 1'b0;
			a2_v <= 1'b0;
			a3_v <= 1'b0;
		end
		else
		begin
			// holding reg
			if(corrected_valid)
				adc_data <= corrected;
			
			// output valids
			if(corrected_valid)
				case(adc_pchl)
					2'b00: a0_v <= 1'b1;
					2'b01: a1_v <= 1'b1;
					2'b10: a2_v <= 1'b1;
					2'b11: a3_v <= 1'b1;
				endcase
			else
			begin
				a0_v <= 1'b0;
				a1_v <= 1'b0;
				a2_v <= 1'b0;
				a3_v <= 1'b0;
			end
		end
	
	// disable adc_clko when cs high
	assign adc_clko = adc_clk | adc_cs;
endmodule
