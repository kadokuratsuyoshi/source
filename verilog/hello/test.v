`timescale 1ns / 1ns
module timeunit();
	initial $timeformat(-9, 1, " ns", 9);
endmodule
module test;
	parameter RATE			= 15.625;
	// for input
	reg	rst;
	reg	clk;
	reg	d;
	// for output
	wire	q;
	// for test
	integer	i;
	hello hello(
        	.rst(rst),
        	.clk(clk),
        	.d(d),
        	.q(q)
	);
	
always #1 clk = ~clk;
initial begin
        #1;
        rst <= 1'b0;
	clk <= 1'b1;
        d <= 1'b0;
	#2;
        rst <= 1;
	for (i=0; i<100; i=i+1) begin
        	#4;
		if ( i == 10 ) begin
			d <= 1'b1;
            	end
            	if ( i == 20 ) begin
                	d <= 1'b0;
            	end
            	if ( i == 30 ) begin
                	d <= 1'b1;
            	end
            	if ( i == 40 ) begin
                	d <= 1'b0;
            	end
            	if ( i == 50 ) begin
                	d <= 1'b1;
            	end
            	if ( i == 60 ) begin
                	d <= 1'b0;
            	end
	end
        $display( "hello, world" );
        $display( " " );
	$finish;
end
initial begin
	$dumpfile("test.vcd");
	$dumpvars(0, test);
end
endmodule
