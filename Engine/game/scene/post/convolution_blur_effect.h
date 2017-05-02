#pragma once

#include "convolution_effect.h"

// Convolution Blur provides a small blur using a convolution matrix.
// This blur is applied directly to the scene. (No downsampling)
class ConvolutionBlurEffect : public ConvolutionEffect
{
public:
	ConvolutionBlurEffect(D3DClass* d3d, const HWND& hwnd) : ConvolutionEffect(d3d, hwnd)
	{
		// Three iterations provides a good-enough blur
		ConvolutionEffect::SetIterationCount(3);
		ConvolutionEffect::SetConvolutionMatrix(1, 2, 1, 2, 4, 2, 1, 2, 1);
	}
};
