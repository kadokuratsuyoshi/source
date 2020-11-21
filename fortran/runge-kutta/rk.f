      program rk
      real rdx(0:3), kdv(0:3)
      t = 0
      x = 1
      v = 0
      dt = 0.01
100   write(6,*) t, x, v
      rdx(0) = dt * rk_v(t, x, v)
      rdx(1) = rk_v(t+dt/2,x+k1(0)/2,v+k1(1)/2) * dt
      rdx(2) = rk_v(t+dt/2,x+k2(0)/2,v+k2(1)/2) * dt
      rdx(3) = rk_v(t+dt,x+k3(0),v+k3(1)) * dt
      rdv(0) = dt * rk_a(t, x, v)
      rdv(1) = rk_a(t+dt/2,x+k1(0)/2,v+k1(1)/2) * dt
      rdv(2) = rk_a(t+dt/2,x+k2(0)/2,v+k2(1)/2) * dt
      rdv(3) = rk_a(t+dt,x+k3(0),v+k3(1)) * dt
      dx = (rdx(0)+2*(rdx(1)+rdx(2))+rdx(3)) / 6
      dv = (rdv(0)+2*(rdv(1)+rdv(2))+rdv(3)) / 6
      t = t + dt
      x = x + dx
      v = v + dv
      if ( t .le. 10 ) goto 100
      end program rk

      function rk_v(t, x, v)
      rk_v = v 
      return
      end function rk_v

      function rk_a(t, x, v)
      k = 1.0
      m = 1.0
      gamma = 1.0
      rk_a = ((-k*x-gamma*v)/m)
      return
      end function rk_a
