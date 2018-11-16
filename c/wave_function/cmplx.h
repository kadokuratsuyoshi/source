/*
  filename : cmplx1.h
  contents : original complex programs
*/

using namespace std;

class cmplx{
private:
public:
  double a;
  double b;
  cmplx();
  ~cmplx();
  cmplx( double x, double y );
  void operator =(cmplx x);
  cmplx operator +(cmplx x);
  cmplx operator -(cmplx x);
  cmplx operator *(cmplx x);
  cmplx operator /(cmplx x);
  cmplx exp_( cmplx x );
  cmplx conj( cmplx x );
  double re( cmplx x );
  double im( cmplx x );
  double norm( cmplx x );
  double arg( cmplx x );
  double abs( cmplx x );
}CMPLX;
cmplx::cmplx(){
}
cmplx::~cmplx(){
}
cmplx::cmplx( double x, double y ){
    a = x;
    b = y;
}
void cmplx::operator =( cmplx x ){
  a = x.a;
  b = x.b;
}
cmplx cmplx::operator +( cmplx x ){
  return cmplx( a + x.a, b + x.b );
}
cmplx cmplx::operator -( cmplx x ){
  return cmplx( a - x.a, b - x.b );
}
cmplx cmplx::operator *( cmplx x ){
  return cmplx( (a * x.a) - (b * x.b), (a * x.b) + (b * x.a) );
}
cmplx cmplx::operator /( cmplx x ){
  return cmplx( ((a * x.a)+(b * x.b)) / ((x.a * x.a)+(x.b * x.b)),
                ((b * x.a)-(a * x.b)) / ((x.a * x.a)+(x.b * x.b)) );
}
cmplx exp_( cmplx x ){
//  return cmplx( cos( x.a ), sin( x.b ) );
  return cmplx( exp(x.a)*cos( x.b ), exp(x.a)*sin( x.b ) );
}
cmplx conj( cmplx x ) {
  return cmplx( x.a, -x.b );
}
double re( cmplx x ){
  return x.a;
}
double im( cmplx x ){
  return x.b;
}
double norm( cmplx x ){
    return( (x.a * x.a) + (x.b * x.b) );
}
double arg( cmplx x ){
    return( x.a / x.b );
}
double abs( cmplx x ){
    return(sqrt((x.a * x.a) + (x.b * x.b)) );
}

// eof
