   10 T=0
   20 X=10
   30 V=0
   40 DT=0.5
   50 M=1
   60 E=0.8
   70 G=1
   80 FOR I=0 TO 100
   90 IF I MOD 3=0 THEN PRINT SPACE$(20+X);"o"
  100 DX=DT*V
  110 DV=DT*(-G)
  120 T=T+DT
  130 X=X+DX
  140 V=V+DV
  150 IF X<0 THEN V=-(V*E)
  160 NEXT I
  170 END