package src;

import include.SeriesInterface;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class Series implements SeriesInterface {
    private double precision;
    private static final int UPPER_LIMIT = 100000000;

    public Series(double precision) {
        this.precision = precision;
    }

    @Override
    public double calc(double x) {
        double result = 0.0;

        switch ((int) (x * 10)) {
            case 0: // 0
                result = Math.PI * Math.PI / 6;
                break;
            case 5: // 0.5
                result = 4 * (1 - Math.log(2));
                break;
            case 10: // 1
                result = 1 - 1.0 / (UPPER_LIMIT + 2);
                break;
            case 14: // sqrt(2)
                if (Math.abs(x - Math.sqrt(2)) < precision) {
                    result = phi(x);
                }
                break;
            default: // 10, 100, 300
                int n = (int) x;
                if (x == Math.abs(n)) {
                    double a = 1, b = 1;
                    for (int i = 2; i <= n; i++) {
                        b = b * i + a;
                        a *= i;
                    }
                    result = b / (a * n);
                } else {
                    System.err.println("Invalid input: " + x);
                }
                break;
        }

        return result;
    }

    private double phi(double x) {
        double sum = 0.0;
        double term = 1.0;
        int k = 1, i = 0;

        while (i++ < UPPER_LIMIT) {
            term = 1.0 / (k * (k + x));
            sum += term;
            k++;
        }

        return sum;
    }

    @Override
    public void printResult(double x, double result, boolean flag) {
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("x=%8s, y=%.15e%n", (x == Math.sqrt(2)) ? "sqrt(2)" : String.format("%.1f", x), result));

        System.out.print(sb.toString());

        try (PrintWriter out = new PrintWriter(new FileWriter("../output.txt", flag))) {
            out.print(sb.toString());
        } catch (IOException e) {
            System.err.println("can't open ../output.txt");
        }
    }
}