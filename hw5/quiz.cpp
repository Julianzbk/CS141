#include <iostream>
#include <cstdint>
using namespace std;

union u_mixed {
  char a;
  double b;
  int c;
  int d; 
};

struct s_complex {
  double a, b;
  u_mixed localunion1, localunion2;
  int c, d;
  char e, f, g, h;
};

struct Bar {
	double f;
	int i;
	char c,d,e,g;
};

union Foo {
	double f;
	char c;
	Bar b;
};



int main()
{
    u_mixed first_ds[5];
    cout << "sizeof(first_ds)=" << sizeof(first_ds) << endl;
    s_complex second_ds[10];
    cout << "sizeof(s_complex)=" << sizeof(s_complex) << endl;
    cout << "sizeof(second_ds)=" << sizeof(second_ds) << endl;
    s_complex ds[3];
    uint64_t addr = reinterpret_cast<uint64_t>(&(ds[0].g)); 
    cout << "addr(ds[0].g)=" << 1000 + addr - (uint64_t) &ds << endl;
    addr = reinterpret_cast<uint64_t>(&(ds[1].localunion1.b));
    cout << "addr(ds[1].localunion1.b)=" << 1000 + addr - (uint64_t) &ds << endl;
    addr = reinterpret_cast<uint64_t>(&(ds[1].c));
    cout << "addr(ds[1].c)=" << 1000 + addr - (uint64_t) &ds << endl;

    Foo a[10];
    Bar b;
    Foo f;
    cout << "sizeof(b)=" << sizeof(b) << endl;
    cout << "sizeof(f)=" << sizeof(f) << endl;
    cout << "sizeof(a)=" << sizeof(a) << endl;
    uint64_t base_addr = reinterpret_cast<uint64_t>(&a);
    addr = reinterpret_cast<uint64_t>(&(a[5].b.g));
    cout << "addr(a[5].b.g)=" << 1000 + addr - base_addr << endl;
    addr = reinterpret_cast<uint64_t>(&(a[4].b.i));
    cout << "addr(a[4].b.i)=" << 1000 + addr - base_addr << endl;
    addr = reinterpret_cast<uint64_t>(&(a[6].f));
    cout << "addr(a[4].b.i)=" << 1000 + addr - base_addr << endl;
}
