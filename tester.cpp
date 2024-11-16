#include <bits/stdc++.h>
using namespace std;

template <class T>
bool check()
{
    return typeid(T) == typeid(double);
}
int main () {
    cout << check <int> () << check <string> () << check <double> ();
}