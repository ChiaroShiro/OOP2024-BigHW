#include <bits/stdc++.h>
using namespace std;

template <class T>
bool check()
{
    return typeid(T) == typeid(double);
}
int main () {
    std::cout << to_string(1293).length() << ", " << to_string(12.111).length();
}