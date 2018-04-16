
import java.util.Scanner;
import java.applet.Applet;
import java.applet.AudioClip;

public class testVerify {

    public static void main(String[] args) throws InterruptedException {
        System.out.println("give me a number and i will tell you if it fits our super secret specifications");
        Scanner input = new Scanner(System.in);
        AudioClip clip1 = Applet.newAudioClip(ClassLoader.getSystemResource("sounds/fonky.wav"));
        intVerfier dv1 = new intVerfier(5, -5, false, false, input, clip1);
        double x = dv1.readAndVerify();
    }
}
