
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;


public class animeList {
    public static ArrayList<Anime> shows = new<Anime> ArrayList();
    public static void main(String[] args) {
        //ReadFileUnsureSizeStringTokSol.main(args);
        
        
       /* for(int i=0;i<shows.size()-1;i++){
            if(shows.get(i).compareTo(shows.get(i+1))==1){
                Anime temp1=shows.get(i);
                shows.remove(i);
                shows.add(i+1, temp1);
                
            }
            
        }*/
        ReadFileUnsureSizeStringTokSol rs = new ReadFileUnsureSizeStringTokSol();
        rs.read(shows);
        System.out.println(shows.toString());
        System.out.println("lets change something. what should "+shows.get(0).title+"'s new rating be?");
        Scanner in = new Scanner(System.in);
        int rat = in.nextInt();
        shows.get(0).setRating(rat);
        System.out.println(shows.toString());
        System.out.println("which shows should be removed from the list?(type 1 for the first, 2 for the second, etc");
        int choice=in.nextInt();
        shows.remove(choice-1);
        System.out.println(shows.toString());
        
        try {
            PrintWriter p = new PrintWriter(new FileWriter("data.txt"));
            PrintWriter w = new PrintWriter(new FileWriter("backup.txt"));
            for(int i=0;i<shows.size();i++){
                p.print(shows.get(i).title+"|"+shows.get(i).genre+"|"+shows.get(i).rating+"|"+shows.get(i).year+"|"+shows.get(i).numEp);
                p.println();
                w.print(shows.get(i).title+"|"+shows.get(i).genre+"|"+shows.get(i).rating+"|"+shows.get(i).year+"|"+shows.get(i).numEp);
                w.println();
            }
            p.close();
            w.close();
        }catch(Exception e){
            
        }
    }
    public static void addToList(Anime a){
        shows.add(a);
    }
}
