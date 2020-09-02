  100 OPEN "euler.txt" FOR OUTPUT AS #1
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 K = 1
  150 M = 1
  160 A = 1
  170 FOR I=0 TO 1000 STEP 1
  180   PRINT #1, T; X; V
  190   PRINT T;X;V
  200   DX = V * DT
  210   DV = (-  1!*(K/M)*X - (A/M)*V)*DT
  220   X = X + DX
  230   V = V + DV
  240   T = T + DT
  250 NEXT
  260 CLOSE #1
  270 END
