#include <stdio.h>
#include <math.h>

int main()
{
    double d = 0.3;
    printf("%.64lf\n", d);
    printf("%.64lf\n", 1 / d);
    printf("%.64lf\n", pow(d, -1));
    return 0;
}
