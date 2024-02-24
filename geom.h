#include <bits/stdc++.h>

using namespace std;

#define double long double
#define int long long

class Point {
	public:
		double x, y;
		Point(double x_ = 0, double y_ = 0): x(x_), y(y_) {}
		Point operator +(Point a) {
			return {a.x + x, a.y + y};
		}
		Point operator -(Point a) {
			return {x - a.x, y - a.y};
		}
		double operator %(Point a) {
			return	a.x * x + a.y * y;
		}
		double operator *(Point a) {
			return	x * a.y - y * a.x;
		}
		Point turn(double angle) {
			double a = x * cosl(angle) - y * sinl(angle);
			double b = x * sinl(angle) + y * cosl(angle);
			return {a, b};
		}
		double len2() {
			return x * x + y * y;
		}
		double len() {
			return hypotl(x, y);
		}
};

istream& operator>> (istream& in, Point &a) {
	in >> a.x >> a.y;
	return in;
}

bool inside(Point a, Point b, Point c, Point p) {
        return ((a - b) * (p - b)) * ((p - b) * (c - b)) >= 0 && ((a - b) * (c - b)) * ((a - b) * (p - b)) >= 0;
}

//многоугольник

class polygon {
	public:
		int n;
		vector<Point> pt;
		polygon(int n_ = 0, vector<Point> input = {}) {
			n = n_;
			pt = input;
		}
		vector<vector<Point>> triangulation() {
			int sz = n;
			int idx = 0;
			vector<vector<Point>> ans;
			vector<int> link(n);
			for (int i = 0; i < n; i++) {
				link[i] = (i + 1) % n;
			}
			while (sz != 3) {
				Point v1 = pt[idx], v2 = pt[link[idx]], v3 = pt[link[link[idx]]];
				int szt = sz - 3;
				int idxt = link[link[link[idx]]];
				bool found = true;
				while (szt) {
					if (inside(v1, v2, v3, pt[idxt]) && inside(v2, v3, v1, pt[idxt])) {
						found = false;
						break;
					}
					idxt = link[idxt];
					szt--;
				}
				if (found && (v2 - v1) * (v3 - v1) <= 0) {
					sz--;
					link[idx] = link[link[idx]];
					ans.push_back({v1, v2, v3});
				}
				idx = link[idx];
			}
			ans.push_back({pt[idx], pt[link[idx]], pt[link[link[idx]]]});
			return ans;
		}	
};







