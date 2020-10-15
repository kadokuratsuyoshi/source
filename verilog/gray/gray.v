module gray(out, clk, rst);
	output	reg [3:0] out;
	input	clk, rst;
	reg		[4:0] q;

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			out <= 4'b0000;
			q <= 4'b0000;
		end
		else begin
			out <= {q[3], q[3:1] ^ q[2:0]};
			q <= q + 1;
		end
	end
endmodule
