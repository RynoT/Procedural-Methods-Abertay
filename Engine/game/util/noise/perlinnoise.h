#pragma once

struct Permutation
{
	Permutation(const int *permArray);

	int value[512];
};

class PerlinNoise
{
	PerlinNoise() { }

public:
	static double noise(double x, double y);

	static double noise(double x, double y, double z);

private:
	inline static double fade(const double& t);

	inline static double lerp(const double& t, const double& a, const double& b);

	static double grad(const int& hash, const double& x, const double& y, const double& z);

	static const int m_PermArray[];
	static const Permutation m_Permutation;
};