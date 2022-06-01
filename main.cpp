#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct date {
    int d, m, y;
};

// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Sakamoto's_methods
// вс - 0, пн - 1, вт - 2, ср - 3, чт - 4, пт - 5, сб - 6 (mod 7)
const int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
int date_to_int(date d) {
    d.y -= d.m < 3;
    return ( d.y + d.y / 4 - d.y / 100 + d.y / 400 + t[d.m - 1] + d.d) % 7;
}

int magic(date d) {
    if (d.m < 3) {
        d.y--; 
        d.m += 12;
    }
    return 365 * d.y + d.y/4 - d.y/100 + d.y/400 + (153 * d.m - 457)/5 + d.d - 306;
}

int main() {
    ifstream fin("plan.txt");
    if (fin.is_open()) {
        int d, m, y;
        fin >> d >> m >> y;
        date begin = { d, m, y };

        fin >> d >> m >> y;
        date end = { d, m, y };

        vector<int> nonworkingdays;
        while (!fin.eof()) {
            fin >> d;
            nonworkingdays.push_back(d);
        }

        auto start = high_resolution_clock::now();
        //auto start = steady_clock::now();
        // main start here!!!
        // 1. общее кол-во дней
        int ndays = magic(end) - magic(begin) + 1;

        // 2. кол-во выходных
        int b = date_to_int(begin);
        int be = b + ndays - 1;
        int n = 0;
        for (int d: nonworkingdays) {
            int tb = b;
            while (tb <= be && (tb - d) % 7 != 0)
                tb++;   // первый день
            if (tb <= be)
                n += 1 + (be - tb) / 7;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        //auto stop = steady_clock::now();
        //duration<double> _duration = duration_cast<duration<double>>(stop - start);
        
	
        // 3. рез
        cout << ndays - n << " (" << duration.count() << " microseconds)" << endl;
        //cout << ndays - n << " (" << _duration.count() * 1000000. << " microseconds)" << endl;
    } else {
        cout << "Error: file plan not found." << endl;
    }
    fin.close();

    return 0;
}