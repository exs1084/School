
import java.text.DecimalFormat;
import java.util.Scanner;
import java.applet.Applet;
import java.applet.AudioClip;

public class TidbitStore {

    public static void main(String[] args) throws InterruptedException {
        AudioClip clip2 = Applet.newAudioClip(ClassLoader.getSystemResource("sounds/lunaVirus.wav"));//http://www.nicovideo.jp/watch/sm21900526
        clip2.play();
        DecimalFormat patternFormatter = new DecimalFormat("##, #00.00");
        AudioClip clip1 = Applet.newAudioClip(ClassLoader.getSystemResource("sounds/fonky.wav"));
        System.out.println("how much did your computer cost?");
        System.out.println("more than 0 and less than or equal to 12k please");
        Scanner input1 = new Scanner(System.in);
        doubleVerifier cost = new doubleVerifier(12000, 0, true, false, input1, clip1);
        double compCost = cost.readAndVerify();

        System.out.println("what is the annual rate?");
        System.out.println("more than 0 and less than or equal to 20% please");
        Scanner input2 = new Scanner(System.in);
        doubleVerifier rate = new doubleVerifier(20, 0, true, false, input2, clip1);
        double compRate = rate.readAndVerify();
        compRate=compRate/100;

        System.out.println("how much was your down payment?");
        System.out.println("more than or equal to 0 and less than or equal to 50% please");
        Scanner input3 = new Scanner(System.in);
        doubleVerifier down = new doubleVerifier(50, 0, true, true, input3, clip1);
        
        double compDown = down.readAndVerify();
        compDown=compDown/100;
        int i = 0;
        double downCost = compCost * compDown;
        compCost = compCost - downCost;
        System.out.println("down payment: $" + patternFormatter.format(downCost));
        double payment = (compCost) * .05;
        payment = round(payment);
        double sum = 0;
        System.out.println("month \t start balance \t interest \t principal \t payment \t end balance");
        while (true) {
            i++;
            compCost = round(compCost);
            System.out.print(i + "\t ");//month
            System.out.print("$" + patternFormatter.format(compCost) + "  \t ");//original balance
            double interest = (compRate / 12) * compCost;
            interest = round(interest);
            System.out.print("$" + patternFormatter.format(interest) + "  \t ");//interest
            sum = sum + interest;

            compCost = compCost + interest;//add interest
            payment = round(payment);

            if (compCost > payment) {//check if payment is bigger than the cost left
                compCost = compCost - payment;
                double principal = payment - interest;
                principal = round(principal);
                System.out.print("$" + patternFormatter.format(principal) + "\t ");//principal
                System.out.print("$" + patternFormatter.format(payment) + "\t ");//payment
            } else {
                System.out.print("$" + patternFormatter.format(compCost - interest) + "\t ");//principal
                System.out.print("$" + patternFormatter.format(compCost) + "\t ");//payment
                compCost = 0;
            }
            compCost = round(compCost);
            System.out.print("$" + patternFormatter.format(compCost));

            System.out.println("");



            if (compCost == 0.00) {
                sum = round(sum);
                System.out.println("total interest=$" + sum);
                break;
            }
        }
        //  System.exit(0);
    }

    public static double round(double num) {
        num = num * 100;
        num = (int) (num + .5);
        num = num / 100;
        return num;
    }
}
