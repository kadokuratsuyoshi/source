`timescale 1ns / 1ps
module lfsr_tb();
	reg clk_tb;
	reg rst_tb;
	wire [3:0] out_tb;

	always begin
		#5;
		clk_tb = ~ clk_tb;
	end

	initial begin
    	clk_tb = 0;
    	rst_tb = 1;
    	#15;
    
    	rst_tb = 0;
    	#400;

    	$display( " " );
    	$display( "sim. done" );
    	$display( " " );
		$finish;
	end

	lfsr DUT(out_tb,clk_tb,rst_tb);

    initial begin
		$dumpfile("test.vcd");
		$dumpvars;
    end
endmodule
