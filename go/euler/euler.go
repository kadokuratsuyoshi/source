package main

import "fmt"

func main(){
    var t       float32 = 0.0
    var x       float32 = 1.0
    var v       float32 = 0.0
    var dt      float32 = 0.01
    var dx, dv  float32
    var k       float32 = 1.0
    var m       float32 = 1.0
    var gamma   float32 = 1.0

    for n=0; n<1000; n++ {
        fmt.Printf("%v %v %v\n", t, x, v)
        dx = dt * v
        dv = dt * ((-k*x-gamma*v)/m)
        x = x + dx
        v = v + dv
        t = t + dt
    }
}
