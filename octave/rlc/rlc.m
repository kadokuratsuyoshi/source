% RLC series oscillation circuit
R = 1;
L = 0.1;
C = 0.01;

A = [0 1;-1/(L*C) -R/L];
B = [0 ; 1/(L*C)];
C = [1 0];
D = [0];

t = 0:0.01:10;
u = ones(length(t),1);
u(200:400) = 0;

x0 = [1 0];
sys = ss(A,B,C,D);
[y,t,x] = lsim(sys,u,t,x0);
plot(t,y,'r-',t,u,'b--');xlabel('time');ylabel('voltage');legend('v2(t)','u(t)');
