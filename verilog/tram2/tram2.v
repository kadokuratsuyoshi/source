module tram2(
	input CS,
	input WE,
	input OE,
	input [7:0] A,
	input [7:0] D,
	output [7:0] Q
);
	parameter WDELAY = 1; // 100;
	parameter RDELAY = 2; // 150;
	reg [7:0] MEM [0:255];

	always @ ( negedge CS or negedge WE or D or A) begin
		#WDELAY
		if ((CS == 1'b0) && (WE == 1'b0)) begin
			MEM[A] <= D;
		end
	end

	assign #RDELAY Q = ((!CS) && (!OE)) ? MEM[A]: 8'hzz;
endmodule
