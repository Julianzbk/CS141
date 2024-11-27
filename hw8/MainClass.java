import java.io.*;
import java.util.Hashtable;

class Disk
{
    static final int NUM_SECTORS = 2048;
    static final int DISK_DELAY = 80; // TODO change to 80
    StringBuffer sectors[] = new StringBuffer[NUM_SECTORS];
    int id;

    Disk(int id)
    {
        this.id = id;
    }

    void write(int sector, StringBuffer data)  // call sleep
    {
        sectors[sector] = data;
        try
        {
            Thread.sleep(DISK_DELAY);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    StringBuffer read(int sector)   // call sleep
    {
        StringBuffer data = sectors[sector];
        try
        {
            Thread.sleep(DISK_DELAY);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return data;
    }
}

class Printer
{
    static final int PRINT_DELAY = 275;
    int id;
    FileWriter fout;

    Printer(int id)
    {   
        this.id = id;
        try
        {
            fout = new FileWriter("PRINTER" + Integer.toString(id));
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    void print(StringBuffer data)  // call sleep
    {
        try
        {
            fout.write(data.toString());
            fout.write("\n");
            Thread.sleep(PRINT_DELAY);
            fout.flush();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}

class PrintJobThread
    extends Thread
{
    String file_name;
    StringBuffer line = new StringBuffer(); // only allowed one line to reuse for read from disk and print to printer

    PrintJobThread(String file_name)
    {
        this.file_name = file_name;
    }

    public void run()
    {
        OS141 OS = OS141.instance();
        FileInfo file_info = OS.dir_manager.lookup(file_name);
        //System.out.println("printjob<<" + file_info.toString());
        int id = OS.printer_manager.request();
        for (int sector = file_info.startingSector;
             sector < file_info.startingSector + file_info.fileLength; sector++)
        {
            line = OS.disks[file_info.diskNumber].sectors[sector];
            OS.printers[id].print(line);
        }
        OS.printer_manager.release(id);
    }
}

class FileInfo
{
    int diskNumber;
    int startingSector;
    int fileLength;

    FileInfo(int diskNumber, int startingSector, int fileLength)
    {
        this.diskNumber = diskNumber;
        this.startingSector = startingSector;
        this.fileLength = fileLength;
    }

    public String toString()
    {
        return "File(" + Integer.toString(diskNumber) + ", " + Integer.toString(startingSector) +
               ", " + Integer.toString(fileLength) + ")";
    }
}

class DirectoryManager
{
    private Hashtable<String, FileInfo> T = new Hashtable<String, FileInfo>();

    DirectoryManager()
    {
    }

    void enter(String fileName, FileInfo file)
    {
        T.put(fileName, file);
    }

    FileInfo lookup(String fileName)
    {
        return T.get(fileName.toString());
    }
}

class ResourceManager
    extends Thread
{
    boolean isFree[];
    ResourceManager(int numItems)
    {
        isFree = new boolean[numItems];
        for (int i = 0; i < numItems; ++i)
            isFree[i] = true;
    }

    synchronized int request()
    {
        while (true)
        {
            for (int i = 0; i < isFree.length; ++i)
            {
                if (isFree[i])
                {
                    isFree[i] = false;
                    return i;
                }
            }
            try
            {
                this.wait();
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }
    
    synchronized void release(int i)
    {
        isFree[i] = true;
        this.notify();  
    }

    public void run()
    {
    }
}

class DiskManager
    extends ResourceManager
{
    int[] next_sector;

    DiskManager(int num_disks)
    {
        super(num_disks);
        next_sector = new int[num_disks];
        for (int i = 0; i < num_disks; i++)
            next_sector[i] = 0;
    }

    synchronized void increment(int sector)
    {
        next_sector[sector]++;
    }
}

class PrinterManager
    extends ResourceManager
{
    PrinterManager(int num_printers)
    {
        super(num_printers);
    }
}

class UserThread
    extends Thread
{
    FileInputStream fin;
    BufferedReader reader; 
    
    UserThread(int id) throws FileNotFoundException// my commands come from an input file with name USERi where i is my user id
    {
        fin = new FileInputStream("USER" + Integer.toString(id));
        reader = new BufferedReader(new InputStreamReader(fin));
    }

    synchronized private void save(String file_name) throws IOException
    {
        OS141 OS = OS141.instance();
        int diskNumber = OS.disk_manager.request();
        int file_length = 0;
        int startingSector = OS.disk_manager.next_sector[diskNumber];
        for (String line; (line = reader.readLine()) != null; file_length++)
        {
            //System.out.println("reading_save>> " + line);
            if (line.contains(".end"))
            {
                break;
            }
            line = line.split(" ")[1];
            OS.disks[diskNumber].write(startingSector + file_length, new StringBuffer(line));
            OS.disk_manager.increment(diskNumber);
        }
        OS.dir_manager.enter(file_name, new FileInfo(diskNumber, startingSector, file_length));
        OS.disk_manager.release(diskNumber);
    }
    
    synchronized private void print(String file_name)
    {
        PrintJobThread print_thread = new PrintJobThread(file_name);
        print_thread.start();
    }

    public void run()
    {
        String file_name = "null";    
        try
        {
        for (String line; (line = reader.readLine()) != null; )
        {
            //System.out.println("reading>> " + line);
            if (line.contains(".save"))
            {
                save(line.substring(line.indexOf('A')));
            }
            else if (line.contains(".print"))
            {
                print(line.substring(line.indexOf('A')));
                
            }
            else
                throw new IOException("Invalid file input");
        }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}

class OS141
    // singleton
{
    int NUM_USERS;
    int NUM_DISKS;
    int NUM_PRINTERS;
    String user_file_names[];
    UserThread users[];
    Disk disks[];
    Printer printers[];
    DirectoryManager dir_manager;
    DiskManager disk_manager;
    PrinterManager printer_manager;
    
    private OS141(String[] args) throws FileNotFoundException
    {
        configure(args);
        users = new UserThread[NUM_USERS];
        user_file_names = new String[NUM_USERS];
        for (int i = 0; i < NUM_USERS; ++i)
        {
            users[i] = new UserThread(i);
            user_file_names[i] = "USER" + Integer.toString(i);
        }
        disks = new Disk[NUM_DISKS];
        for (int i = 0; i < NUM_DISKS; ++i)
        {
            disks[i] = new Disk(i);
        }
        printers = new Printer[NUM_PRINTERS];
        for (int i = 0; i < NUM_PRINTERS; ++i)
        {
            printers[i] = new Printer(i);
        }
        dir_manager = new DirectoryManager();
        disk_manager = new DiskManager(NUM_DISKS);
        printer_manager = new PrinterManager(NUM_PRINTERS);

    }

    private void configure(String[] args)
    {
        NUM_USERS = Integer.parseInt(args[0]);
        NUM_DISKS = Integer.parseInt(args[1]);
        NUM_PRINTERS = Integer.parseInt(args[2]);
    }

    void startUserThreads()
    {
        for (UserThread u: users)
            u.start();
    }

    void joinUserThreads()
    {
        try
        {
        for (UserThread u: users)
            u.join();
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }

    private static OS141 inst;
    
    public static OS141 instance()
    {
        if (inst == null)
        {
            throw new NullPointerException("Bad call on instance()");
        }
        return inst;
    }

    public static OS141 instance(String[] args)
    {
        if (inst == null)
        {
            try
            {
                inst = new OS141(args);
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }

        }
        return inst;
    }
}

public class MainClass
{
    static void test_hardware()
    {
        Disk[] disks = new Disk[2];
        for (int i = 0; i < 2; i++)
            disks[i] = new Disk(i);
        
        disks[1].write(3, new StringBuffer("hihihih"));
        System.out.println(disks[1].read(3).toString());

        Printer[] printers = new Printer[2];
        for (int i = 0; i < 2; i++)
            printers[i] = new Printer(i);

        printers[1].print(new StringBuffer("oiiaioiiiai"));
    }

    static void test_printjob_thread()
    {
        OS141 OS = OS141.instance();
        OS.disks[1].sectors[3] = new StringBuffer("owowo");
        OS.disks[1].sectors[4] = new StringBuffer("wuwuwu");
        OS.dir_manager.enter("nima.txt", new FileInfo(1, 3, 2));
        PrintJobThread pjt = new PrintJobThread("nima.txt");
        pjt.start();
    }

    public static void main(String[] args)
    {
        for (int i = 0; i < args.length; ++i)
        {
            //System.out.println("Args[" + i + "] = " + args[i]);
            args[i] = args[i].replaceFirst("-", "");
        }
        
        OS141 OS = OS141.instance(args);
        OS.startUserThreads();
        OS.joinUserThreads();

        System.out.println("*** 141 OS Simulation ***");
    }
}
