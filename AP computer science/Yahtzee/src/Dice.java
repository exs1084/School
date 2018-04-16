/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author sodeerik
 */
import java.util.*;
public class Dice {
   Random r = new Random();
    private int side;
    public Dice(int sides){
        side=sides;
    }
    public int roll(){
        int result;
        
        result = r.nextInt(side)+1;
        //System.out.println("in roll, res = " + result);
        return result;
    }
}
