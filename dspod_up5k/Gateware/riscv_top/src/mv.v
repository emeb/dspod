// mv.v - midiverb circuit
// 08-28-21 E. Brombaugh

`default_nettype none

// uncomment this to use microcode in read-only BRAM
//`define RTL_UCODE

module mv(
	input clk,
	input reset,
	input ena,
	input signed [15:0] in,
`ifndef RTL_UCODE
	input uc_w_clk,
	input [7:0] uc_w_addr,
	input [15:0] uc_w_data,
	input uc_we,
	input prog,
`else
	input [5:0] prog,
`endif
	output curr_prog,
	output reg signed [15:0] out_l, out_r,
	output reg valid);
	
	// instruction pointer
	reg [6:0] iaddr;
	always @(posedge clk)
		if(reset)
			iaddr <= 7'h00;
		else if(ena)
			iaddr <= iaddr + 7'h01;
	
`ifdef RTL_UCODE
	// program selector
	reg rprog[5:0];
	always @(posedge clk)
	begin
		if(reset)
			rprog <= prog;
		else if(ena)
			if(iaddr == 7'h00)
				rprog <= prog;
	end
	
	// Microcode stored in Read-only BRAM
	reg [15:0] microcode[2047:0], ucode;
	
	initial
		$readmemh("../src/mv_code.hex",microcode);
	
	always @(posedge clk)
		ucode <= microcode[{rprog[3:0],iaddr}];
`else
	// program selector
	reg rprog;
	always @(posedge clk)
		if(reset)
			rprog <= 1'b0;
		else if(ena & (iaddr == 7'h7f))
			rprog <= prog;
		
	assign curr_prog = rprog;
		
	// Microcode stored in dual-port BRAM
	reg [15:0] microcode[255:0], ucode;
	
	// load it with known-good algos at start
	initial
		$readmemh("../src/mv_code.hex",microcode);
		
	// write port
	always @(posedge uc_w_clk)
		if(uc_we)
			microcode[uc_w_addr] <= uc_w_data;
	
	// read port
	always @(posedge clk)
		ucode <= microcode[{rprog,iaddr}];
`endif
	
	// split out components
	wire [1:0] op = ucode[15:14];
	wire [13:0] addr_inc = ucode[13:0];
	
	// address accumulator
	reg [13:0] addr;
	always @(posedge clk)
	begin
		if(reset)
			addr <= 14'h0000;
		else if(ena)
			addr <= addr + addr_inc;
	end
	
	// memory store
	reg signed [15:0] databus, acc;
	wire ram_we = (ena & (op[1] | (iaddr == 7'h0)));
	wire signed [15:0] ram_do;		
	SB_SPRAM256KA
		uram(
			.ADDRESS(addr),
			.DATAIN(databus),
			.MASKWREN(4'hf),
			.WREN(ram_we),
			.CHIPSELECT(1'b1),
			.CLOCK(clk),
			.STANDBY(1'b0),
			.SLEEP(1'b0),
			.POWEROFF(1'b1),
			.DATAOUT(ram_do)
		);

	// data bus
	always @(posedge clk)
		if(iaddr == 7'h00)
			databus <= {{3{in[15]}},in[15:3]} & 16'hFFFE;
		else
			casex(op)
				2'b0x: databus <= ram_do;
				2'b10: databus <= acc;
				2'b11: databus <= ~acc;
			endcase
	
	// saturation
	wire signed [12:0] osat;
	sat #(.isz(16), .osz(13), .warn(0))
	usat(
		.in(databus),
		.out(osat)
	);
	
	// output
	reg signed [12:0] otmp;
	always @(posedge clk)
	begin
		if(reset)
		begin
			valid <= 1'b0;
			otmp <= 13'd0;
			out_l <= 16'd0;
			out_r <= 16'd0;
		end
		else
		begin
			valid <= 1'b0;
			if(ena)
			begin
				if(iaddr == 7'h60)
					otmp <= osat;
				if(iaddr == 7'h70)
				begin
					out_l <= {osat,3'h0};
					out_r <= {otmp,3'h0};
					valid <= 1'b1;
				end
			end
		end
	end
	
	// accumulator
	always @(posedge clk)
		if(ena & (iaddr != 7'h00) & (iaddr != 7'h60) & (iaddr != 7'h70))
			acc <= {databus[15],databus[15:1]} + {15'h0000,databus[15]} + (op[0] ? 16'h0000 : acc);
		
endmodule
