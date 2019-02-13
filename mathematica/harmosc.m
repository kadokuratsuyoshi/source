sol = DSolve[{x''[t] + 2 zeta x'[t] + x[t] == 0, x[0] == 1, x' == 0}, x[t],]
  p1 = Plot[Re[x[t]] /. sol1[[1]] /. zeta -> 0.1, {t, 0, 10}, PlotRange -> All, PlotLabel -> "zeta=0.1"]
