import java.text.DecimalFormat;

abstract class Shape
{
    String name;
    DecimalFormat df;

    Shape(String name)
    {
        this.name = name;
        this.df = new DecimalFormat();
    }

    static boolean isWhole(double d)
    {
        return (d % 1 == 0) && (Math.floor(d) == d);
    }

    abstract void draw();

    abstract double area();

    abstract void print();
}

class Circle extends Shape
{
    int radius;

    Circle(String name, int radius)
    {
        super(name);
        this.radius = radius;
        df.setMaximumFractionDigits(2);
    }
    
    /*
    static String center(String s, int n)
    {
        int spaceSize = n - s.length();
        int prefixSize = spaceSize / 2;
        int suffixSize = (spaceSize + 1) / 2;
        return n > s.length()
                ? " ".repeat(prefixSize) + s + " ".repeat(suffixSize)
                : s;
    }

    void draw()
    {
        // fat circles at radius = 1, 2
        int maxWidth = radius * 2 + 1;
        for (int i = radius; i >= 0; i -= 1) // remove " - 1" for top vertex
        {
            int width = (int) Math.round(Math.sqrt(Math.pow(radius, 2) - Math.pow(i, 2)));
            System.out.println(center("* ".repeat(width * 2 + 1), maxWidth * 2));
        }
        System.out.println("* ".repeat(maxWidth));
        for (int i = 0; i <= radius; i += 1)     // to " <= radius" for top vertex
        {
            int width = (int) Math.round(Math.sqrt(Math.pow(radius, 2) - Math.pow(i, 2)));
            System.out.println(center("* ".repeat(width * 2 + 1), maxWidth * 2));        
        }
    }
    */

    void draw()
    {
        System.out.println("*"); // .repeat() is illegal ig
    }

    double area()
    {
        return Math.pow(radius, 2) * Math.PI;
    }
    
    void print()
    {
        System.out.print(name + "(" + radius + ") : " + df.format(area()));
    }
}

class Triangle extends Shape
{
    int base;
    int height;
    
    Triangle(String name, int base, int height)
    {
        super(name);
        this.base = base;
        this.height = height;
        this.df = new DecimalFormat("#");
    }

    void draw()
    {
        double slope = (double) height / (double) base;
        int width = 1;
        for (int i = 0; i < height; ++i)
        {
            for (int w = 0; w < width; ++w)
                System.out.print("* ");
            System.out.println();
            if (i % (int) Math.ceil(slope) == 0)
                width += (int) (1 / slope);
        }
        for (int b = 0; b < base; ++b)
            System.out.print("* ");
    }

    double area()
    {
        return 0.5 * base * height;
    }
    
    void print()
    {
        String a;
        if (isWhole(area()))
            a = df.format(area());
        else
            a = Double.toString(area());
        System.out.print(name + "(" + base + ", " + height + 
                           ") : " + a);
    }

}

class Square extends Shape
{
    int length;
    
    Square(String name, int length)
    {
        super(name);
        this.length = length;
        this.df = new DecimalFormat("#");
    }

    void draw()
    {
        for (int l = 0; l < length; ++l)
            System.out.print("* ");
        System.out.println();
        for (int i = 1; i < length - 1; ++i)
        {
            System.out.print("* ");
            for (int l = 0; l < length - 2; ++l)
                System.out.print("  ");
            System.out.println("*");
        }
        if (length > 1)
            for (int l = 0; l < length; ++l)
                System.out.print("* ");
    }
    
    double area()
    {
        return Math.pow(length, 2);
    }

    void print()
    {
        String a;
        if (isWhole(area()))
            a = df.format(area());
        else
            a = Double.toString(area());
        System.out.print(name + "(" + length + ") : " + a);
    }
}

class Rectangle extends Square
{
    int width;

    Rectangle(String name, int length, int width)
    {
        super(name, length);
        this.width = width;
        this.df = new DecimalFormat("#");
    }

    void draw()
    {
        if (length > 0)
            for (int w = 0; w < width; ++w)
                System.out.print("* ");
            System.out.println();
        for (int i = 1; i < length - 1; ++i)
        {
            if (width == 1)
                System.out.println("* ");
            else
            {
                System.out.print("* ");
                for (int w = 0; w < width - 2; ++w)
                    System.out.print("  ");
                System.out.println("*");
            }
        }
        if (length > 1)
            for (int w = 0; w < width; ++w)
                System.out.print("* ");
    }
    
    double area()
    {
        return length * width;
    }

    void print()
    {
        String a;
        if (isWhole(area()))
            a = df.format(area());
        else
            a = Double.toString(area());
        System.out.print(name + '(' + length + ", " +
                            width + ") : " + a);
    }
}

class ListNode
{
    Shape data;
    ListNode next;
    
    ListNode(Shape data, ListNode next)
    {
        this.data = data;
        this.next = next;
    }
}


class Picture
{
    ListNode head;

    Picture()
    {
        this.head = null;
    }

    void add(Shape sh)
    {
        if (head == null)
        {
            head = new ListNode(sh, null);
            return;
        }
        ListNode p = head;
        for (; p.next != null; p = p.next);
        p.next = new ListNode(sh, null);
    }

    Shape pop()
    {
        ListNode p = head;
        if (p == null)
            return null;
        if (p.next == null)
        {
            Shape item = p.data;
            head = null;
            return item;
        }

        for (; p.next.next != null; p = p.next);
        Shape item = p.next.data;
        p.next = null;
        return item;
    }

    void printAll()
    {
        boolean even = false;
        for (ListNode p = head; p != null; p = p.next)
        {
            p.data.print();
            System.out.println();
            if (even)
                System.out.println();
            even = even ? false : true;
        }
    }
    
    void drawAll()
    {
        for (ListNode p = head; p != null; p = p.next)
        {
            p.data.draw();
            System.out.println("\n");
        }
    }
    
    double totalArea()
    {
        double area = 0;
        for (ListNode p = head; p != null; p = p.next)
        {
            area += p.data.area();
        }
        return area;
    }

    int length()
    {
        int len = 0;
        for (ListNode p = head; p != null; p = p.next)
            len++;
        return len;
    }

    boolean isEmpty()
    {
        return head == null;
    }
}

public class mainClass
{
    public static void main(String[] args)
    {
        int arg1 = Integer.parseInt(args[0]);
        int arg2 = Integer.parseInt(args[1]);
        Picture pic = new Picture();
        pic.add(new Triangle("FirstTriangle", arg1, arg2));
        pic.add(new Triangle("SecondTriangle", arg1 - 1, arg2 - 1));
        pic.add(new Circle("FirstCircle", arg1));
        pic.add(new Circle("SecondCircle", arg1 - 1));
        pic.add(new Square("FirstSquare", arg1));
        pic.add(new Square("SecondSquare", arg1 - 1));
        pic.add(new Rectangle("FirstRectangle", arg1, arg2));
        pic.add(new Rectangle("SecondRectangle", arg1 - 1, arg2 - 1));
        pic.printAll();
        pic.drawAll();
        System.out.printf("Total : %.2f\n", pic.totalArea());
        //test();
    }
}

