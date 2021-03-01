  100 '    save "elemag.bas",a
  110 PRINT "Design of ELECTROMAGNET "
  120 CURRDEN = 3 : REM [A/mm^2]  if <3A/mm^2 then not too HOT
  130 PHI = 0.2
  140 PRINT "Diameter of wire";PHI;"mm"
  150 CROSS=3.14159*(PHI/2)*(PHI/2)
  160 PRINT "Cross Section:";CROSS;"mm^2"
  170 MAXCURR = CURRDEN * CROSS
  180 PRINT "Max Current:";MAXCURR;"A"
  190 E = 5 : REM [V]
  200 R = E / MAXCURR
  210 PRINT "E:";E;"V, R:";R;"ohm"
  220 DCR = 607.6 : REM [ohm/km]
  230 LENG = R / DCR * 1000
  240 PRINT "Wire Length :";LENG;"m"
  250 COILPHI = 5 : REM [cm]
  260 N=LENG/(COILPHI/100*3.14159)
  270 PRINT "Trun number :";N;"turn"
  280 END
