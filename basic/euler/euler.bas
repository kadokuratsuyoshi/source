  100 OPEN "euler.txt" FOR OUTPUT AS #1
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 T = 0
  150 K = 1
  160 M = 1
  170 GAMMAA = 1
  180 FOR I=0 TO 1000 STEP 1
  190   PRINT #1, T; X; V
  200   PRINT T;X;V
  210   DX = V * DT
  220   DV = ( (-K*X - GAMMA*V)/M ) * DT
  230   X = X + DX
  240   V = V + DV
  250   T = T + DT
  260 NEXT
  270 CLOSE #1
  280 END
