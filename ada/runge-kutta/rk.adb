with	Ada.Text_IO,
	Ada.Float_Text_IO;
use	Ada.Text_IO,
	Ada.Float_Text_IO;

function rk_v(t, x, v: Float) return Float is
begin
	return v;
end rk_v;

function rk_a(t, x, v: Float) return Float is
	k : Float := 1.0;
	m : Float := 1.0;
	a : Float := 1.0;
begin
	return (-1.0*(k/m)*x - (a/m)*v);
end rk_a;

procedure rk is
	t, dt, x, v, dx, dv: Float;
	k1 : array(0..1) of Float;
	k2 : array(0..1) of Float;
begin
	t := 0.0;
	dt := 0.01;
	x := 1.0;
	v := 0.0;
	loop
		Put_Line(t, x, v);
		k1(0) := rk_v(t, x, v) * dt;
		k1(1) := rk_a(t, x, v) * dt;
		k2(0) := rk_v(t+dt/2.0, x+k1(0)/2.0, v+k1(1)/2.0) * dt;
		k2(1) := rk_a(t+dt/2.0, x+k1(0)/2.0, v+k1(1)/2.0) * dt;
		k3(0) := rk_v(t+dt/2.0, x+k2(0)/2.0, v+k2(1)/2.0) * dt;
		k3(1) := rk_a(t+dt/2.0, x+k2(0)/2.0, v+k2(1)/2.0) * dt;
		k4(0) := rk_v(t+dt, x+k3(0), v+k3(1)) * dt;
		k4(1) := rk_a(t+dt, x+k3(0), v+k3(1)) * dt;
		dx := (k1(0)+2.0*k2(0)+2.0*k3(0)+k4(0))/6.0;
		dv := (k1(1)+2.0*k2(1)+2.0*k3(1)+k4(1))/6.0;
		x := x + dv;
		v := v + dv;
		t := t + dt;
		exit when t > 10.0;
	end loop;
end rk;
