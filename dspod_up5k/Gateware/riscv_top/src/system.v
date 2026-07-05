/*
 * system.v - top-level system for picorv32
 * 06-30-19 E. Brombaugh
 */

`default_nettype none

module system(
	input	clk24,			// clock, reset
			reset,
	
	input	RX,				// serial
	output	TX,
	
	input	[11:0] adc_data,	// adc input
	input	adc_ena0,
			adc_ena1,
			adc_ena2,
			adc_ena3,
	
	input	[15:0] lvl_do,		// level detect input
	output	lvl_ena,
	output	[1:0] lvl_addr,
	
	inout	spi0_mosi,			// SPI core 0
			spi0_miso,
			spi0_sclk,
			spi0_cs0,
			spi0_cs1,
	
	inout	spi1_mosi,			// SPI core 1
			spi1_miso,
			spi1_sclk,
			spi1_cs0,
	
	inout	i2c0_sda,			// I2C core 0
			i2c0_scl,
	
	input	[31:0] gp_in,		// gpio
	output	reg [31:0] gp_out,
	
	output	reg [7:0] mv_w_addr,	// midiverb microcode
	output	reg [15:0] mv_w_data,
	output	reg mv_we
);
	// CPU
	reg         cpu_nrst;
	wire        mem_valid;
	wire        mem_instr;
	wire        mem_ready;
	wire [31:0] mem_addr;
	reg  [31:0] mem_rdata;
	wire [31:0] mem_wdata;
	wire [ 3:0] mem_wstrb;
	picorv32 #(
		.PROGADDR_RESET(32'h 0000_0000),	// start or ROM
		.STACKADDR(32'h 1001_0000),			// end of SPRAM
		.BARREL_SHIFTER(0),
		.COMPRESSED_ISA(0),
		.ENABLE_COUNTERS(0),
		.ENABLE_MUL(0),
		.ENABLE_DIV(0),
		.ENABLE_IRQ(0),
		.ENABLE_IRQ_QREGS(0),
		.CATCH_MISALIGN(0),
		.CATCH_ILLINSN(0)
	) cpu_I (
		.clk       (clk24),
		.resetn    (cpu_nrst),
		.mem_valid (mem_valid),
		.mem_instr (mem_instr),
		.mem_ready (mem_ready),
		.mem_addr  (mem_addr),
		.mem_wdata (mem_wdata),
		.mem_wstrb (mem_wstrb),
		.mem_rdata (mem_rdata)
	);
	
	// Address decode
	reg rom_remap;
	wire rom_sel = (mem_addr[31:28]==4'h0)&~rom_remap&mem_valid ? 1'b1 : 1'b0;
	wire ram_sel = ((mem_addr[31:28]==4'h1)|((mem_addr[31:28]==4'h0)&rom_remap))&mem_valid ? 1'b1 : 1'b0;
	wire gpo_sel = (mem_addr[31:28]==4'h2)&mem_valid ? 1'b1 : 1'b0;
	wire ser_sel = (mem_addr[31:28]==4'h3)&mem_valid ? 1'b1 : 1'b0;
	wire wbb_sel = (mem_addr[31:28]==4'h4)&mem_valid ? 1'b1 : 1'b0;
	wire cnt_sel = (mem_addr[31:28]==4'h5)&mem_valid ? 1'b1 : 1'b0;
	wire adc_sel = (mem_addr[31:28]==4'h6)&mem_valid ? 1'b1 : 1'b0;
	wire sys_sel = (mem_addr[31:28]==4'h7)&mem_valid ? 1'b1 : 1'b0;
	wire lvl_sel = (mem_addr[31:28]==4'h8)&mem_valid ? 1'b1 : 1'b0;
	wire mvb_sel = (mem_addr[31:28]==4'h9)&mem_valid ? 1'b1 : 1'b0;
	
	// system control
	always @(posedge clk24)
		if(reset)
		begin
			cpu_nrst <= 1'b0;
			rom_remap <= 1'b0;
		end
		else if(sys_sel & mem_wstrb[0])
			{rom_remap,cpu_nrst} <= mem_wdata[1:0] ^ 2'b01;
		else
			cpu_nrst <= 1'b1;
		
	// 2k x 32 ROM
	reg [31:0] rom[2047:0], rom_do;
	initial
        $readmemh("rom.hex",rom);		
	always @(posedge clk24)
		rom_do <= rom[mem_addr[12:2]];
	
	// RAM, byte addressable
	wire [31:0] ram_do;
	spram_16kx32 uram(
		.clk(clk24),
		.sel(ram_sel),
		.we(mem_wstrb),
		.addr(mem_addr[15:0]),
		.wdat(mem_wdata),
		.rdat(ram_do)
	);
	
	// GPIO
	always @(posedge clk24)
		if(gpo_sel)
		begin
			if(mem_wstrb[0])
				gp_out[7:0] <= mem_wdata[7:0];
			if(mem_wstrb[1])
				gp_out[15:8] <= mem_wdata[15:8];
			if(mem_wstrb[2])
				gp_out[23:16] <= mem_wdata[23:16];
			if(mem_wstrb[3])
				gp_out[31:24] <= mem_wdata[31:24];
		end
	wire [31:0] gpo_do;
	always @(*)
		if(mem_addr[2])
			gpo_do = gp_in;
		else
			gpo_do = gp_out;
	
	// Serial
	wire [7:0] ser_do;
	acia uacia(
		.clk(clk24),			// system clock
		.rst(reset),			// system reset
		.cs(ser_sel),			// chip select
		.we(mem_wstrb[0]),		// write enable
		.rs(mem_addr[2]),		// address
		.rx(RX),				// serial receive
		.din(mem_wdata[7:0]),	// data bus input
		.dout(ser_do),			// data bus output
		.tx(TX),				// serial transmit
		.irq()					// interrupt request
	);
	
	// 256B Wishbone bus master and SB IP cores @ F100-F1FF
	wire [7:0] wbb_do;
	wire wbb_rdy;
	wb_bus uwbb(
		.clk(clk24),			// system clock
		.rst(reset),			// system reset
		.cs(wbb_sel),			// chip select
		.we(mem_wstrb[0]),		// write enable
		.addr(mem_addr[9:2]),	// address
		.din(mem_wdata[7:0]),	// data bus input
		.dout(wbb_do),			// data bus output
		.rdy(wbb_rdy),			// bus ready
		.spi0_mosi(spi0_mosi),	// spi core 0 mosi
		.spi0_miso(spi0_miso),	// spi core 0 miso
		.spi0_sclk(spi0_sclk),	// spi core 0 sclk
		.spi0_cs0(spi0_cs0),	// spi core 0 cs0
		.spi0_cs1(spi0_cs1),	// spi core 0 cs1
		.spi1_mosi(spi1_mosi),	// spi core 1 mosi
		.spi1_miso(spi1_miso),	// spi core 1 miso
		.spi1_sclk(spi1_sclk),	// spi core 1 sclk
		.spi1_cs0(spi1_cs0),	// spi core 1 cs0
		.i2c0_sda(i2c0_sda),	// i2c core 0 data
		.i2c0_scl(i2c0_scl)		// i2c core 0 clk
	);
	
	// Resettable clock counter
	reg [31:0] cnt;
	always @(posedge clk24)
		if(cnt_sel & |mem_wstrb)
		begin
			if(mem_wstrb[0])
				cnt[7:0] <= mem_wdata[7:0];
			if(mem_wstrb[1])
				cnt[15:8] <= mem_wdata[15:8];
			if(mem_wstrb[2])
				cnt[23:16] <= mem_wdata[23:16];
			if(mem_wstrb[3])
				cnt[31:24] <= mem_wdata[31:24];
		end
		else
			cnt <= cnt + 32'd1;
	
	// ADC holding regs
	reg [11:0] adc_h0, adc_h1, adc_h2, adc_h3;
	always @(posedge clk24)
	begin
		if(reset)
		begin
			adc_h0 <= 12'h000;
			adc_h1 <= 12'h000;
			adc_h2 <= 12'h000;
			adc_h3 <= 12'h000;
		end
		else
		begin 
			if(adc_ena0)
				adc_h0 <= adc_data;
			if(adc_ena1)
				adc_h1 <= adc_data;
			if(adc_ena2)
				adc_h2 <= adc_data;
			if(adc_ena3)
				adc_h3 <= adc_data;
		end
	end
	wire [11:0] adc_do;
	always @(*)
		casex(mem_addr[3:2])
			2'b00: adc_do = adc_h0;
			2'b01: adc_do = adc_h1;
			2'b10: adc_do = adc_h2;
			2'b11: adc_do = adc_h3;
			default: adc_do = adc_h0;
		endcase
	
	// Level detect
	assign lvl_ena = lvl_sel;			// for resetting hold reg
	assign lvl_addr = mem_addr[3:2];	// for muxing onto lvl_do
	
	// Midiverb microcode write interface
	// 16-bits in 32, so insert 16-bit dummy between every half-word.
	always @(posedge clk24)
	begin
		// write only if 16-bits or more
		mv_we <= mvb_sel & mem_wstrb[1] & mem_wstrb[0];
		
		if(mvb_sel)
		begin
			// grab address
			mv_w_addr <= mem_addr[9:2];
			
			// grab data
			if(mem_wstrb[0])
				mv_w_data[7:0] <= mem_wdata[7:0];
			if(mem_wstrb[1])
				mv_w_data[15:8] <= mem_wdata[15:8];
		end
	end
	
	// Read Mux
	always @(*)
		casex({lvl_sel,adc_sel,cnt_sel,wbb_sel,ser_sel,gpo_sel,ram_sel,rom_sel})
			8'b00000001: mem_rdata = rom_do;
			8'b0000001x: mem_rdata = ram_do;
			8'b000001xx: mem_rdata = gpo_do;
			8'b00001xxx: mem_rdata = {{24{1'b0}},ser_do};
			8'b0001xxxx: mem_rdata = {{24{1'b0}},wbb_do};
			8'b001xxxxx: mem_rdata = cnt;
			8'b01xxxxxx: mem_rdata = adc_do;
			8'b1xxxxxxx: mem_rdata = lvl_do;
			default: mem_rdata = 32'd0;
		endcase
	
	// ready flag
	reg mem_rdy;
	always @(posedge clk24)
		if(reset)
			mem_rdy <= 1'b0;
		else
			mem_rdy <= (mvb_sel|lvl_sel|adc_sel|cnt_sel|ser_sel|gpo_sel|ram_sel|rom_sel) & ~mem_rdy;
	assign mem_ready = wbb_rdy | mem_rdy;

endmodule

