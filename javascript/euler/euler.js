// dx(t)/dt
var dxdt = function(x){
    var c = 0.001;
    var r = 100;
    var e = 10 ;
    return (e-x)/r/c;
};

// Euler method
var euler = function(x0, t0, tn, n){
    x = x0;
    t = t0;
    h = (tn - t0) /n;
    X = new Array(n);
    for(i=0; i<n; i++){
        x += dxdt(x) * h;
        X[i] = x;
        t = t0 + i*h;
//        console.log(t,x);
        //document.write(t);
        //document.write(" ");
        //document.write(x);
        //document.write("<br>");
    }
file = new File("euler.csv");
fflag = file.open("w","","");
if (fflag ) {
    txt = file.writeln("abc");
}
file.close();
    return X;
};

window.onload=function () {
    alert(euler(0, 0, 1, 100)); // display x(0)~x(1.0)
};
