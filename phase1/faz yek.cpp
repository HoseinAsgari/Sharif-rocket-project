#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    double x, y, f, v;
    cout << "x_target = ";
    cin >> x;
    cout << "y_target = ";
    cin >> y;
    v = sqrt(9.8 * y + 9.8 * sqrt(pow(x, 2) + pow(y, 2)));
    double r = 9.8 * x;
    double tan_a = pow(v, 2) / r;
    double a = atan(tan_a) * 180 / 3.14;
    f = 0.3 * v;
    cout.precision(0);
    cout << "Min Required Initial Velocity : " << fixed << v << " m/s" << "\n";
    cout << "Desired Angle : " << fixed << a << " degrees" << "\n";
    cout << "Min Required fuel : " << f << " kilogram";
}
