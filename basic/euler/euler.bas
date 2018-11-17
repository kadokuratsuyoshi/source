  100 OPEN "euler.txt" FOR OUTPUT AS #1
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 K = 1
  150 M = 1
  160 A = 1
  170 FOR T=0 TO 10 STEP 0.01
  180   DX = V + DT
  190   DV = (-  1!*(K/M)*X - (A/M)*V)*DT
  200   X = X + DX
  210   V = V + DV
  220   PRINT #1, T; X; V
  230   PRINT T;X;V
  240 NEXT
  250 CLOSE #1
  260 END
