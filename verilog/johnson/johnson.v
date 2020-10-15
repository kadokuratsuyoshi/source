module johnson(out, clk, rst);
	output	reg [3:0] out;
	input	clk, rst;

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			out <= 4'b0;
		end
		else begin
			out <= out << 1;
			out[0] <= ~out[3];
		end
	end
endmodule
