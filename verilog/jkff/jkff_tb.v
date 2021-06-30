`timescale 1ns / 1ps

module timeunit();
    initial $timeformat(-9, 1, " ns", 9);
endmodule

module test;
	parameter CYCLE = 10;
	parameter STEP = CYCLE/2;

	reg             rst;
	reg				clk;
    reg             j;
	reg				k;
    wire            q;
    integer         i;
    jkff jkff(
        .rst(rst),
        .clk(clk),
        .j(j),
		.k(k),
        .q(q)
    );

	initial clk = 0;
    always #STEP clk = ~clk;

	initial begin
        rst         <= 1'b0;
        j           <= 1'b0;
		k			<= 1'b0;
		#2;
        rst         <= 1;
		for (i=0; i<100; i=i+1) begin
            #4;
            if ( i == 10 ) begin
                j <= 1'b0;
                k <= 1'b0;
            end
            if ( i == 20 ) begin
                j <= 1'b1;
                k <= 1'b0;
            end
            if ( i == 30 ) begin
                j <= 1'b0;
                k <= 1'b1;
            end
            if ( i == 40 ) begin
                j <= 1'b1;
                k <= 1'b1;
            end
            if ( i == 50 ) begin
                j <= 1'b0;
                k <= 1'b0;
            end
            if ( i == 60 ) begin
                j <= 1'b1;
                k <= 1'b0;
            end
			if ( i == 70 ) begin
                j <= 1'b0;
                k <= 1'b1;
			end
			if ( i == 80 ) begin
                j <= 1'b1;
                k <= 1'b1;
			end
			if ( i == 90 ) begin
                j <= 1'b0;
                k <= 1'b0;
			end
		end
		$finish;
	end

    initial begin
		//$monitor ($stime, " pc_cntr=%d", pc_cntr );
		$dumpfile("test.vcd");
		$dumpvars(0, test);
    end
endmodule
