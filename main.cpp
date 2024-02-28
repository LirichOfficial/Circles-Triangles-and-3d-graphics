#pragma once

#include <bits/stdc++.h>
#include "geom.h"

using namespace std;

#define i64 long long
#define d64 long double

const int WIDE = 2000;
const int HEIT = 2000;
const int MAXCOL = 512;
const int INF = 1e9;
const double EPS = 1e-12;
const double PI = 3.1415926535897932384626;


ostream& operator<<(ostream &out, Color a) {
	out << (int)(a.r * MAXCOL) << " " << (int)(a.g * MAXCOL) << " " << (int)(a.b * MAXCOL);
	return out;
}



void ConsoleOut(ofstream &fout, vector<Color> &output) {
	fout << "P3\n";
	fout << WIDE << " " << HEIT << "\n";
	fout << MAXCOL + 1 << "\n";
	for (int i = 0; i < HEIT * WIDE; i++) {
		fout << output[i] << "\n";
	}
}



void MakeGradient(vector<Color> &output) {
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


double binpow(double n, int p) {
	if (p == 0) {
		return 1;
	}
	if (p % 2 == 0) {
		double c = binpow (n, p / 2);
		return c * c;
	} else {
		return binpow(n, p - 1) * n;
	}
}


signed main() {
	ofstream fout;
	fout.open("image.ppm");

	vector<Color> output(WIDE * HEIT);

	MakeGradient(output);

	double mh = 1, mw = 1, md = 1;
	
	Point camera = Point(0, 0, 0);
	Point sun = Point(0, 10, 0);

	vector<Sphere> shars = {Sphere(Point(0, 0, 7), 2, Color(1, 1, 0))};

	for (int i = 0; i < HEIT; i++) {
		for (int j = 0; j < WIDE; j++) {
			Color cl = output[i * WIDE + j];
			double dst = INF;
			for (auto shar : shars) {
				Point xy = Point(-mw / 2.0 + mw * ((double)j / (WIDE - 1)), mh / 2.0 - mh * ((double)i / (HEIT - 1)), md);
				Vector v = Vector(camera, xy);
				auto tt = shar.Intersection(v, camera);
				if (!tt) continue;
				double t = min(tt -> first, tt -> second);
				Point pt = Shift(camera, v.Mul(t));
				Vector tuda = Vector(camera, pt);
				Vector suda = Vector(sun, pt);
				auto pp = shar.Intersection(suda, sun);
				if (tuda.Len() < dst && abs(1 - min(pp -> first, pp -> second)) <= EPS) {
					dst = suda.Len();
					Vector base = Vector(shar.centre, pt);
					cl = shar.col * (base.Norm() % suda.Mul(-1).Norm());
				} else if (tuda.Len() < dst) {
					dst = suda.Len();
					cl = {0, 0, 0};
				}
			}
			output[i * WIDE + j] = cl;
		}
	}	

	ConsoleOut(fout, output);	
}
