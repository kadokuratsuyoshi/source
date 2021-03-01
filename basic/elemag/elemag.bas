  100 '    save "elemag.bas",a
  110 PRINT "Design of ELECTROMAGNET "
  120 CURRDEN = 3 : REM [A/mm^2]  if <3A/mm^2 then not too hot
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
  280 N0=N/LENG
  290 H=N0 * MAXCURR
  300 PRINT "H :";H;"A/m"
  310 U0 = 1e-07*(4*3.14159)
  320 B = U0*H
  330 PRINT "B :";B;"T or Wb/m"
  340 S = 3.14159*((COILPHI/200)^2)
  350 V = S * 0.0001 : REM Gap thickness == 0.1mm
  360 W = (B*B/2*U0) * V : REM Total Energy W of Magnetical field
  370 REM F = grad W = W / 0.001
  380 PRINT "COIL CROSS SECTION :";S*100*100;"cm^2"
  390 F = (B*B*S)/(2*U0)
  400 F = F / 0.0001
  410 PRINT "F :";F;"N (0.2e-6N is the Force of wire between 1Adc, 1m gap)"
  420 END
