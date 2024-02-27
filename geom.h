#include <bits/stdc++.h>

#define i64 long long
#define d64 long double

using namespace std;

class Color {
        public:
                double r, g, b;
                Color(double reed = 0, double greeen = 0, double bluee = 0): r(reed), g(greeen), b(bluee) {}

                Color ColorRev() const {
                        return Color(1 - r, 1 - g, 1 - b);
                }

                Color operator *(double k) const {
                        return Color(r * k, g * k, b * k);
                }
};

class Point {
	public:
		double x;
		double y;
		double z;
		Point() {}
		
		Point(double x, double y, double z):
			x(x),
			y(y),
			z(z)
		{}
};

class Vector {
	public:
		double x;
		double y;
		double z;

		Vector() {}

		Vector(Point a, Point b) {
			x = b.x - a.x;
			y = b.y - a.y;
			z = b.z - a.z;
		}

		Vector(Point a) {
			x = a.x;
			y = a.y;
			z = a.z;
		}

		double Len() const {
			return sqrt(x * x + y * y + z * z);
		}

		Vector operator -(Vector a) const {
			return Vector(Point(x - a.x, y - a.y, z - a.z));
		}

		Vector operator +(Vector a) const {
			return Vector(Point(x + a.x, y + a.y, z + a.z));
		}

		Vector operator -(Point a) const {
                        return Vector(Point(x - a.x, y - a.y, z - a.z));
                }

                Vector operator +(Point a) const {
                        return Vector(Point(x + a.x, y + a.y, z + a.z));
                }

		Vector operator -(double a) const {
                        return Vector(Point(x - a, y - a, z - a));
                }

                Vector operator +(double a) const {
                        return Vector(Point(x + a, y + a, z + a));
                }

		Vector Mul(double k) const {
			return Vector(Point(x * k, y * k, z * k));		
		}

		double operator %(Vector a) const {
			return x * a.x + y * a.y + z * a.z;
		}

		Vector operator *(Vector a) {
			Vector one, two, three;
			one = Vector(Point(1, 0, 0)).Mul(y * a.z - z * a.y);
			two = Vector(Point(0, 1, 0)).Mul(x * a.z - z * a.x);
			three = Vector(Point(0, 0, 1)).Mul(x * a.y - y * a.x);
			return one - two + three;
		}

};

class Sphere {
	public:
		Point centre;
		double r;
		Color col;

		Sphere() {}

		Sphere(Point centre, double r, Color cl): centre(centre), r(r), col(cl) {}

		shared_ptr<pair<double, double>> Intersection(Vector v, Point a) const {
			Vector u = Vector(centre, a);
			double discriminant = ((v % u) * 2) * ((v % u) * 2) - 4 * (v % v) * (u % u - r * r);	
			if (discriminant < 0) {
				return nullptr;
			}
			double t1 = (-2 * (u % v) + sqrtl(discriminant)) / (2 * (v % v));
			double t2 = (-2 * (u % v) - sqrtl(discriminant)) / (2 * (v % v));
			return make_shared<pair<double, double>>(t1, t2);
		}
};


Point Shift(Point nw, Vector a) {
	return Point(nw.x + a.x, nw.y + a.y, nw.z + a.z);
}
