// Copyright 2024, Roberto De Ioris.

#pragma once

#include "MathVM.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		bool Sin(MATHVM_ARGS); constexpr int32 SinArgs = 1;
		bool Cos(MATHVM_ARGS); constexpr int32 CosArgs = 1;
		bool Min(MATHVM_ARGS); constexpr int32 MinArgs = -1;
		bool Max(MATHVM_ARGS); constexpr int32 MaxArgs = -1;
		bool Map(MATHVM_ARGS); constexpr int32 MapArgs = 5;
		bool Rand(MATHVM_ARGS); constexpr int32 RandArgs = 2;
		bool Pow(MATHVM_ARGS); constexpr int32 PowArgs = 2;
		bool Hue2R(MATHVM_ARGS); constexpr int32 Hue2RArgs = 1;
		bool Hue2G(MATHVM_ARGS); constexpr int32 Hue2GArgs = 1;
		bool Hue2B(MATHVM_ARGS); constexpr int32 Hue2BArgs = 1;
		bool Lerp(MATHVM_ARGS); constexpr int32 LerpArgs = 3;
		bool Gradient(MATHVM_ARGS); constexpr int32 GradientArgs = -1;
	}
}