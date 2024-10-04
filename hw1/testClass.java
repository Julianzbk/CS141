import java.lang.Runnable;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

public class testClass
{
    static final class util
    {
        static String redirectOutAndRun(Runnable statements)
        {
            final ByteArrayOutputStream oStream = new ByteArrayOutputStream();
            System.setOut(new PrintStream(oStream));
            statements.run();
            final String output = oStream.toString();
            System.setOut(System.out);
            return output;
        }
    }

    public static void main(String[] args)
    {
        String s = util.redirectOutAndRun(() -> {System.out.print("alalalalal");});
    }
}

/*
public class testClass
{
    @Test
    public void testSquarePrint()
    {
        Square sq = new Square("sq", 2);
        redirectOut(() -> {sq.print();});
        
        
    }
}
*/