      program rk
      real k1(0:1), k2(0:1), k3(0:1), k4(0:1)
      t = 0
      dt = 0.01
      x = 1
      v = 0
100   write(6,*) t, x, v
      k1(0) = rk_v(t, x, v) * dt
      k1(1) = rk_a(t, x, v) * dt
      k2(0) = rk_v(t+dt/2.0,x+k1(0)/2.0,v+k1(1)/2.0) * dt
      k2(1) = rk_a(t+dt/2.0,x+k1(0)/2.0,v+k1(1)/2.0) * dt
      k3(0) = rk_v(t+dt/2.0,x+k2(0)/2.0,v+k2(1)/2.0) * dt
      k3(1) = rk_a(t+dt/2.0,x+k2(0)/2.0,v+k2(1)/2.0) * dt
      k4(0) = rk_v(t+dt,x+k3(0),v+k3(1)) * dt
      k4(1) = rk_a(t+dt,x+k3(0),v+k3(1)) * dt
      dx = (k1(0)+2.0*k2(0)+2.0*k3(0)+k4(0)) / 6.0
      dv = (k1(1)+2.0*k2(1)+2.0*k3(1)+k4(1)) / 6.0
      x = x + dx
      v = v + dv
      t = t + dt
      if ( t .le. 10 ) goto 100
      end program rk

      function rk_v(t, x, v)
      rk_v = v 
      return
      end function rk_v

      function rk_a(t, x, v)
      k = 1.0
      m = 1.0
      a = 1.0
      rk_a = -1.0*(k/m)*x - (a/m)*v
      return
      end function rk_a
