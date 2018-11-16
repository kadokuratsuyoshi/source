import java.io.*;

public class euler {
    public static void main (String [] args) {
        double t;
        double dt = 0.01;
        double x = 1.0;
        double v = 0.0;
        double dx, dv;
        double k = 1.0;
        double m = 1.0;
        double a = 1.0;

        for (t=0; t<10; t=t+dt ) {
            dx = v * dt;
            dv = (-1.0*(k/m)*x - (a/m)*v) * dt;
            x = x + dx;
            v = v + dv;
            System.out.printf("%f %f %f\n", t, x, v);
        }
    }
}
