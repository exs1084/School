
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class ReadFileUnsureSizeStringTokSol
{
  int move;
  public ArrayList<Integer> moves = new<Integer> ArrayList();

  public ReadFileUnsureSizeStringTokSol()
  {
    read();
  }
  public void read()
  {
    try
    {
      BufferedReader input = new BufferedReader(new FileReader("data4.txt"));

      String line;
      //Attempt to read from the file
      line = input.readLine();
      while (line != null) //goes to the end of file
      {
        System.out.println("inside the loop");
        StringTokenizer st = new StringTokenizer(line, "|"); //| is the delimiter
        //Now break up the line
        move = Integer.parseInt(st.nextToken());
        moves.add(move);

          System.out.println(move);
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