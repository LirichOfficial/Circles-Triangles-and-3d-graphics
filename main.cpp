#pragma once

#include <bits/stdc++.h>
#include "geom.h"

using namespace std;

#define i64 long long
#define d64 long double

const int WIDE = 2000;
const int HEIT = 2000;
const int MAXCOL = 255;
const int INF = 1e9;


ostream& operator<<(ostream &out, Color a) {
	out << round(a.r * MAXCOL) << " " << round(a.g * MAXCOL) << " " << round(a.b * MAXCOL);
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




signed main() {
	ofstream fout;
	fout.open("image.ppm");

	vector<Color> output(WIDE * HEIT);

	MakeGradient(output);

	double mh = 1, mw = 1, md = 1;
	
	Point camera = Point(0, 0, 0);
	Point sun = Point(40, 0, 0);

	vector<Sphere> shars = {Sphere(Point(-3, 0, 20), 4, Color(1, 0, 0)), Sphere(Point(4, 0, 15), 4, Color(1, 1, 0))};

	for (int i = 0; i < HEIT; i++) {
		for (int j = 0; j < WIDE; j++) {
			Color cl = output[i * WIDE + j];
			double dst = INF;
			for (auto shar : shars) {
				Point xy = Point(-mw / 2.0 + mw * ((double)j / (WIDE - 1)), mh / 2.0 - mh * ((double)i / (HEIT - 1)), md);
				Vector v = Vector(camera, xy);
				auto pt = shar.Intersection(v, camera);
				if (!pt) {
					continue;
				} else {
                                	if (pt -> first < pt -> second) {
                                        	Vector sud = (v.Mul(pt -> first) + camera).Mul(-1);
                                        	Point tp = Shift(camera, v.Mul(pt -> first));
                                        	Vector tud = Vector(tp, sun);
						if (sud.Len() < dst) {
                                        		cl = shar.col * max(sud % tud / (sud.Len() * tud.Len()), 0.0) * 2;
						}
                                	} else {
                                        	Vector sud = (v.Mul(pt -> second) + camera).Mul(-1);
                                        	Point tp = Shift(camera, v.Mul(pt -> second));
                                        	Vector tud = Vector(tp, sun);
                                        	if (sud.Len() < dst) {
                                                        cl = shar.col * max(sud % tud / (sud.Len() * tud.Len()), 0.0) * 2;
                                                }
                                	}
				}
			}
			output[i * WIDE + j] = cl;
		}
	}	

	ConsoleOut(fout, output);	
}
