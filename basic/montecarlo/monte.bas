   10 '     save "monte.bas",a
   20 N = 2^18
   30 A = 0
   40 FOR I=1 TO N
   50 X=RND
   60 Y=RND
   70 IF (X*X+Y*Y)<=1 THEN A = A + 1
   80 NEXT I
   90 PRINT (A/N) * 4
  100 END
