import java.applet.Applet;
import java.util.Scanner;
import java.applet.AudioClip;


public class adshb {


    public double high, low;
    public boolean high2, low2;
    public Scanner scan;
    public AudioClip beep;


    public adshb(double hi, double lo, boolean hi2, boolean lo2, Scanner sc, AudioClip beep2) {
        high = hi;
        low = lo;
        high2 = hi2;
        low2 = lo2;
        scan = sc;
        beep = beep2;


    }


    public double readAndVerify() {
        double num = 0.0;
        if (high2 == true && low2 == true) {
            while (true) {


                try {
                    num = scan.nextDouble();


                } catch (Exception e) {
                    System.out.println("+=============+");
                    System.out.println("|Bad Character|");
                    System.out.println("+=============+");
         


                } finally {
                    scan.nextLine();
                }




                if (high >= num && num >= low) {
                    break;
                } else {
                    System.out.println("+==================+");
                    System.out.println("|Number Not Allowed|");
                    System.out.println("+==================+");
                    playSound();
                }
            }
        }
        if (high2 == true && low2 == false) {
            while (true) {


                try {
                    num = scan.nextDouble();


                } catch (Exception e) {
                    System.out.println("+=============+");
                    System.out.println("|Bad Character|");
                    System.out.println("+=============+");
                   


                } finally {
                    scan.nextLine();
                }




                if (high >= num && num > low) {
                    break;
                } else {
                    System.out.println("+==================+");
                    System.out.println("|Number Not Allowed|");
                    System.out.println("+==================+");
                    playSound();
                }
            }
        }
        if (high2 == false && low2 == true) {
            while (true) {


                try {
                    num = scan.nextDouble();


                } catch (Exception e) {
                    System.out.println("+=============+");
                    System.out.println("|Bad Character|");
                    System.out.println("+=============+");
                  


                } finally {
                    scan.nextLine();
                }




                if (high > num && num >= low) {
                    break;
                } else {
                    System.out.println("+==================+");
                    System.out.println("|Number Not Allowed|");
                    System.out.println("+==================+");
                    playSound();
                }
            }
        }
        if (high2 == false && low2 == false) {
            while (true) {


                try {
                    num = scan.nextDouble();


                } catch (Exception e) {
                    System.out.println("+=============+");
                    System.out.println("|Bad Character|");
                    System.out.println("+=============+");
                    


                } finally {
                    scan.nextLine();
                }




                if (high > num && num > low) {
                    break;
                } else {
                    System.out.println("+==================+");
                    System.out.println("|Number Not Allowed|");
                    System.out.println("+==================+");
                    playSound();
                }
            }
        }
        return num;


    }


    public static void playSound() {
        AudioClip beep = Applet.newAudioClip(ClassLoader.getSystemResource("sounds/Q.wav"));
        beep.play();
    }
}