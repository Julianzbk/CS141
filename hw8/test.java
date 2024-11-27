class Task
    extends Thread
{
    String name;
    int period;

    Task(String name, int period)
    {
        this.name = name;
        this.period = period;
    }

    public void run()
    {
        System.out.println("Running " + name);
        while (true)
        {
            System.out.println(name);
            try
            {
                sleep(period);
            }
            catch (InterruptedException e)
            {
                System.out.println("💀");
            }
        }
    }
}

public class test
{
    static void meth(String arg1, String arg2)
    {
        arg2 = arg1;
    }

    public static void main(String args[]) throws InterruptedException
    {
        /*
        Task t1 = new Task("t1", 100);
        Task t2 = new Task("t2", 200);
        Task t3 = new Task("t3", 300);
        t1.start();
        t2.start();
        t3.start();
        */

        String s1 = new String("hihihi");
        String s2 = new String("asbdja");
        meth(s1, s2);
        System.out.println(s2);
    }
}
