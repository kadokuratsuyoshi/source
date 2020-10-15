module ring(out, clk, rst);
	output	reg [3:0] out;
	input	clk, rst;

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			out <= 4'b0001;
		end
		else begin
			out[0] <= out[1];
			out[1] <= out[2];
			out[2] <= out[3];
			out[3] <= out[0];
		end
	end
endmodule
