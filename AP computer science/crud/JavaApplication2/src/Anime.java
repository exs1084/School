/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author sodeerik
 */
public class Anime implements Comparable{
    public String title;
    public String genre;
    public int rating;
    public int year;//make vars
    public int numEp;
    
    public Anime(){
        
    }
    
    public Anime(String t, String g, int r, int y, int n){
        title=t;
        genre=g;//set vars
        rating=r;
        year=y;
        numEp=n;
    }
    public void setTitle(String t){
        title=t;
    }
    public void setGenre(String g){
        genre=g;
    }
    public void setRating(int r){
        rating=r;
    }
    public void setYear(int y){
        year=y;
    }
    public void setEp(int n){
        numEp=n;  
    }
    public String toString(){
        String temp ="";
        temp+=title+" ";
        temp+=genre+" ";
        temp+=rating+" ";
        temp+=year+" ";
        temp+=numEp;
        return temp;
    }

    @Override
    public int compareTo(Object t) {//compare ratings. return positive 1 if rating is greater than argumetns rating
        Anime temp = (Anime) t;
        if(this.rating>temp.rating){
            return 1;
        }else{
            return -1;
        }
    }
}
