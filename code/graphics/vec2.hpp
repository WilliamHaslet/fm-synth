#pragma once

struct Vec2
{
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float x, float y);

	Vec2 operator+(Vec2 other);
	Vec2 operator-(Vec2 other);
	Vec2 operator*(Vec2 other);
	Vec2 operator/(Vec2 other);

	Vec2 operator+(float scalar);
	Vec2 operator-(float scalar);
	Vec2 operator*(float scalar);
	Vec2 operator/(float scalar);
};
