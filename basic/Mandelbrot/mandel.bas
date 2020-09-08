  100 '    save "mandel.bas",a
  110 DIM ST$[20],SH$[20]
  120 ST$="0123456789ABCDEF"
  130 FOR Y=-12 TO 12
  140   FOR X=-39 TO 39
  150     CA=X*0.0458
  160     CB=Y*0.08333
  170     A=CA
  180     B=CB
  190     FOR I=0 TO 15
  200       T=A*A-B*B+CA
  210       B=2*A*B+CB
  220       A=T
  230       IF (A*A+B*B)>4 THEN GOTO 270
  240     NEXT I
  250     PRINT " ";
  260     GOTO 310
  270     REM
  280     SH$[0]=MID$(ST$,I+1,1)
  290 REM SH$[0]=ST$[0;(I+1)],1
  300     PRINT SH$[0];
  310   NEXT X
  320   PRINT
  330 NEXT Y
  340 END
