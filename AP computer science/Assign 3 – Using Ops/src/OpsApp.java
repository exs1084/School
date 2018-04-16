import java.applet.Applet;
import java.applet.AudioClip;//possibly unessecary but im scared to remove them haha
public class OpsApp {
    static AudioClip clip1;
    
    public static void main(String[] args) {
        
        OppsFrame f = new OppsFrame();
        f.setBounds(50,50,550,550);//create window
        
        f.setVisible(true);
        
    }
}
