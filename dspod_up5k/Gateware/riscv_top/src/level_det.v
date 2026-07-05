// level_det.v - I2S audio level detection
// 06-22-2026 E. Brombaugh

`default_nettype none

module level_det(
	input clk,
	input reset,
	input ena,
	input signed [15:0] left_rx, right_rx, left_tx, right_tx,
	input lvl_ena,
	input [1:0] lvl_addr,
	output reg [15:0] lvl_do
);
	// one-hot enables
	reg [6:0] ena_pipe;
	always @(posedge clk)
		if(reset)
			ena_pipe <= 6'd0;
		else
			ena_pipe <= {ena_pipe[5:0],ena};
		
	// mux inputs into recifier
	reg signed [15:0] mux;
	always @(*)
		casex(ena_pipe[3:0])
			4'b0001: mux = left_rx;
			4'b001x: mux = right_rx;
			4'b01xx: mux = left_tx;
			4'b1xxx: mux = right_tx;
			default: mux = 16'bx;
		endcase
	
	// rectifier
	reg [15:0] lvl_data;
	always @(posedge clk)
		if(reset)
			lvl_data <= 16'd0;
		else
			lvl_data <= mux[15] ? {1'b0,~mux[14:0]} +15'd1 : {1'b0,mux[14:0]};
	
	// mux hold regs into detector
	reg [15:0] lvl_lr, lvl_rr, lvl_lt, lvl_rt;
	reg [15:0] lvl_hld_mux;
	always @(*)
		casex(ena_pipe[4:1])
			4'b0001: lvl_hld_mux = lvl_lr;
			4'b001x: lvl_hld_mux = lvl_rr;
			4'b01xx: lvl_hld_mux = lvl_lt;
			4'b1xxx: lvl_hld_mux = lvl_rt;
			default: lvl_hld_mux = 16'bx;
		endcase
		
	// level Peak detection
	wire [15:0] lvl_peak = (lvl_data > lvl_hld_mux) ? lvl_data : lvl_hld_mux;
		
	// level hold registers
	reg [1:0] lvl_clr_addr;
	reg [2:0] lvl_clr_pipe;
	always @(posedge clk)
	begin
		if(reset)
		begin
			lvl_lr <= 16'h000;
			lvl_rr <= 16'h000;
			lvl_lt <= 16'h000;
			lvl_rt <= 16'h000;
			lvl_clr_addr <= 2'b00;
			lvl_clr_pipe <= 3'b000;
		end
		else
		begin
			// grab the read address for clearing
			if(lvl_ena)
				lvl_clr_addr <= lvl_addr;
			
			// delay the clear signal until after read complete
			lvl_clr_pipe <= {lvl_clr_pipe[1:0],lvl_ena};
			
			// hold the peaks
			if(ena_pipe[1])
				lvl_lr <= lvl_peak;
			if(ena_pipe[2])
				lvl_rr <= lvl_peak;
			if(ena_pipe[3])
				lvl_lt <= lvl_peak;
			if(ena_pipe[4])
				lvl_rt <= lvl_peak;
			
			// clear the registers after read
			if(lvl_clr_pipe[2])
				case(lvl_clr_addr)
					2'b00: lvl_lr <= 16'd0;
					2'b01: lvl_rr <= 16'd0;
					2'b10: lvl_lt <= 16'd0;
					2'b11: lvl_rt <= 16'd0;
				endcase
		end
	end
	
	// mux hold regs out to CPU
	always @(*)
		casex(lvl_addr)
			2'b00: lvl_do = lvl_lr;
			2'b01: lvl_do = lvl_rr;
			2'b10: lvl_do = lvl_lt;
			2'b11: lvl_do = lvl_rt;
			default: lvl_do = lvl_lr;
		endcase
endmodule

