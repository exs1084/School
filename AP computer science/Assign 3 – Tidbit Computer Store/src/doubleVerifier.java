
import java.applet.Applet;
import java.util.Scanner;
import java.applet.AudioClip;

public class doubleVerifier {

    public double high, low;
    public boolean high2, low2;
    public Scanner scan;
    public AudioClip clip1;

    public doubleVerifier(double hi, double lo, boolean hi2, boolean lo2, Scanner sc, AudioClip clp) {
        high = hi;
        low = lo;
        high2 = hi2;
        low2 = lo2;
        scan = sc;
        clip1 = clp;
    }

    public double readAndVerify() throws InterruptedException {
        double num = 0.0;
        if (high2 == true && low2 == true) {
            while (true) {

                try {
                    num = scan.nextDouble();
                    if (high >= num && num >= low) {

                        break;
                    } else {
                        clip1.play();
                        Thread.sleep(2000);
                        clip1.stop();
                        System.out.println("That number isnt allowed");
                    }
//more logic here to decide range
//logic
                } catch (Exception e) {
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    System.out.println(" Bad Character");
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    clip1.play();
                    Thread.sleep(2000);
                    clip1.stop();

                } finally {
                    scan.nextLine();

                }


            }
        }
        if (high2 == true && low2 == false) {
            while (true) {

                try {
                    num = scan.nextDouble();
                    if (high >= num && num > low) {

                        break;
                    } else {
                        clip1.play();
                        Thread.sleep(2000);
                        clip1.stop();
                        System.out.println("That number isnt allowed");
                    }
//more logic here to decide range
//logic
                } catch (Exception e) {
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    System.out.println(" Bad Character");
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    clip1.play();
                    Thread.sleep(2000);
                    clip1.stop();
                } finally {
                    scan.nextLine();
                }


            }
        }
        if (high2 == false && low2 == true) {
            while (true) {

                try {
                    num = scan.nextDouble();
                    if (high > num && num >= low) {

                        break;
                    } else {
                        clip1.play();
                        Thread.sleep(2000);
                        clip1.stop();
                        System.out.println("That number isnt allowed");
                    }
//more logic here to decide range
//logic
                } catch (Exception e) {
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    System.out.println(" Bad Character");
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    clip1.play();
                    Thread.sleep(2000);
                    clip1.stop();
                } finally {
                    scan.nextLine();
                }


            }
        }
        if (high2 == false && low2 == false) {
            while (true) {

                try {
                    num = scan.nextDouble();
                    if (high > num && num > low) {

                        break;
                    } else {
                        clip1.play();
                        Thread.sleep(2000);
                        clip1.stop();
                        System.out.println("That number isnt allowed");
                    }
//more logic here to decide range
//logic
                } catch (Exception e) {
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    System.out.println(" Bad Character");
                    System.out.println("@@@@@@@@@@@@@@@@@@@@");
                    clip1.play();
                    Thread.sleep(2000);
                    clip1.stop();
                } finally {
                    scan.nextLine();
                }


            }
        }
        return num;
    }
}
