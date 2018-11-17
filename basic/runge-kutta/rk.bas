  100 OPEN "rk.txt" FOR OUTPUT AS #1
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 FOR T=0 TO 10 STEP 0.01
  150   FT = T: FX = X: FV = V
  160   GOSUB *FUNC_RK_V
  170   K1(1) = RK_V * DT
  180   FT = T: FX = X: FV = V
  190   GOSUB *FUNC_RK_A
  200   K1(2) = RK_A * DT
  210   FT = T+DT/2: FX = X+K1(1)/2: FV = V+K1(2)/2
  220   GOSUB *FUNC_RK_V
  230   K2(1) = RK_V * DT
  240   FT = T+DT/2: FX = X+K1(1)/2: FV = V+K1(2)/2
  250   GOSUB *FUNC_RK_A
  260   K2(2) = RK_A * DT
  270   FT = T+DT/2: FX = X+K2(1)/2: FV = V+K2(2)/2
  280   GOSUB *FUNC_RK_V
  290   K3(1) = RK_V * DT
  300   FT = T+DT/2: FX = X+K2(1)/2: FV = V+K2(2)/2
  310   GOSUB *FUNC_RK_A
  320   K3(2) = RK_A * DT
  330   FT = T+DT: FX = X+K3(1): FV = V+K3(2)
  340   GOSUB *FUNC_RK_V
  350   K4(1) = RK_V * DT
  360   FT = T+DT: FX = X+K3(1): FV = V+K3(2)
  370   GOSUB *FUNC_RK_A
  380   K4(2) = RK_A * DT
  390   DX = (K1(1)+2*K2(1)+2*K3(1)+K4(1))/6
  400   DV = (K1(2)+2*K2(2)+2*K3(2)+K4(1))/6
  410   X = X + DX
  420   V = V + DV
  430   PRINT #1, T; X; V
  440   PRINT T; X; V
  450 NEXT
  460 CLOSE #1
  470 END
  480 *FUNC_RK_V
  490 RK_V = FV
  500 RETURN
  510 *FUNC_RK_A
  520 K=1
  530 M=1
  540 A=1
  550 RK_A = (-1*(K/M)*FX - (A/M)*FV)
  560 RETURN
