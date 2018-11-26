with	Ada.Text_IO,
	Ada.Float_Text_IO;
use	Ada.Text_IO,
	Ada.Float_Text_IO;

procedure euler is
	t, dt, x, v, dx, dv, k, m, a: Float;

begin
	t := 0.0;
	dt := 0.01;
	x := 1.0;
	v := 0.0;
	k := 1.0;
	m := 1.0;
	a := 1.0;
	loop
		Put(Item => Float'Adjacent(t, Float'First));
		Put(" ");
		Put(Item => Float'Adjacent(x, Float'First));
		Put(" ");
		Put(Item => Float'Adjacent(v, Float'First));
		Put_Line("");
		dx := v * dt;
		dv := (-1.0*(k/m)*x - (a/m)*v) * dt;
		x := x + dx;
		v := v + dv;
		t := t + dt;
		exit when t > 10.0;
	end loop;
end euler;
