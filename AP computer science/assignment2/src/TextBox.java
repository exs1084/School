
import java.util.Scanner;

public class TextBox {

    public static void main(String[] args) {
        System.out.println("say something to me please. i want to border your thoughts...in some tilda");
        Scanner input = new Scanner(System.in);
        String str = input.nextLine();//SPACES BREAK THE SAUCE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        int length = str.length();
        if (length > 20) {
            str = str.substring(0, 20);
            length=20;
        }
        for (int i = 0; i <= (length + 5); i++) {//top border
            System.out.print("~");

        }
        System.out.println("~");
        System.out.print("~~");
        for (int b = 0; b <= (length + 2); b++) {//empty line
            System.out.print(" ");
        }
        System.out.println("~~");

        System.out.println("~~ " + str + " "
                + " ~~");//text line
        System.out.print("~~");
        for (int b = 0; b <= (length + 2); b++) {//empty line
            System.out.print(" ");
        }
        System.out.println("~~");
        for (int i = 0; i <= (length + 5); i++) {//bottom border
            System.out.print("~");

        }
        System.out.println("~");
    }
}
