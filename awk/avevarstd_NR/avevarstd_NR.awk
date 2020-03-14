BEGIN{
  sum=0;
  j=0;
}
{
  line[NR]=$3;
  i=NR;
  if ( (i>=100) && (i<300) ) {
    sum+=$3;
    j++;
  }
}
END{
  ave=sum/j;
  varTemp=0;
  for (i=1; i<=NR; i++) {
    if ( (i>=100) && (i<300) ) {
      varTemp+=(line[i]-ave)^2;
    }
  }
  var=varTemp/j; # =varTemp/(j-1)
  std=sqrt(var);
  print "NR", "j", "sum", "ave", "var", "std";
  print NR, j, sum, ave, var, std;
}
