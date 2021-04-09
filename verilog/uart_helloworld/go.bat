iverilog -o hello.out hello.v uart_hello.v send_signal.v uart.v baud_clk.v tx.v
vvp hello.out
gtkwave test.vcd
