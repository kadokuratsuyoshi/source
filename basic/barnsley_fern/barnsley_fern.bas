  100 '    save "barnsleyfern.bas",a
  110 CLS 3
  120 X=0:Y=0
  130 FOR I=1 TO 10000
  140 PSET(320+X*30,320-Y*30),4
  150 R=RND(1000)/1000
  160 IF R<0.01 THEN ELSE *ELSE1
  170 X1=0
  180 Y1=Y*0.16 : GOTO *ENDIF
  190 *ELSE1 : IF R<(0.01+0.07) THEN ELSE *ELSE2
  200 X1=X*0.2-Y*0.26
  210 Y1=X*0.23+Y*0.22+1.6:GOTO *ENDIF
  220 *ELSE2 : IF R<(0.01+0.07+0.07) THEN ELSE *ELSE3
  230 X1=Y*0.28-X*0.15
  240 Y1=X*0.26+Y*0.24+0.44:GOTO *ENDIF
  250 *ELSE3
  260 X1=X*0.85+Y*0.04
  270 Y1=Y*0.85-X*0.04+1.6
  280 *ENDIF
  290 X=X1:Y=Y1
  300 NEXT I
  310 END
