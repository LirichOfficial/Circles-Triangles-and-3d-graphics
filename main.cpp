#pragma once

#include <bits/stdc++.h>
#include "geom.h"

using namespace std;

#define i64 long long
#define d64 long double

const int WIDE = 1920;
const int HEIT = 1280;
const int MAXCOL = 255;
const int INF = 1e9;
const double EPS = 1e-9;
const double PI = 3.1415926535897932384626;
const double mh = 0.7692, mw = 1.1538, md = 1;
const double DontRemember = 1.2;

map<string, Material> type = {
	{"red bubble(basic)", Material(Color(1, 0, 0), 0.6, 1.2, 35, 1)},
	{"yellow bubble(modified)", Material(Color(1, 1, 0), 1.1, 0.5, 45, 0)}
};

class Light {
	public: 
		Point centre;
		double intensity;

		Light() {}

		Light(Point centre, double intensity): centre(centre), intensity(intensity) {}
};



ostream& operator<<(ostream &out, Color a) {
	out << (int)(a.r * MAXCOL) << " " << (int)(a.g * MAXCOL) << " " << (int)(a.b * MAXCOL);
	return out;
}



void ConsoleOut(ofstream &fout, vector<Color> &output) {
	fout << "P3\n";
	fout << WIDE << " " << HEIT << "\n";
	fout << MAXCOL + 1 << "\n";
	for (int i = 0; i < HEIT * WIDE; i++) {
		fout << output[i].Pow(DontRemember) << "\n";
	}
}



void MakeGradient(vector<Color> &output) {
	for (int i = 0; i < HEIT; i++) {
                for (int j = 0; j < WIDE; j++) {
                        output[j + WIDE * i].r = 1.0 - (double)(i + j) / (double)(WIDE + HEIT - 2);
                }
        }

        for (int i = 0; i < HEIT; i++) {
                for (int j = WIDE - 1; j >= 0; j--) {
                        int dist = i + (WIDE - j - 1);
                        output[j + WIDE * i].g = 1.0 - (double)dist / (double)(WIDE + HEIT - 2);
                }
        }

        for (int i = HEIT - 1; i >= 0; i--) {
                for (int j = 0; j < WIDE; j++) {
                        int dist = (HEIT - i - 1) + j;
                        output[j + WIDE * i].b = 1.0 - (double)dist / (double)(WIDE + HEIT - 2);
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

void Pers(const Point &camera, const vector<Color> &output, const vector<Light> &suns, const vector<Sphere> &shars, const int &i, const int &j, Color &cl, double &intensity, double &specture, double &white, double &transparency, Point &p, Vector &norm, Material &Mat, double &dst, Color cur) {


                        for (auto shar : shars) {
                                Point xy = Point(-mw / 2.0 + mw * ((double)j / (WIDE - 1)), mh / 2.0 - mh * ((double)i / (HEIT - 1)), md);
                                Vector v = Vector(camera, xy);
                                auto tt = shar.Intersection(v, camera);
                                if (!tt) continue;

                                double t = min(tt -> first, tt -> second);
                                Point pt = Shift(camera, v.Mul(t));
                                Vector tuda = Vector(camera, pt);
                                if (tuda.Len() >= dst) continue;

                                cl = shar.mat.col * shar.mat.transparency + cur.ColorRev() * (1 - shar.mat.transparency);
                                dst = tuda.Len();
                                norm = Vector(shar.centre, pt);
                                Mat = shar.mat;
                                p = pt;
                                transparency = shar.mat.transparency;

                                for (auto sun : suns) {
                                        bool ok = true;
                                        Vector suda = Vector(sun.centre, pt);
                                        for (auto st : shars) {
                                                auto pers = st.Intersection(suda, sun.centre);
                                                if (!pers) continue;
						if ((pers -> first >= 0 && pers -> first + EPS < 1.0) || (pers -> second >= 0 && pers -> second + EPS < 1.0)) {
                                                        ok = false;
                                                        break;
                                                }
                                        }

                                        if (!ok) continue;

                                        auto pp = shar.Intersection(suda, sun.centre);
                                        if (abs(1 - min(pp -> first, pp -> second)) <= EPS) {
                                                Vector base = Vector(shar.centre, pt);
                                                intensity += sun.intensity * (base.Norm() % suda.Mul(-1).Norm()) * shar.mat.diffusionAlbedo;
                                        }
                                }
                        }
			intensity = min(intensity, 1.0);
}


Color Zerkalo(const vector<Sphere> &shars, const vector<Light> &suns, Vector ray, Point p) {
	double dst = INF;
	
	double intensity = 0;
	Color cl = Color(0, 0, 0);
	
	for (auto shar : shars) {
		auto pers = shar.Intersection(ray, p);
		if (!pers) continue;
		
		if (pers -> second <= EPS && pers -> first <= EPS) continue;

		if (pers -> second <= EPS) { 
			ray = ray.Mul(pers -> first);
		} else if (pers -> first <= EPS) {
			ray = ray.Mul(pers -> second);
		} else {
			ray = ray.Mul(min(pers -> first, pers -> second));
		}	

		if (ray.Len() >= dst) continue;

		cl = shar.mat.col;
		dst = ray.Len();

		for (auto sun : suns) {
			Vector suda = Vector(sun.centre, Shift(p, ray));
			
			bool ok = true;	
			for (auto shr : shars) {
				auto RealShit = shr.Intersection(suda, sun.centre);
				if (!RealShit) continue;

				if ((RealShit -> first >= EPS && 1 - RealShit -> first > EPS) || (RealShit -> second >= EPS && 1 - RealShit -> second > EPS)) {
					ok = false;
					break;
				}
			}
			if (ok) {
				intensity += max(Vector(shar.centre, Shift(p, ray)).Norm() % Vector(Shift(p, ray), sun.centre).Norm(), 0.0) * sun.intensity * shar.mat.diffusionAlbedo; 
			}
		}
	}
	return cl * intensity;
}	


signed main() {
	ofstream fout;
	fout.open("image.ppm");

	vector<Color> output(WIDE * HEIT);

	MakeGradient(output);
	
	Point camera = Point(0, 0, 0);
	vector<Light> suns = {Light(Point(-6, 0, -4), 1), Light(Point(10, 0, 4), 0.8)};

	vector<Sphere> shars = {Sphere(Point(-1, 0, 15), 2, type["red bubble(basic)"]), Sphere(Point(4, 0, 11), 2, type["yellow bubble(modified)"])};

	for (int i = 0; i < HEIT; i++) {
		for (int j = 0; j < WIDE; j++) {
			Color cl = output[i * WIDE + j];
			Color base = output[i * WIDE + j];

			double dst = INF;
			double intensity = 0;
			double specture = 0;
			double white = 0;
			double transparency = 0;

			Vector norm;
			Material Mat;
			Point p;

			Pers(camera, output, suns, shars, i, j, cl, intensity, specture, white, transparency, p, norm, Mat, dst, base);
		        
			Color Addition = Color(0, 0, 0);	

			if (abs(INF - dst) > EPS) {
				norm = norm.Norm();	
				for (auto sun : suns) {
                                        bool ok = true;
                                        Vector suda = Vector(sun.centre, p);
                                        for (auto st : shars) {
                                                auto pers = st.Intersection(suda, sun.centre);
                                                if (!pers) continue;
                                                if ((pers -> first >= 0 && pers -> first + EPS < 1.0) || (pers -> second >= 0 && pers -> second + EPS < 1.0)) {
                                                        ok = false;
                                                        break;
                                                }
                                        }

                                        if (!ok) continue;
					
                                       	Vector N = Vector(sun.centre, p).Norm();
					Vector B = norm.Mul(N.Mul(-1) % norm).Mul(-1);
					Vector M = ((N - B) - B).Norm();
					double cs = max(M % Vector(p, camera).Norm(), 0.0);
					white += Mat.specularAlbedo * binpow(cs, Mat.specular) * sun.intensity;

					Addition = Addition + Zerkalo(shars, suns, M, p);
					
                                }


				output[i * WIDE + j] = ((cl * intensity + (Color(1, 1, 1) * white)) + Addition * (1 - transparency));
			}
		}
	}	

	ConsoleOut(fout, output);	
}
