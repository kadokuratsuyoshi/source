module hello (
        rst,
        clk,
        d,
        q
    );
    input	rst;
    input	clk;
    input       d;
    output      q;
    reg         r;
        
    assign q = r;
    always @ (posedge clk or negedge rst ) begin
        if ( !rst ) begin
            r <= 1'b0;
        end
        else begin
            r <= ~d;
        end
    end
endmodule
//  eof
