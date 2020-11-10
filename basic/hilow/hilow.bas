10 REM ****************************
20 REM ***Play the HI/LOW game ***
30 REM ****************************
40 C=0
50 N=RND(100)
60 INPUT "Guess a number?", G
70 IF G=N THEN 120
80 IF G>N THEN PRINT "Lower"
90 IF G<N THEN PRINT "Higher"
100 C=C+1
110 GOTO 50
120 PRINT "You guessed it in", C, " tries!"
130 END
