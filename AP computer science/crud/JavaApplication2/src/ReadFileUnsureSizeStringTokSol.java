
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class ReadFileUnsureSizeStringTokSol
{
    //public static ArrayList<Anime> shows = new<Anime> ArrayList();
  String title, genre;
  int rating,year,ep;
  

 /* public ReadFileUnsureSizeStringTokSol()
  {
    read();
  }*/
  public void read(ArrayList<Anime> shs)
  {
    try
    {
      BufferedReader input = new BufferedReader(new FileReader("data.txt"));

      String line;
      //Attempt to read from the file
      line = input.readLine();
      while (line != null) //goes to the end of file
      {
        
        StringTokenizer st = new StringTokenizer(line, "|"); //| is the delimiter
        //Now break up the line
        title = st.nextToken();
        genre = st.nextToken();
        rating= Integer.parseInt(st.nextToken());
        year  = Integer.parseInt(st.nextToken());
        ep    = Integer.parseInt(st.nextToken());
        shs.add(new Anime(title,genre,rating,year,ep));
          //System.out.println(shs+"showsssss");
        //animeList.addToList(new Anime(title,genre,rating,year,ep));
        //  System.out.println("shows are"+animeList.shows);
                

        System.out.println(">>>>>>>>>>>>>>line ="+ line);
        line = input.readLine();
          
        
      }
      input.close();
    }
    catch (Exception e)
    {
      System.out.println(e.toString());
    }
  }
  public static void main(String[] args)
  {
    //Demo use of args
    for (int i = 0; i < args.length; i++)
    {
      System.out.println(args[i]);
    }
    ReadFileUnsureSizeStringTokSol rfus = new ReadFileUnsureSizeStringTokSol();
  }
}