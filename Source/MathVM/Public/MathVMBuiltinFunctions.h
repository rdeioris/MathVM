// Copyright 2024, Roberto De Ioris.

#pragma once

#include "MathVM.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		bool Abs(MATHVM_ARGS); constexpr int32 AbsArgs = 1;
		bool ACos(MATHVM_ARGS); constexpr int32 ACosArgs = 1;
		bool All(MATHVM_ARGS); constexpr int32 AllArgs = -1;
		bool Any(MATHVM_ARGS); constexpr int32 AnyArgs = -1;
		bool ASin(MATHVM_ARGS); constexpr int32 ASinArgs = 1;
		bool ATan(MATHVM_ARGS); constexpr int32 ATanArgs = 1;
		bool Ceil(MATHVM_ARGS); constexpr int32 CeilArgs = 1;
		bool Clamp(MATHVM_ARGS); constexpr int32 ClampArgs = 3;
		bool Cos(MATHVM_ARGS); constexpr int32 CosArgs = 1;
		bool Degrees(MATHVM_ARGS); constexpr int32 DegreesArgs = 1;
		bool Distance(MATHVM_ARGS); constexpr int32 DistanceArgs = -1;
		bool Dot(MATHVM_ARGS); constexpr int32 DotArgs = -1;
		bool Equal(MATHVM_ARGS); constexpr int32 EqualArgs = -1;
		bool Exp(MATHVM_ARGS); constexpr int32 ExpArgs = 1;
		bool Exp2(MATHVM_ARGS); constexpr int32 Exp2Args = 1;
		bool Floor(MATHVM_ARGS); constexpr int32 FloorArgs = 1;
		bool Fract(MATHVM_ARGS); constexpr int32 FractArgs = 1;
		bool Gradient(MATHVM_ARGS); constexpr int32 GradientArgs = -1;
		bool GreaterThan(MATHVM_ARGS); constexpr int32 GreaterThanArgs = 2;
		bool GreaterEqual(MATHVM_ARGS); constexpr int32 GreaterEqualArgs = 2;
		bool Hue2B(MATHVM_ARGS); constexpr int32 Hue2BArgs = 1;
		bool Hue2G(MATHVM_ARGS); constexpr int32 Hue2GArgs = 1;
		bool Hue2R(MATHVM_ARGS); constexpr int32 Hue2RArgs = 1;
		bool Length(MATHVM_ARGS); constexpr int32 LengthArgs = -1;
		bool Lerp(MATHVM_ARGS); constexpr int32 LerpArgs = 3;
		bool LessThan(MATHVM_ARGS); constexpr int32 LessArgs = 2;
		bool LessEqual(MATHVM_ARGS); constexpr int32 LessEqualArgs = 2;
		bool Log(MATHVM_ARGS); constexpr int32 LogArgs = 1;
		bool Log2(MATHVM_ARGS); constexpr int32 Log2Args = 1;
		bool Map(MATHVM_ARGS); constexpr int32 MapArgs = 5;
		bool Max(MATHVM_ARGS); constexpr int32 MaxArgs = -1;
		bool Min(MATHVM_ARGS); constexpr int32 MinArgs = -1;
		bool Mod(MATHVM_ARGS); constexpr int32 ModArgs = 2;
		bool Not(MATHVM_ARGS); constexpr int32 NotArgs = 1;
		bool Pow(MATHVM_ARGS); constexpr int32 PowArgs = 2;
		bool Radians(MATHVM_ARGS); constexpr int32 RadiansArgs = 1;
		bool Rand(MATHVM_ARGS); constexpr int32 RandArgs = 2;
		bool Round(MATHVM_ARGS); constexpr int32 RoundArgs = 1;
		bool RoundEven(MATHVM_ARGS); constexpr int32 RoundEvenArgs = 1;
		bool Sign(MATHVM_ARGS); constexpr int32 SignArgs = 1;
		bool Sin(MATHVM_ARGS); constexpr int32 SinArgs = 1;
		bool Sqrt(MATHVM_ARGS); constexpr int32 SqrtArgs = 1;
		bool Step(MATHVM_ARGS); constexpr int32 StepArgs = 2;
		bool Tan(MATHVM_ARGS); constexpr int32 TanArgs = 1;
		bool Trunc(MATHVM_ARGS); constexpr int32 TruncArgs = 1;

		// Resources
		bool Read(MATHVM_ARGS); constexpr int32 ReadArgs = -1;
		bool Write(MATHVM_ARGS); constexpr int32 WriteArgs = -1;
	}
}