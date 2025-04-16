package src;

import java.util.concurrent.Semaphore;

public class Main {
    public static void main(String[] args) {
        double[] xArrays = {0.0, 0.5, 1.0, Math.sqrt(2), 10.0, 100.0, 300.0};
        int numValues = xArrays.length;
        Series series = new Series(1e-6);

        Semaphore semaphore = new Semaphore(1);
        boolean[] append = {false};

        for (int i = 0; i < numValues; i++) {
            final int index = i;
            new Thread(() -> {
                try {
                    semaphore.acquire();
                    double x = xArrays[index];
                    double result = series.calc(x);
                    series.printResult(x, result, append[0]);
                    append[0] = true;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                } finally {
                    semaphore.release();
                }
            }).start();
        }
    }
}