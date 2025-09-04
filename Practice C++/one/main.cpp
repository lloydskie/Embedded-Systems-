#include <iostream>

using namespace std;

int main()
{
    string v = "";
    string w = "";
    string x = "Ben";
    string y = "Ana";
    string z = "double dead";
    char a = 'A';
    bool b = true;
    int c;
    float d = 69.6;
    double e = 69.6;
    int f, g, h;

    cout << x << " is dead." << endl;
    cout << y << " is deadly." << endl;
    cout << x << " and " << y << " are " << z << "."<<endl;
    cout << "Enter your victim name: ";
    cin >> w;
    cout << "Your next target is " << w << "." << endl;
    {
        cout << "Enter your victim name: ";
        cin >> w;
    }
    cout << "Age: ";
    cin >> c;
    cout << "Your victim was " << w << " and " << c << " years old."<<endl;
    cout << "Enter birth year(YYYY): ";
    cin >> f;
    cout << "Enter birth month(1-12): ";
    cin >> g;
    if (g < 1 || g > 12) {
        cout << "Invalid month. Please enter a value between 1 and 12." << endl;
    }else {
        cin >> g;
    }
    cout << "Enter birth day(1-31): ";
    cin >> h;
    if (h < 1 || h > 31) {
        cout << "Invalid day. Please enter a value between 1 and 31." << endl;
    }else {
        cin >> h;
    }
    cout << "Your victim " << w << " was born on " << f << "-" << g << "-" << h << "."<<endl;

    return 0;
}
