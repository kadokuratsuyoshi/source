module spi_msb(
    input             CLK,
    input             RST,
    input             START,
    input[7:0]        TXDATA,
    input             RXSIG,
    output reg        TXSIG = 0,
    output reg[7:0]   RXDATA1,
    output reg[7:0]   RXDATA2,
    output reg        SPICLK = 1, //Mode3
	output			  CS_N
);

	reg[7:0] txbuff  = 8'h0;
	reg[7:0] rxbuff1 = 8'h0;
	reg[7:0] rxbuff2 = 8'h0;
	reg      spien   = 0;
	reg[8:0] clkCnt  = 9'b0;
	reg[4:0] dataCnt = 5'b0;
	reg		 rxf	 = 1'b0;
	reg[7:0] txcnt   = 5'h0;
	reg[7:0] spicnt  = 5'h0;
	reg      rx12    = 1'b0;

	parameter CLKFLAG = 9'd125;//CLK/(SPICLK*2)  ex)50MHz/(200kHz*2)
	parameter DATAFLAG = 9'd250;//CLK/SPICLK       ex)50MHz/200kHz*2

	always@(posedge CLK or negedge RST)begin
		if(!RST)begin
			txbuff  <= 8'h0;
			rxbuff1 <= 8'h0;
			rxbuff2 <= 8'h0;
			spien   <= 0;
			SPICLK  <= 1;//Mode3
			clkCnt  <= 9'b0;
			rxf     <= 1'b0;
			spicnt  <= 5'h0;
			rx12    <= 1'b0;
		end
		else begin
			if(spien)begin
				clkCnt <= clkCnt + 9'b1;
				if(clkCnt == CLKFLAG)begin
					SPICLK <= ~SPICLK;
					spicnt <= spicnt + 5'h1;
					if(!SPICLK)begin
						if (rxf == 1'b1) begin
							if (rx12 == 1'b0 ) begin
								rxbuff1[7:1] <= {rxbuff1[6:0]};
								rxbuff1[0]   <= RXSIG;
								if (spicnt > 15) begin
									rx12 = 1'b1;
								end
							end
							else begin
								rxbuff2[7:1] <= {rxbuff2[6:0]};
								rxbuff2[0]   <= RXSIG;
							end
						end
						dataCnt <= dataCnt + 5'b1;
					end
				end
				else if(clkCnt == DATAFLAG)begin
					if(dataCnt < 5'b11001)begin
						clkCnt <= 9'b0;
						SPICLK <= ~SPICLK;
						TXSIG  <= txbuff[7];
						txbuff <= {txbuff[6:0], 1'b0};
						txcnt  <= txcnt + 1;
						//if (txcnt > 6) begin
							rxf <= 1'b1;
						//end
					end
					else begin
						RXDATA1  <= rxbuff1;
						RXDATA2  <= rxbuff2;
						txbuff  <= 8'h0;
						//rxbuff1  <= 8'h0;
						//rxbuff2  <= 8'h0;
						spien   <= 0;
						clkCnt  <= 9'b0;
						dataCnt <= 5'b0;
						SPICLK  <= 1;//Mode3
						rx12    <= 1'b0;
					end
				end
			end
			else if(START)begin
				//txbuff  <= TXDATA;
				//rxbuff  <= 8'b0;
				spien   <= 1;
				clkCnt  <= 9'b0;
				dataCnt <= 5'b0;
				SPICLK  <= 0;//Mode3
				//TXSIG   <= txbuff[7];
				//txbuff  <= {txbuff[6:0], 1'b0};
				TXSIG   <= TXDATA[7];
				txbuff  <= {TXDATA[6:0], 1'b0};
				rxf     <= 1'b0;
				txcnt   <= 5'h0;
				spicnt  <= 5'h0;
				rx12    <= 1'b0;
			end
		end
	end

	assign CS_N = ~spien;

endmodule
