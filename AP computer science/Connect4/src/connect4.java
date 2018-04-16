/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author sodeerik
 */
public class connect4 {

    public String[][] pieces = new String[6][7];

    public connect4() {
        for (int r = 0; r < pieces.length; r++) {
            for (int c = 0; c < pieces[0].length; c++) {
                pieces[r][c] = "|#|";
            }
        }

        print();

    }

    public boolean placePiece(String s, int r) {
        if (r > pieces[0].length - 1) {
            System.out.println("illegal move--nonexistant row");
            return false;
        }
        if (pieces[0][r] != "|#|") {
            System.out.println("illegal move--full row");
            return false;
        }
        for (int c = pieces.length - 1; c >= 0; c--) {
            if (pieces[c][r] == "|#|") {
                pieces[c][r] = s;
                break;
            }
        }


        return true;
    }

    public String checkWin() {
        
        //0=not over, 1=X wins, 2=O wins
        for (int r = 0; r < pieces.length; r++) {
            for (int c = 0; c < pieces[0].length; c++) {
                System.out.println("looping");
                String t = pieces[r][c];
                if(!t.equals("|#|")){
                try {
                    
                    if(t.equals(pieces[r][c+1])&&t.equals(pieces[r][c+2])&&t.equals(pieces[r][c+3])){//check same column positive--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("->");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r][c-1])&&t.equals(pieces[r][c-2])&&t.equals(pieces[r][c-3])){//check same column negative--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("<-");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r+1][c])&&t.equals(pieces[r+2][c])&&t.equals(pieces[r+3][c])){//check same row positive--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("vv");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r-1][c])&&t.equals(pieces[r-2][c])&&t.equals(pieces[r-3][c])){//check same row negative--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("^^");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r+1][c+1])&&t.equals(pieces[r+2][c+2])&&t.equals(pieces[r+3][c+3])){//check diag all pos--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("v>");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r-1][c-1])&&t.equals(pieces[r-2][c-2])&&t.equals(pieces[r-3][c-3])){//check diag all neg--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("<^");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r+1][c-1])&&t.equals(pieces[r+2][c-2])&&t.equals(pieces[r+3][c-3])){//check diag pos row neg col--done
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("<v");
                        return t;
                    }
                    } catch (Exception e) {}
                try {
                    
                    if(t.equals(pieces[r-1][c+1])&&t.equals(pieces[r-2][c+2])&&t.equals(pieces[r-3][c+3])){//check diag neg row pos col
                        System.out.println("row "+ r +" column "+c+" "+t);
                        System.out.println("^>");
                        return t;
                    }
                    } catch (Exception e) {}
            }}

        }

        return "none. its a tie.";

    }

    public void print() {
        for (int r = 0; r < pieces.length; r++) {
            for (int c = 0; c < pieces[0].length; c++) {
                System.out.print(pieces[r][c]);
            }
            System.out.println("");
        }
    }
}
