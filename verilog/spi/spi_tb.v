`timescale 1 ns/ 100 ps
module spi_msb_vlg_tst();
	// constants                                           
	// general purpose registers
	reg eachvec;
	// test vector input registers
	reg CLK;
	reg RST;
	reg RXSIG;
	reg START;
	reg [7:0] TXDATA;
	// wires                                               
	wire [7:0]  RXDATA1;
	wire [7:0]  RXDATA2;
	wire SPICLK;
	wire TXSIG;

	// assign statements (if any)                          
	spi_msb i1 (
	// port map - connection between master ports and signals/registers   
    	.CLK(CLK),
    	.RST(RST),
    	.RXDATA1(RXDATA1),
    	.RXDATA2(RXDATA2),
    	.RXSIG(RXSIG),
    	.SPICLK(SPICLK),
    	.START(START),
    	.TXDATA(TXDATA),
    	.TXSIG(TXSIG)
	);

	always #10 CLK = ~CLK;

	initial begin    
		CLK = 1;
		RST = 1;
		START = 0;
		TXDATA = 8'b01010100;
		RXSIG = 0;

		#2000 START = 1;
		#100 RXSIG = 1;
     	START = 0;

		// tx phase
		#40000;

		// rx phase
		#5000 RXSIG = 1;	// 0x82
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 1;
		#5000 RXSIG = 0;

		#5000 RXSIG = 1;	// 0xc5
		#5000 RXSIG = 1;
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 0;
		#5000 RXSIG = 1;
		#5000 RXSIG = 0;
		#5000 RXSIG = 1;

		//#2000 START = 1;
		//#100;
     	//START = 0;

		#50000;
		#50000;

		$display("Running testbench");                       
		$finish;
	end                                                    

	always begin                                                  
		@eachvec;                                              
	end                                                    

	initial begin
		$dumpfile("test.vcd");
		$dumpvars;
	end
endmodule
