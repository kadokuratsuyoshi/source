`timescale 1ns / 1ps

module timeunit();
	initial $timeformat(-9, 1, " ns", 9);
endmodule

module tram2_tb;
	parameter CYCLE = 10;
	parameter STEP = CYCLE/2;
	parameter STROBE = CYCLE-STEP-1;
	parameter PATNUM1 = 3;
	parameter PATNUM2 = 37;
	parameter TESTADDR1 = 8'haa;
	parameter TESTADDR2 = 8'h00;
	reg clk, rst, cs, we, oe;
	reg [7:0] addr;
	reg [7:0] write_data;
	reg [7:0] patmem1 [0:PATNUM1-1];
	reg [7:0] expmem1 [0:PATNUM1-1];
	reg [7:0] patmem2 [0:PATNUM2-1];
	reg [7:0] expmem2 [0:PATNUM2-1];
	wire [7:0] read_data;
	integer I;
	integer FD;

	tram2 dut(
		.CS(cs),
		.WE(we),
		.OE(oe),
		.A(addr),
		.D(write_data),
		.Q(read_data)
	);

	initial clk = 0;
	always #STEP clk = ~clk;	
	/*always begin
		clk = 1'b0;	#(STEP);
		clk = 1'b1; #(STEP);
	end*/
 
	initial begin
		FD = $fopen("passfail.txt");
		rst = 1'b0;
		addr = 8'hzz;
		write_data = 8'hzz;
		cs = 1'b1;
		we = 1'b1;
		oe = 1'b1; #(2*CYCLE);
		rst = 1'b1;	#(2*CYCLE);
		$readmemh("p1.pat", patmem1);
		for (I=0; I<PATNUM1; I=I+1) begin
			@(posedge clk) #(1); 
			addr = TESTADDR1 + I;					#(CYCLE);
			cs = 1'b0; we = 1'b0;					#(CYCLE);
			write_data = patmem1[I];				#(5*CYCLE);
			cs = 1'b1; we = 1'b1;					#(CYCLE);
			addr = 8'hzz; write_data = 8'hzz;		#(5*CYCLE);
		end
		$readmemh("e1.pat", expmem1);
		for (I=0; I<PATNUM1; I=I+1) begin
			@(posedge clk) #(1); 
			addr = TESTADDR1 + I;					#(CYCLE);
			cs = 1'b0; oe = 1'b0;					#(5*CYCLE);
			// judge
			if (expmem1[I] !== read_data) begin
				$fdisplay(FD, $stime, "ns FAIL : I=%03d, Addr=%04h, Exp=%02h, RD=%02h", I, addr, expmem1[I], read_data);
			end
			else begin
				$fdisplay(FD, $stime, "ns PASS : I=%03d, Addr=%04h, Exp=%02h, RD=%02h", I, addr, expmem1[I], read_data);
			end
			cs = 1'b1; oe = 1'b1;					#(CYCLE);
			addr = 8'hzz; 							#(5*CYCLE);
		end

		$readmemb("p2.pat", patmem2);
		for (I=0; I<PATNUM2; I=I+1) begin
			@(posedge clk) #(1); 
			addr = TESTADDR2 + I;					#(CYCLE);
			cs = 1'b0; we = 1'b0;					#(CYCLE);
			write_data = patmem2[I];				#(5*CYCLE);
			cs = 1'b1; we = 1'b1;					#(CYCLE);
			addr = 8'hzz; write_data = 8'hzz;		#(5*CYCLE);
		end
		$readmemb("p2.pat", expmem2);
		for (I=0; I<PATNUM2; I=I+1) begin
			@(posedge clk) #(1); 
			addr = TESTADDR2 + I;					#(CYCLE);
			cs = 1'b0; oe = 1'b0;					#(5*CYCLE);
			// judge
			if (expmem2[I] !== read_data) begin
				$fdisplay(FD, $stime, "ns FAIL : I=%03d, Addr=%04h, Exp=%08b, RD=%08b", I, addr, expmem2[I], read_data);
			end
			else begin
				$fdisplay(FD, $stime, "ns PASS : I=%03d, Addr=%04h, Exp=%08b, RD=%08b", I, addr, expmem2[I], read_data);
			end
			cs = 1'b1; oe = 1'b1;					#(CYCLE);
			addr = 8'hzz; 							#(5*CYCLE);
		end
		$fclose(FD);
		$finish;
	end

	initial begin
		$monitor($time," addr=%2h, write=%2h, read=%2h", addr, write_data, read_data);
		$dumpfile("test.vcd");
		$dumpvars;
	end
endmodule
