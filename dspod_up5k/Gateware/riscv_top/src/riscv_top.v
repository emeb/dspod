// riscv_top.v - RISC-V top level for dspod_up5k
// 06-04-26 E. Brombaugh

`default_nettype none

module riscv_top(
	// 12MHz clock osc
	input	CLK12MHZ,
	
	// audio clock input
	input	AUDIOCLK,
	
	// button
	input	BTN,
	
	// par/ser input
	input	SER_DAT,
	output	SER_CLK,
	output	SER_SHF,
	
	// ADC - DOUT/DIN from ADC pov
	input	ADC_DOUT,
	output	ADC_DIN,
			ADC_CLK,
			ADC_CS,
		
	// SPI0 port hooked to cfg flash & psram
	inout	MEM_SPI_MOSI_IO0,
			MEM_SPI_MISO_IO1,
			MEM_SPI_SCLK,
			MEM_SPI_WP_IO2,
			MEM_SPI_HLD_IO3,
			MEM_SPI_FLASH_CS,
			MEM_SPI_PSRAM_CS,
	
	// Gate output
	output	GATE_OUT,
	
	// I2S - DO/DI from FPGA pov
	input	I2S_DI,
	output	I2S_MCLK,
			I2S_DO,
			I2S_SCLK,
			I2S_LRCK,
	
    // ACIA serial
    inout 	RX, TX,
	
	// I2C0 port
	inout	SDA,
			SCL,
	
	// LED - via drivers
	output	RGB0,
			RGB1,
			RGB2,
			
	// SPI1 port for LCD
	inout	LCD_SDO,
			LCD_SCK,
			LCD_CS,
	
	// GP Out for LCD
	output	LCD_NRST,
			LCD_DC,
			LCD_BK
);
		
	// Fin=12, Fout=24
	wire clk, pll_lock;
	SB_PLL40_PAD #(
		.DIVR(4'b0000),
		.DIVF(7'b0111111),
		.DIVQ(3'b101),
		.FILTER_RANGE(3'b001),
		.FEEDBACK_PATH("SIMPLE"),
		.DELAY_ADJUSTMENT_MODE_FEEDBACK("FIXED"),
		.FDA_FEEDBACK(4'b0000),
		.DELAY_ADJUSTMENT_MODE_RELATIVE("FIXED"),
		.FDA_RELATIVE(4'b0000),
		.SHIFTREG_DIV_MODE(2'b00),
		.PLLOUT_SELECT("GENCLK"),
		.ENABLE_ICEGATE(1'b0)
	)
	pll_inst (
		.PACKAGEPIN(CLK12MHZ),
		.PLLOUTCORE(clk),
		.PLLOUTGLOBAL(),
		.EXTFEEDBACK(),
		.DYNAMICDELAY(8'h00),
		.RESETB(1'b1),
		.BYPASS(1'b0),
		.LATCHINPUTVALUE(),
		.LOCK(pll_lock),
		.SDI(),
		.SDO(),
		.SCLK()
	);
	
	// reset generator waits > 10us afer PLL lock
	reg [7:0] reset_cnt;
	reg reset;    
	always @(posedge clk)
	begin
		if(!pll_lock)
		begin
			reset_cnt <= 8'h00;
			reset <= 1'b1;
		end
		else
		begin
			if(reset_cnt != 8'hff)
			begin
				reset_cnt <= reset_cnt + 8'h01;
				reset <= 1'b1;
			end
			else
				reset <= 1'b0;
		end
	end
	
	// reset generator for the audio clock domain
	reg [2:0] audio_reset_pipe;
	initial
		audio_reset_pipe = 3'b111;
	always @(posedge AUDIOCLK)
		audio_reset_pipe <= {audio_reset_pipe[1:0], reset};
	wire audio_reset = audio_reset_pipe[2];
	
	// system core
	wire [31:0] gpio_o, gpio_i;
	wire raw_rx, raw_tx;
	wire [11:0] adc_data;
	wire a0_v, a1_v, a2_v, a3_v;
	wire [15:0] lvl_do;
	wire lvl_ena;
	wire [1:0] lvl_addr;
	wire [7:0] mv_w_addr;
	wire [15:0] mv_w_data;
	wire mv_we;
	system uut(
		.clk24(clk),
		.reset(reset),
		
		.RX(raw_rx),
		.TX(raw_tx),
	
		.adc_data(adc_data),
		.adc_ena0(a0_v),
		.adc_ena1(a1_v),
		.adc_ena2(a2_v),
		.adc_ena3(a3_v),
	
		.lvl_do(lvl_do),
		.lvl_ena(lvl_ena),
		.lvl_addr(lvl_addr),
		
		.spi0_mosi(MEM_SPI_MOSI_IO0),
		.spi0_miso(MEM_SPI_MISO_IO1),
		.spi0_sclk(MEM_SPI_SCLK),
		.spi0_cs0(MEM_SPI_FLASH_CS),
		.spi0_cs1(MEM_SPI_PSRAM_CS),
	
		.spi1_mosi(LCD_SDO),
		.spi1_sclk(LCD_SCK),
		.spi1_cs0(LCD_CS),
	
		.i2c0_sda(SDA),
		.i2c0_scl(SCL),
	
		.gp_out(gpio_o),
		.gp_in(gpio_i),
		
		.mv_w_addr(mv_w_addr),
		.mv_w_data(mv_w_data),
		.mv_we(mv_we)
	);
	
	// remaining memory bus signals tied inactive
	assign MEM_SPI_WP_IO2 = 1'b1;
	assign MEM_SPI_HLD_IO3 = 1'b1;
	
	// Serial I/O w/ pullup on RX
	SB_IO #(
		.PIN_TYPE(6'b101001),
		.PULLUP(1'b1),
		.NEG_TRIGGER(1'b0),
		.IO_STANDARD("SB_LVCMOS")
	) urx_io (
		.PACKAGE_PIN(RX),
		.LATCH_INPUT_VALUE(1'b0),
		.CLOCK_ENABLE(1'b0),
		.INPUT_CLK(1'b0),
		.OUTPUT_CLK(1'b0),
		.OUTPUT_ENABLE(1'b0),
		.D_OUT_0(1'b0),
		.D_OUT_1(1'b0),
		.D_IN_0(raw_rx),
		.D_IN_1()
	);
	SB_IO #(
		.PIN_TYPE(6'b101001),
		.PULLUP(1'b0),
		.NEG_TRIGGER(1'b0),
		.IO_STANDARD("SB_LVCMOS")
	) utx_io (
		.PACKAGE_PIN(TX),
		.LATCH_INPUT_VALUE(1'b0),
		.CLOCK_ENABLE(1'b0),
		.INPUT_CLK(1'b0),
		.OUTPUT_CLK(1'b0),
		.OUTPUT_ENABLE(1'b1),
		.D_OUT_0(raw_tx),
		.D_OUT_1(1'b0),
		.D_IN_0(),
		.D_IN_1()
	);
	
	// RGB LED Driver IP core
	SB_RGBA_DRV #(
		.CURRENT_MODE("0b1"),
		.RGB0_CURRENT("0b000001"),
		.RGB1_CURRENT("0b000001"),
		.RGB2_CURRENT("0b000011")
	) RGBA_DRIVER (
		.CURREN(1'b1),
		.RGBLEDEN(1'b1),
		.RGB0PWM(gpio_o[17]),
		.RGB1PWM(gpio_o[18]),
		.RGB2PWM(gpio_o[19]),
		.RGB0(RGB0),
		.RGB1(RGB1),
		.RGB2(RGB2)
	);
	
	// LCD control lines
	assign LCD_NRST = gpio_o[31];
	assign LCD_DC = gpio_o[30];
	assign LCD_BK = gpio_o[29];

	// gate output
	assign GATE_OUT = gpio_o[28];
	
	// parallel/serial input
	wire [7:0] ps_data;
	wire ps_valid;
	ps_input upsin(
		.clk(clk),
		.reset(reset),
		.SER_DAT(SER_DAT),
		.SER_CLK(SER_CLK),
		.SER_SHF(SER_SHF),
		.ps_data(ps_data),
		.ps_valid(ps_valid)
	);

	// debounce button inputs
	wire [4:0] dbnc;
	wire dbnc_valid;
	debounce #(
		.WIDTH(5),
		.LEN(5),
		.DECIM(2667)
	) udbnc(
		.clk(clk),
		.reset(reset),
		.raw_valid(ps_valid),
		.raw_in({BTN,ps_data[7:4]}),
		.dbnc_out(dbnc),
		.dbnc_valid(dbnc_valid)
	);
	
	// decode A/B phases of rotary encoder
	wire signed [7:0] rot_val;
	rotary_decode urdcd(
		.clk(clk),
		.reset(reset),
		.ena(dbnc_valid),
		.enc_a(dbnc[0]),
		.enc_b(dbnc[1]),
		.clr(gpio_o[27]),
		.value(rot_val)
	);
	
	// hook up gp inputs
	wire curr_prog;
	assign gpio_i = {curr_prog,9'd0,rot_val,dbnc,BTN,ps_data};
	
	// ADC
	adc #(
		.PRESCALE(3'd3)	// 500kHz ADC_CLK to improve Tacq - Fsamp = 7.8kHz
	) uadc(
		.clk(clk),
		.reset(reset),
    
		.adc_cs(ADC_CS),
		.adc_si(ADC_DIN),
		.adc_clko(ADC_CLK),
		.adc_so(ADC_DOUT),
	
		.adc_data(adc_data),
	
		.a0_v(a3_v),	// swap order of ADC inputs to match CV naming
		.a1_v(a2_v),
		.a2_v(a1_v),
		.a3_v(a0_v)
	);
	
	// I2S I/O
	wire i2s_load;
	wire signed [15:0] left_rx, right_rx;
	wire signed [15:0] left_tx, right_tx;
	i2s_inout ui2s(
		.clk(AUDIOCLK),
		.reset(audio_reset),
		.l_in(left_tx),
		.r_in(right_tx),
		.l_out(left_rx),
		.r_out(right_rx),
		.sdout(I2S_DO),
		.sdin(I2S_DI),
		.sclk(I2S_SCLK),
		.lrclk(I2S_LRCK),
		.mclk(I2S_MCLK),
		.load(i2s_load)
	);
	
	// uncomment this to use the Midiverb model
`define MIDIVERB
`ifdef MIDIVERB
	// sum left + right for midiverb input
	wire signed [16:0] stereo_sum = {right_rx[15],right_rx} + 
									{left_rx[15],left_rx} + 16'd1;
	reg signed [15:0] mono;
	reg i2s_load_d1;
	always @(posedge AUDIOCLK)
		if(audio_reset)
		begin
			mono <= 16'd0;
			i2s_load_d1 <= 1'b0;
		end
		else
		begin
			if(i2s_load)
				mono <= stereo_sum[16:1];
			i2s_load_d1 <= i2s_load;
		end
		
	// clk divider for Midiverb
	reg [1:0] mv_clkdiv;
	reg mv_ena;
	always @(posedge AUDIOCLK)
	begin
		if(audio_reset)
		begin
			mv_clkdiv <= 2'd0;
			mv_ena <= 1'b0;
		end
		else
		begin
			mv_clkdiv <= mv_clkdiv + 2'd1;
			mv_ena <= mv_clkdiv == 2'b11;
		end
	end
	
	// Midiverb hardware implementation
	wire signed [15:0] left_fx, right_fx;
	mv umv(
		.clk(AUDIOCLK),
		.reset(audio_reset),
		.ena(mv_ena),
		.in(mono),
		.uc_w_clk(clk),
		.uc_w_addr(mv_w_addr),
		.uc_w_data(mv_w_data),
		.uc_we(mv_we),
		.prog(gpio_o[0]),
		//.prog(gpio_o[5:0]),
		.curr_prog(curr_prog),
		.out_l(left_fx),
		.out_r(right_fx),
		.valid()
	);	
`else
	// audio generator
	reg signed [15:0] left_fx, right_fx;
	always @(posedge AUDIOCLK)
		if(audio_reset)
		begin
			left_fx <= 16'h0000;
			right_fx <= 16'h0000;
		end
		else if(i2s_load)
		begin
			left_fx <= left_fx + 16'h0100;
			right_fx <= right_fx - 16'h0100;
		end
`endif
		
	// synchronize a3_v to AUDIOCLK domain by stretching it 8x
	reg [7:0] vspipe;
	always @(posedge clk)
		vspipe <= {vspipe[6:0],a3_v};
	wire vstretch = |vspipe;
	
	// wet dry mix codec input w/ generated audio
	wd_mix uwd_mix(
		.clk(AUDIOCLK),
		.reset(audio_reset),
		.ena(i2s_load),
		.mix(adc_data),
		.mix_ena(vstretch),
		.left_dry(left_rx),
		.right_dry(right_rx),
		.left_wet(left_fx),
		.right_wet(right_fx),
		.left_out(left_tx),
		.right_out(right_tx),
		.valid()
	);
		
	// synchronize lvl_ena to AUDIOCLK domain by stretching it 3x
	reg [2:0] lespipe;
	always @(posedge clk)
		lespipe <= {lespipe[1:0],lvl_ena};
	wire lestretch = |lespipe;
	
	// level detector
	level_det uld(
		.clk(AUDIOCLK),
		.reset(audio_reset),
		.ena(i2s_load),
		.left_rx(left_rx),
		.right_rx(right_rx),
		.left_tx(left_tx),
		.right_tx(right_tx),
		.lvl_ena(lestretch),
		.lvl_addr(lvl_addr),
		.lvl_do(lvl_do)
	);
endmodule
