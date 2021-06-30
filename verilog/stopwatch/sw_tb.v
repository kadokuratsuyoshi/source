`timescale 1ps/1ps
module sw_tb;
	reg		ck, res, start, stop;
	wire	[3:0] q;
	parameter	STEP = 10000;

	stopwatch sw(ck, res, start, stop, q);

	always #(STEP/2) ck = ~ck;

	initial begin
		ck = 0;
		res = 1;
		start = 0;
		stop = 0;			#STEP
		res = 0;			#STEP
		res = 1;			#(STEP*5)
		start = 1; stop = 0;#(STEP*20)
		start = 0; stop = 1;#(STEP*5)
		res = 0;			#(STEP*5) 
		$finish;
	end

	initial $monitor($time, "ck=%b, res=%b, q=%h", ck, res, q);

    initial begin
		$dumpfile("test.vcd");
		$dumpvars;
    end
endmodule
