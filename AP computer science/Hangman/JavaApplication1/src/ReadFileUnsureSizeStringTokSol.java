import java.io.*;
import java.util.*;

/*File could look like this
 McCarthy|Walter|255 Grapevine Rd|Wenham|MA|01984|12000.00
 NaSmith|Courtney|7 Main St.|Clifton Park|NY|12065|18000.00
 Anderson|Trinity|957 First St.|Hermosa Beach|CA|01954|19000.00
 */
public class ReadFileUnsureSizeStringTokSol
{
  public static ArrayList<String> words = new<String> ArrayList();

  public ReadFileUnsureSizeStringTokSol()
  {
    read();
  }
  public void read()
  {
    try
    {
      BufferedReader input = new BufferedReader(new FileReader("Titles.txt"));

      String line;
      //Attempt to read from the file
      line = input.readLine();
      while (line != null) //goes to the end of file
      {
        
        StringTokenizer st = new StringTokenizer(line, "|"); //| is the delimiter
        //Now break up the line
        words.add(st.nextToken());
        

        
        line = input.readLine();
      }
      System.out.println(words);
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