  100 REM          save "z8euler.bas",a
  110 DT = 0.01
  120 X = 1
  130 V = 0
  140 T = 0
  160 K = 1
  170 M = 1
  180 A = 0.25
  190 FOR I = 0 TO 1000
  200   IF I MOD  50 = 0 THEN PRINT SPACE$(20+10*X);"*"
  210   DX = V * DT
  220   DV = ((-K*X-A*V)/M)*DT
  230   X = X + DX
  240   V = V + DV
  250   T = T + DT
  260 NEXT
  270 END
