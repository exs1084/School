
import java.util.ArrayList;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author sodeerik
 */
public class connectApp {
    
    public static void main(String[] args) {
         ArrayList<Integer> moves;
        connect4 c = new connect4();
        String turn = "|X|";
        ReadFileUnsureSizeStringTokSol re = new ReadFileUnsureSizeStringTokSol();
        moves=re.moves;
        int i=0;
        while(moves.size()>i){
            if(c.placePiece(turn, moves.get(i))==true){
                if(turn=="|X|"){
                    turn="|O|";
                }else{
                    turn="|X|";
                }
            }
            i++;
        }
        System.out.println("winner " +c.checkWin());
        c.print();
    }
}
