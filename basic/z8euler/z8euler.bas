  100 REM          save "z8euler.bas",a
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 K = 1
  150 M = 1
  160 A = 0.25
  170 I = 0
  180 FOR T=0 TO 10 STEP 0.01
  190   DX = V * DT
  200   DV = ((-K*X-A*V)/M)*DT
  210   X = X + DX
  220   V = V + DV
  230   IF I MOD  50 = 0 THEN PRINT SPACE$(20+10*X);"*"
  240   I = I + 1
  250 NEXT
  260 END
