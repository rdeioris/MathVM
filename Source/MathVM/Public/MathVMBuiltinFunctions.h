// Copyright 2024, Roberto De Ioris.

#pragma once

#include "MathVM.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		MATHVM_API bool Abs(MATHVM_ARGS); constexpr int32 AbsArgs = 1;
		MATHVM_API bool ACos(MATHVM_ARGS); constexpr int32 ACosArgs = 1;
		MATHVM_API bool All(MATHVM_ARGS); constexpr int32 AllArgs = -1;
		MATHVM_API bool Any(MATHVM_ARGS); constexpr int32 AnyArgs = -1;
		MATHVM_API bool ASin(MATHVM_ARGS); constexpr int32 ASinArgs = 1;
		MATHVM_API bool ATan(MATHVM_ARGS); constexpr int32 ATanArgs = 1;
		MATHVM_API bool Ceil(MATHVM_ARGS); constexpr int32 CeilArgs = 1;
		MATHVM_API bool Clamp(MATHVM_ARGS); constexpr int32 ClampArgs = 3;
		MATHVM_API bool Cos(MATHVM_ARGS); constexpr int32 CosArgs = 1;
		MATHVM_API bool Degrees(MATHVM_ARGS); constexpr int32 DegreesArgs = 1;
		MATHVM_API bool Distance(MATHVM_ARGS); constexpr int32 DistanceArgs = -1;
		MATHVM_API bool Dot(MATHVM_ARGS); constexpr int32 DotArgs = -1;
		MATHVM_API bool Equal(MATHVM_ARGS); constexpr int32 EqualArgs = -1;
		MATHVM_API bool Exp(MATHVM_ARGS); constexpr int32 ExpArgs = 1;
		MATHVM_API bool Exp2(MATHVM_ARGS); constexpr int32 Exp2Args = 1;
		MATHVM_API bool Floor(MATHVM_ARGS); constexpr int32 FloorArgs = 1;
		MATHVM_API bool Fract(MATHVM_ARGS); constexpr int32 FractArgs = 1;
		MATHVM_API bool Gradient(MATHVM_ARGS); constexpr int32 GradientArgs = -1;
		MATHVM_API bool GreaterThan(MATHVM_ARGS); constexpr int32 GreaterThanArgs = 2;
		MATHVM_API bool GreaterEqual(MATHVM_ARGS); constexpr int32 GreaterEqualArgs = 2;
		MATHVM_API bool Hue2B(MATHVM_ARGS); constexpr int32 Hue2BArgs = 1;
		MATHVM_API bool Hue2G(MATHVM_ARGS); constexpr int32 Hue2GArgs = 1;
		MATHVM_API bool Hue2R(MATHVM_ARGS); constexpr int32 Hue2RArgs = 1;
		MATHVM_API bool Length(MATHVM_ARGS); constexpr int32 LengthArgs = -1;
		MATHVM_API bool Lerp(MATHVM_ARGS); constexpr int32 LerpArgs = 3;
		MATHVM_API bool LessThan(MATHVM_ARGS); constexpr int32 LessArgs = 2;
		MATHVM_API bool LessEqual(MATHVM_ARGS); constexpr int32 LessEqualArgs = 2;
		MATHVM_API bool Log(MATHVM_ARGS); constexpr int32 LogArgs = 1;
		MATHVM_API bool Log2(MATHVM_ARGS); constexpr int32 Log2Args = 1;
		MATHVM_API bool Map(MATHVM_ARGS); constexpr int32 MapArgs = 5;
		MATHVM_API bool Max(MATHVM_ARGS); constexpr int32 MaxArgs = -1;
		MATHVM_API bool Min(MATHVM_ARGS); constexpr int32 MinArgs = -1;
		MATHVM_API bool Mod(MATHVM_ARGS); constexpr int32 ModArgs = 2;
		MATHVM_API bool Not(MATHVM_ARGS); constexpr int32 NotArgs = 1;
		MATHVM_API bool Pow(MATHVM_ARGS); constexpr int32 PowArgs = 2;
		MATHVM_API bool Radians(MATHVM_ARGS); constexpr int32 RadiansArgs = 1;
		MATHVM_API bool Rand(MATHVM_ARGS); constexpr int32 RandArgs = 2;
		MATHVM_API bool Round(MATHVM_ARGS); constexpr int32 RoundArgs = 1;
		MATHVM_API bool RoundEven(MATHVM_ARGS); constexpr int32 RoundEvenArgs = 1;
		MATHVM_API bool Sign(MATHVM_ARGS); constexpr int32 SignArgs = 1;
		MATHVM_API bool Sin(MATHVM_ARGS); constexpr int32 SinArgs = 1;
		MATHVM_API bool Sqrt(MATHVM_ARGS); constexpr int32 SqrtArgs = 1;
		MATHVM_API bool Tan(MATHVM_ARGS); constexpr int32 TanArgs = 1;
		MATHVM_API bool Trunc(MATHVM_ARGS); constexpr int32 TruncArgs = 1;

		// Resources
		MATHVM_API bool Read(MATHVM_ARGS); constexpr int32 ReadArgs = -1;
		MATHVM_API bool Write(MATHVM_ARGS); constexpr int32 WriteArgs = -1;
	}
}