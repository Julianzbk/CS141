#include <iostream>
#include <iomanip>
#include <malloc.h>
#include <math.h>
#include <string.h>

using std::cout, std::endl;


typedef double(*double_method_type)(void*);
typedef void(*void_method_type)(void*);

typedef union
{
    double_method_type double_method;
    void_method_type void_method;
} vTableEntry;

typedef vTableEntry* vPointer;

enum index
{
    DESTRUCTOR_INDEX = 0,
    DRAW_INDEX = 1,
    AREA_INDEX,
    PRINT_INDEX,
};

//class Shape
struct Shape
{
    vPointer v_ptr;
    char* name;
};
typedef struct Shape Shape;
// Forward declaration of virtual methods:
void Shape_Destructor(Shape* _this);

/*
    1. Cast function ptr to respective return types but with void* arguments (for _this).
    2. Assign that ptr to anonymous union initialized within vTable.
*/
vTableEntry Shape_vTable[] = {{.void_method = (void_method_type) Shape_Destructor}};

void Shape_Shape(Shape* _this, const char* name)
{
    _this->v_ptr = Shape_vTable;
    _this->name = (char*) malloc(strlen(name) + 1); 
    strcpy(_this->name, name);
}

void Shape_Destructor(Shape* _this)
{
    free(_this->name);
}

// class Circle extends Shape
struct Circle
{
    vPointer v_ptr;
    Shape* super;
    int radius;
};
typedef struct Circle Circle;

void Circle_Destructor(Circle* _this);
void Circle_draw(Circle* _this);
double Circle_area(Circle* _this);
void Circle_print(Circle* _this);

#define void_cast(method) {.void_method = (void_method_type) method}
#define double_cast(method) {.double_method = (double_method_type) method}
vTableEntry Circle_vTable[] = {void_cast(Circle_Destructor), void_cast(Circle_draw),
                               double_cast(Circle_area), void_cast(Circle_print)};

void Circle_Circle(Circle* _this, const char* name, int radius)
{
    _this->v_ptr = Circle_vTable;
    _this->super = (Shape*) malloc(sizeof(Shape));
    Shape_Shape(_this->super, name);
    _this->radius = radius;
}

void Circle_draw(Circle* _this)
{
    cout << "**\n**" << endl ; // this is a circle i swear
}

double Circle_area(Circle* _this)
{
    return M_PI * pow(_this->radius, 2);
}

void Circle_print(Circle* _this)
{
    double area = (_this->v_ptr[AREA_INDEX].double_method)(_this);
    cout << _this->super->name << "(" << _this->radius << ") : ";
    if (area == (int) area)
        cout << std::fixed << std::setprecision(0) << area << endl;
    else
        cout << std::fixed << std::setprecision(2) << area << endl;
}

void Circle_Destructor(Circle* _this)
{
    Shape_Destructor(_this->super);
    free(_this->super);
}

//class Triangle extends Shape
struct Triangle
{
    vPointer v_ptr;
    Shape* super;
    int base;
    int height;
};
typedef struct Triangle Triangle;

void Triangle_Destructor(Triangle* _this);
void Triangle_draw(Triangle* _this);
double Triangle_area(Triangle* _this);
void Triangle_print(Triangle* _this);

vTableEntry Triangle_vTable[] = {void_cast(Triangle_Destructor), void_cast(Triangle_draw),
                               double_cast(Triangle_area), void_cast(Triangle_print)};

void Triangle_Triangle(Triangle* _this, const char* name, int base, int height)
{
    _this->v_ptr = Triangle_vTable;
    _this->super = (Shape*) malloc(sizeof(Shape));
    Shape_Shape(_this->super, name);
    _this->base = base;
    _this->height = height;
}

void Triangle_draw(Triangle* _this)
{
    int height = _this->height;
    int base = _this->base;

    double slope = (double) height / (double) base;
    int width = 1;
    for (int i = 0; i < height; ++i)
    {
        for (int w = 0; w < width; ++w)
            cout << "* ";
        cout << endl;
        if (i % (int) ceil(slope) == 0)
            width += (int) (1 / slope);
    }
    for (int b = 0; b < base; ++b)
        cout << "* ";
    cout << endl;
}

double Triangle_area(Triangle* _this)
{
    return _this->base * _this->height / 2;
}

void Triangle_print(Triangle* _this)
{
    double area = (_this->v_ptr[AREA_INDEX].double_method)(_this);
    cout << _this->super->name << "(" << _this->base << ", " << _this->height << ") : ";
    if (area == (int) area)
        cout << std::fixed << std::setprecision(0) << area << endl;
    else
        cout << std::fixed << std::setprecision(2) << area << endl;
}

void Triangle_Destructor(Triangle* _this)
{
    Shape_Destructor(_this->super);
    free(_this->super);
}

// class Square
struct Square
{
    vPointer v_ptr;
    Shape* super;
    int length;
};
typedef struct Square Square;

void Square_Destructor(Square* _this);
void Square_draw(Square* _this);
double Square_area(Square* _this);
void Square_print(Square* _this);

vTableEntry Square_vTable[] = {void_cast(Square_Destructor), void_cast(Square_draw),
                               double_cast(Square_area), void_cast(Square_print)};

void Square_Square(Square* _this, const char* name, int length)
{
    _this->v_ptr = Square_vTable;
    _this->super = (Shape*) malloc(sizeof(Shape));
    Shape_Shape(_this->super, name);
    _this->length = length;
}

void Square_draw(Square* _this)
{
    int length = _this->length;
    for (int l = 0; l < length; ++l)
        cout << "* ";
    cout << endl;
    for (int i = 1; i < length - 1; ++i)
    {
        cout << "* ";
        for (int l = 0; l < length - 2; ++l)
            cout << "  ";
        cout << "*" << endl;
    }
    if (length > 1)
        for (int l = 0; l < length; ++l)
            cout << "* ";
    cout << endl;
}

double Square_area(Square* _this)
{
    return pow(_this->length, 2);
}

void Square_print(Square* _this)
{
    double area = (_this->v_ptr[AREA_INDEX].double_method)(_this);
    cout << _this->super->name << "(" << _this->length << ") : ";
    if (area == (int) area)
        cout << std::fixed << std::setprecision(0) << area << endl;
    else
        cout << std::fixed << std::setprecision(2) << area << endl;
}

void Square_Destructor(Square* _this)
{
    Shape_Destructor(_this->super);
    free(_this->super);
}


// class Rectangle
struct Rectangle
{
    vPointer v_ptr;
    Shape* super;
    int length;
    int width;
};
typedef struct Rectangle Rectangle;

void Rectangle_Destructor(Rectangle* _this);
void Rectangle_draw(Rectangle* _this);
double Rectangle_area(Rectangle* _this);
void Rectangle_print(Rectangle* _this);

vTableEntry Rectangle_vTable[] = {void_cast(Rectangle_Destructor), void_cast(Rectangle_draw),
                               double_cast(Rectangle_area), void_cast(Rectangle_print)};

void Rectangle_Rectangle(Rectangle* _this, const char* name, int length, int width)
{
    _this->v_ptr = Rectangle_vTable;
    _this->super = (Shape*) malloc(sizeof(Shape));
    Shape_Shape(_this->super, name);
    _this->length = length;
    _this->width = width;
}

void Rectangle_draw(Rectangle* _this)
{
    int length = _this->length;
    int width = _this->width;
    if (length > 0)
        for (int w = 0; w < width; ++w)
            cout << "* ";
    cout << endl;
    for (int i = 1; i < length - 1; ++i)
    {
        if (width == 1)
            cout << "* ";
        else
        {
            cout << "* ";
            for (int w = 0; w < width - 2; ++w)
                cout << "  ";
            cout << "*" << endl;
        }
    }
    if (length > 1)
        for (int w = 0; w < width; ++w)
            cout << "* ";
    cout << endl;
}

double Rectangle_area(Rectangle* _this)
{
    return _this->length * _this->width;
}

void Rectangle_print(Rectangle* _this)
{
    double area = (_this->v_ptr[AREA_INDEX].double_method)(_this);
    cout << _this->super->name << "(" << _this->length << ", " << _this->width << ") : ";
    if (area == (int) area)
        cout << std::fixed << std::setprecision(0) << area << endl;
    else
        cout << std::fixed << std::setprecision(2) << area << endl;
}

void Rectangle_Destructor(Rectangle* _this)
{
    Shape_Destructor(_this->super);
    free(_this->super);
}

// class Picture
void drawAll(Shape* pic[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        (pic[i]->v_ptr[DRAW_INDEX].void_method)(pic[i]);
        cout << endl;
    }
}

void printAll(Shape* pic[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        (pic[i]->v_ptr[PRINT_INDEX].void_method)(pic[i]);
        if (i % 2 == 1)
            cout << endl;
    }
}

double totalArea(Shape* pic[], int size)
{
    double area = 0;
    for (int i = 0; i < size; ++i)
    {
        area += (pic[i]->v_ptr[AREA_INDEX].double_method)(pic[i]);
    }
    return area;
}

void destroyAll(Shape* pic[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        (pic[i]->v_ptr[DESTRUCTOR_INDEX].void_method)(pic[i]);
    }
}

int main(int argc, char* argv[])
{
    int arg1 = atoi(argv[1]);
    int arg2 = atoi(argv[2]);
    //cout << arg1 << ", " << arg2 << endl;
    cout << std::fixed;
    Triangle tri1, tri2;
    Triangle_Triangle(&tri1, "FirstTriangle", arg1, arg2);
    Triangle_Triangle(&tri2, "SecondTriangle", arg1 - 1, arg2 - 1);
    Circle cir1, cir2;
    Circle_Circle(&cir1, "FirstCircle", arg1);
    Circle_Circle(&cir2, "SecondCircle", arg1 - 1);
    Square sq1, sq2;
    Square_Square(&sq1, "FirstSquare", arg1);
    Square_Square(&sq2, "SecondSquare", arg1 - 1);
    Rectangle re1, re2;
    Rectangle_Rectangle(&re1, "FirstRectangle", arg1, arg2);
    Rectangle_Rectangle(&re2, "SecondRectangle", arg1 - 1, arg2 - 1);

    Shape* pic[] = {(Shape*) &tri1, (Shape*) &tri2, (Shape*) &cir1, (Shape*) &cir2,
                    (Shape*) &sq1, (Shape*) &sq2, (Shape*) &re1, (Shape*) &re2};
    size_t pic_len = sizeof(pic) / sizeof(Shape*);
    printAll(pic, pic_len);
    drawAll(pic, pic_len);
    cout << std::fixed << std::setprecision(2);
    cout << "Total : " << totalArea(pic, pic_len) << endl;
    destroyAll(pic, pic_len);
}
