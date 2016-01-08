
#include <iostream>
using std::ostream;

class Vec3f {
	public:
		float v[3];
	public:
		Vec3f();
		Vec3f(float x, float y, float z);

		float returnv0();
		float returnv1();
		float returnv2();

		float &operator[](int index);
		float operator[](int index) const;

		Vec3f operator*(float scale) const;
		Vec3f operator/(float scale) const;
		Vec3f operator+(const Vec3f &other) const;
		Vec3f operator-(const Vec3f &other) const;
		Vec3f operator-() const;

		const Vec3f &operator*=(float scale);
		const Vec3f &operator/=(float scale);
		const Vec3f &operator+=(const Vec3f &other);
		const Vec3f &operator-=(const Vec3f &other);

		float magnitude() const;
		float magnitudeSquared() const;
		Vec3f normalize() const;
		float dot(const Vec3f &other) const;
		Vec3f cross(const Vec3f &other) const;
};

Vec3f operator*(float scale, const Vec3f &v);
ostream &operator<<(ostream &output, const Vec3f &v);
