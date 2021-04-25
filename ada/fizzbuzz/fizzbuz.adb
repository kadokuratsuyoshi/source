with Ada.Text_IO;
use Ada.Text_IO;

procedure fizzbuzz is 

function fb (N : Integer) return Integer is begin
	if N mod 15 = 0 then
		Put_Line ("FizzBuzz");
	elsif N mod 3 = 0 then
		Put_Line ("Fizz");
	elsif N mod 5 = 0 then
		Put_Line ("Buzz");
	else
		Put_Line (Integer'Image (N)(2 .. Integer'Image(N)'Last));
	end if;
	return N*2;
end fb;

a : Integer;
begin
    for I in Integer range 1 .. 20 loop
		a := fb(I);
		Put_Line(a'img);
		New_Line;
    end loop;
end fizzbuzz;
