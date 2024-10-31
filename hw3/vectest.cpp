#include "vector.h"

int main()
{
    Vector<int> intVec{1,3,5,7,9};
    Vector<double> doubleVec{1.5,2.5,3.5,4.5};
    Vector<int> iv{intVec};
    Vector<double> dv{doubleVec};
    cout << "intVec" << intVec << endl; // "intVec(1, 3, 5, 7, 9)"
    cout << "iv" << iv << endl; // "iv(1, 3, 5, 7, 9)"
    cout << "doubleVec" << doubleVec << endl; // "doubleVec(1.5, 2.5, 3.5, 4.5)"
    cout << "dv" << dv << endl; // "dv(1.5, 2.5, 3.5, 4.5)"
    // add at least one test case for each method defined in Vector
    cout << "in_range(-1): " << iv.in_range(-1) << endl;
    cout << "size: " << iv.size() << endl;
    cout << "operator[]: " << dv[0] << endl;
    const double d = dv[1];
    cout << "operator[] const: " << d << endl;
    cout << "Dot product: " << intVec * intVec << endl;
    cout << "Addition: " << intVec + intVec << endl;
    Vector<int> iv3 = intVec;
    cout << "Copy assignment: " << iv3 << endl;
    cout << "Equality: " << (intVec == iv3) << endl;
    cout << "Inequality: " << (intVec != (1 + intVec)) << endl;
    cout << "Scalar multiplication: " << (3 * intVec) << endl;
    cout << "Scalar addition: " << (3 + intVec) << endl;
    return 0;
}
