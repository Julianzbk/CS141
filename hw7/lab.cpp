#include <iostream>
using namespace std;

int i = 3;
int a[5] = {-99, 3, 1, 2, -99};

void put()
{
    cout << "[" << a[1] << ", " << a[2] << ", " << a[3] << "]" << endl;
}

void do_smth(int& t)
{
    i = i - 1;
    t = i + 1;
    a[t] = a[i] + 1;
    a[i] = a[t] + 1;
    i = i - 1;
    a[t] = t - 1;
    a[i] = a[t] - 1;
}
/*
void do_smth(int t)
{
    put();
    i = i - 1;
    a[i] = i + 1;
    put();
    a[a[i]] = a[i] + 1;
    put();
    a[i] = a[a[i]] + 1;
    put();
    i = i - 1;
    a[a[i]] = a[i] - 1;
    put();
    a[i] = a[a[i]] - 1;
    put();
}
*/

int main()
{
    do_smth(a[i]);
    cout << a[0] << a[4] << endl;
    put();
}
