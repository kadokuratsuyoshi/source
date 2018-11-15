      program euler
      t = 0
      dt = 0.01
      x = 1
      v = 0
100   write(6,*) t, x, v
      dx = dxdt(x, v, t) * dt
      dv = dvdt(x ,v, t) * dt
      x = x + dx
      v = v + dv
      t = t + dt
      if ( t .le. 10 ) goto 100
      end program euler

      function dxdt(x, v, t)
      dxdt = v 
      return
      end function dxdt

      function dvdt(x, v, t)
      k = 1.0
      m = 1.0
      a = 1.0
      dvdt = -1.0*(k/m)*x - (a/m)*v
      return
      end function dvdt
