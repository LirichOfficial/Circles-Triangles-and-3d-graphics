#include <bits/stdc++.h>
#include "geom.h"

using namespace std;

#define int long long
#define double long double

const int WIDE = 1920;
const int HEIT = 1280;
const int MAXCOL = 255;

class pixel {
	public:
		double r, g, b;
		pixel(double reed = 0, double greeen = 0, double bluee = 0): r(reed), g(greeen), b(bluee) {}
};

ostream& operator<<(ostream &out, pixel a) {
	out << (int)(a.r * MAXCOL) << " " << (int)(a.g * MAXCOL) << " " << (int)(a.b * MAXCOL);
	return out;
}



void console_out(ofstream &fout, vector<pixel> &output) {
	fout << "P3\n";
	fout << WIDE << " " << HEIT << "\n";
	fout << MAXCOL + 1 << "\n";
	for (int i = 0; i < HEIT * WIDE; i++) {
		fout << output[i] << "\n";
	}
}



void make_gradient(vector<pixel> &output) {
	for (int i = 0; i < HEIT; i++) {
                for (int j = 0; j < WIDE; j++) {
                        output[j + WIDE * i].r = 1 - (double)(i + j) / (double)(WIDE + HEIT - 2);
                }
        }

        for (int i = 0; i < HEIT; i++) {
                for (int j = WIDE - 1; j >= 0; j--) {
                        int dist = i + (WIDE - j - 1);
                        output[j + WIDE * i].g = 1 - (double)dist / (double)(WIDE + HEIT - 2);
                }
        }

        for (int i = HEIT - 1; i >= 0; i--) {
                for (int j = 0; j < WIDE; j++) {
                        int dist = (HEIT - i - 1) + j;
                        output[j + WIDE * i].b = 1 - (double)dist / (double)(WIDE + HEIT - 2);
                }
        }
}

void drawTriangle(vector<pixel> &output, Point a, Point b, Point c) {
	for (int i = 0; i < HEIT; i++) {
                for (int j = 0; j < WIDE; j++) {
                        if (inside(a, b, c, {j, i}) && inside(a, c, b, {j, i})) {
                                        output[i * WIDE + j].r = 1 - output[i * WIDE + j].r;
                                        output[i * WIDE + j].g = 1 - output[i * WIDE + j].g;
                                        output[i * WIDE + j].b = 1 - output[i * WIDE + j].b;
                        }
                }
        }
}




signed main() {
	ofstream fout;
	fout.open("image.ppm");

	vector<pixel> output(WIDE * HEIT);

	make_gradient(output);
	
	cout << "Input polygon:\n";
	int n;	
	cin >> n;

	vector<Point> nw(n);
	
	for (int i = 0; i < n; i++) {
		cin >> nw[i];
	}

	polygon pol = polygon(n, nw);

	vector<vector<Point>> out = pol.triangulation();

	for (auto i : out) {
		drawTriangle(output, i[0], i[1], i[2]);
	}	

	console_out(fout, output);	
}
