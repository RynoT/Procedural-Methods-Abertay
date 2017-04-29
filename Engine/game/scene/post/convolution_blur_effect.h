#pragma once

#include "convolution_effect.h"

class ConvolutionBlurEffect : public ConvolutionEffect
{
public:
	ConvolutionBlurEffect(D3DClass* d3d, const HWND& hwnd) : ConvolutionEffect(d3d, hwnd)
	{
		float value = 1.0f / 16.0f;
		ConvolutionEffect::SetIterationCount(3);
		ConvolutionEffect::SetConvolutionMatrix(value, value, value, value, value, value, value, value, value);
	}
};
