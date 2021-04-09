`timescale 1ns / 1ps

module timeunit();
	initial $timeformat(-9, 1, " ns", 9);
endmodule

module test;
	reg	clk;
	reg	pause;
	reg	tx;
	reg	tx_debug;

	uart_hello dut(
        	.clk(clk),
        	.pause(pause)
	);
	
  always #1 clk = ~clk;
 
  initial begin
	  clk = 0;
	  pause = 0;

	  #1000;
	  $finish;
  end

	initial begin
		$dumpfile("test.vcd");
		$dumpvars;
	end
endmodule
