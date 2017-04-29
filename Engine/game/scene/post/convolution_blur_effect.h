#pragma once

#include "convolution_effect.h"

class ConvolutionBlurEffect : public ConvolutionEffect
{
public:
	ConvolutionBlurEffect(D3DClass* d3d, const HWND& hwnd) : ConvolutionEffect(d3d, hwnd)
	{
		ConvolutionEffect::SetIterationCount(3);
		ConvolutionEffect::SetConvolutionMatrix(1, 2, 1, 2, 4, 2, 1, 2, 1);
	}
};
