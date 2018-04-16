import java.util.*;
import javax.swing.JPanel;
public class HangmanApp {
    static Random r = new Random();
    static public HangmanProcessor a;
    
    static public String originalDisp;
    static ArrayList<String> words;
     
    public static void main(String[] args) {
        
        ReadFileUnsureSizeStringTokSol.main(args);
        
        words = ReadFileUnsureSizeStringTokSol.words;//read in
        System.out.println(words);
        int choice = r.nextInt(words.size());//choose random anime name
        a = new HangmanProcessor(words.get(choice));//create proccessor
        
         originalDisp=a.getDisplayWord();
         System.out.println( originalDisp);
         System.out.println(a.getWordToGuess());
        
        HangmanGui f;
        f = new HangmanGui();//creat gooey
        f.setBounds(50,50,750,550);
        
        f.setVisible(true);
        
        
        
        
       
        
    }
    
    public static String replaceLetters(String word){
        a.replaceLetters(word);
        return a.getDisplayWord();
        
    }
    
}
