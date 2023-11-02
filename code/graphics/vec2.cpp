#include "vec2.hpp"

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{

}

// Vec2 operations
Vec2 Vec2::operator+(Vec2 other)
{
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(Vec2 other)
{
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(Vec2 other)
{
	return Vec2(x * other.x, y * other.y);
}

Vec2 Vec2::operator/(Vec2 other)
{
	return Vec2(x / other.x, y / other.y);
}

// float operations
Vec2 Vec2::operator+(float scalar)
{
	return Vec2(x + scalar, y + scalar);
}

Vec2 Vec2::operator-(float scalar)
{
	return Vec2(x - scalar, y - scalar);
}

Vec2 Vec2::operator*(float scalar)
{
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar)
{
	return Vec2(x / scalar, y / scalar);
}
