//	8bit linear feedback shift register
module lfsr (out, clk, rst);

	output	reg [3:0] out;
	input	clk, rst;
	wire	fb;
	assign	fb = ~(out[3] ^ out[2]);

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			out <= 4'b0;
		end
		else begin
			out <= {out[2:0],fb};
		end
	end
endmodule
