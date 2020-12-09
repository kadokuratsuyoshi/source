   10 '     save "resonat.bas",a
   20 CLS 3
   30 PI=3.14159265358979#
   40 R=1000 : L=0.53 : C=4.7e-10
   50 RL=0.001
   60 MAXZ=0
   70 FOR F=50 TO 20000 STEP 10
   80   A=R+RL
   90   B=(  2!*PI*F*L)-(  1!/(  2!*PI*F*C))
  100   Z=SQR(A*A+B*B)
  110   LINE-(F/40,200-(R/Z)*100)
  120   IF MAXZ<(R/Z) THEN MAXZ=R/Z:FQ=F
  130 NEXT F
  140 PRINT MAXZ,FQ;"Hz"
  150 END
