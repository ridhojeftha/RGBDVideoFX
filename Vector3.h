/**
 * RGBDVideoFX - Helper class, Vector3
 * ridhojeftha
 */
#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Vector3{
public:
	float x, y, z;

	Vector3(){x=0, y=0, z=0;}
	Vector3(float x1, float y1, float z1){
		x=x1;
		y=y1;
		z=z1;
	}

	Vector3(const Vector3& vec){
		x=vec.x;
		y=vec.y;
		z=vec.z;
	};

	void operator+=(Vector3& vec){
		x+=vec.x;
		y+=vec.y;
		z+=vec.z;
	}
	void operator-=(Vector3& vec){
		x-=vec.x;
		y-=vec.y;
		z-=vec.z;
	}
	void operator*=(float num){
		x*=num;
		y*=num;
		z*=num;
	}
	void operator/=(float num){
		x/=num;
		y/=num;
		z/=num;
	}

	Vector3 operator+(Vector3& vec){
		return Vector3(x+vec.x, y+vec.y, z+vec.z);
	}
	Vector3 operator-(Vector3& vec){
		return Vector3(x-vec.x, y-vec.y, z-vec.z);
	}
	Vector3 operator*(float num){
		return Vector3(x*num, y*num, z*num);
	}
	Vector3 operator/(float num){
		return Vector3(x/num, y/num, z/num);
	}

	Vector3 minVec(Vector3& vec){
		using namespace std;
		return Vector3(min<float>(x,vec.x),min<float>(y,vec.y),min<float>(z,vec.z));
	}
	Vector3 maxVec(Vector3& vec){
		using namespace std;
		return Vector3(max<float>(x,vec.x),max<float>(y,vec.y),max<float>(z,vec.z));
	}

	float length(){
		return sqrtf(x * x + y * y + z * z);
	}
	float length2(){
		return x * x + y * y + z * z;
	}
	Vector3 normalize(){
		float magnitude = length();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
		return Vector3(x,y,z);
	}

	Vector3 cross(const Vector3& vec){

		return Vector3(	(y*vec.z)-(vec.y*z),
						-(x*vec.z)+(vec.x*z),
						(x*vec.y)-(y*vec.x)
					);

	}

	friend std::ostream& operator<<(std::ostream &strm, const Vector3 &a) {
		return strm << "Vector3(" << a.x<<", "<<a.y<<", "<<a.z << ")";
	}
};
#endif
