/*
	RTL:		sw.v
	contents:	stop-watch
	make:		iverilog sw_tb.v sw.v -o sw.out
				vvp sw.out
				gtkwave test.vcd
	usage:		if start == 1 then stop-watch starts
*/
module stopwatch(clk, rst, start, stop, cnt);
	output	reg [3:0] cnt;
	input	clk, rst;
	input	start, stop;
	reg flg;

	always @(posedge clk or negedge rst) begin
		if (!rst) begin
			flg <= 1'b0;
			cnt <= 4'b0000;
		end
		if (start == 1'b1) begin
			flg <= 1;
		end
		if (stop == 1'b1) begin
			flg <= 0;
		end
		if (flg == 1'b1) begin
			cnt <= cnt + 4'b0001;
		end
	end
endmodule
