program rk(input, output);

function rk_v(t, x, v : real):real;
    rk_v := v
end;

function rk_a(t, x, v : real):real;
    var k, m, a : real;
begin
    k := 1.0;
    m := 1.0;
    a := 1.0;
    rk_a := -1.0*(k/m)*x - (a/m)*v
end;

    var i, tmax : integer;
    var tmax, t, dt, x, dx, v, dv : real;
    var ct, cx, cv : real;
    var k1, k2, k3, k4 : array [0,1] of real;
begin
    tmax := 1000;
    t := 0.0;
    dt := 0.01;
    x := 1.0;
    v := 0.0;
    for i:=0 to tmax do begin
        k1[0] := rk_v(t, x, v) * dt;
        k1[1] := rk_a(t, x, v) * dt;
        ct := t + dt/2.0;
        cx := x + k1[0]/2.0;
        cv := v + k1[1]/2.0;
        k2[0] := rk_v(ct, cx, cv) * dt;
        k2[1] := rk_a(ct, cx, cv) * dt;
        cx := x + k2[0]/2.0;
        cv := v + k2[1]/2.0;
        k3[0] := rk_v(ct, cx, cv) * dt;
        k3[1] := rk_a(ct, cx, cv) * dt;
        ct := t + dt;
        cx := x + k3[0];
        cv := v + k3[1];
        k4[0] := rk_v(ct, cx, cvt) * dt;
        k4[1] := rk_a(ct, cx, cvt) * dt;
        dx := (k1[0]+2.0*k2[0]+2.0*k3[0]+k4[0]) / 6.0;
        dv := (k1[1]+2.0*k2[1]+2.0*k3[1]+k4[1]) / 6.0;
        x := x + dx;
        v := v + dv;
        t := t + dt;
        writeln(t, ' ', x, ' ', v)
    end
end.
