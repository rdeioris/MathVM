// Copyright 2024, Roberto De Ioris.

#include "MathVMBuiltinFunctions.h"

FMathVMBase::FMathVMBase()
{
	OperatorAdd = [](FMathVMCallContext& CallContext) -> bool
		{
			double A = 0, B = 0;
			if (!CallContext.PopArgument(B))
			{
				return false;
			}
			if (!CallContext.PopArgument(A))
			{
				return false;
			}

			return CallContext.PushResult(A + B);
		};

	OperatorSub = [](FMathVMCallContext& CallContext) -> bool
		{
			double A = 0, B = 0;
			if (!CallContext.PopArgument(B))
			{
				return false;
			}
			if (!CallContext.PopArgument(A))
			{
				return false;
			}

			return CallContext.PushResult(A - B);
		};

	OperatorMul = [](FMathVMCallContext& CallContext) -> bool
		{
			double A = 0, B = 0;
			if (!CallContext.PopArgument(B))
			{
				return false;
			}
			if (!CallContext.PopArgument(A))
			{
				return false;
			}

			return CallContext.PushResult(A * B);
		};

	OperatorDiv = [](FMathVMCallContext& CallContext) -> bool
		{
			double A = 0, B = 0;
			if (!CallContext.PopArgument(B))
			{
				return false;
			}

			if (B == 0.0)
			{
				return CallContext.SetError("Division by zero");
			}

			if (!CallContext.PopArgument(A))
			{
				return false;
			}

			return CallContext.PushResult(A / B);
		};

	OperatorMod = [](FMathVMCallContext& CallContext) -> bool
		{
			double A = 0, B = 0;
			if (!CallContext.PopArgument(B))
			{
				return false;
			}
			if (!CallContext.PopArgument(A))
			{
				return false;
			}

			return CallContext.PushResult(static_cast<int64>(A) % static_cast<int64>(B));
		};

	OperatorAssign = [](FMathVMCallContext& CallContext) -> bool
		{
			FString A;
			double B = 0;

			if (!CallContext.PopArgument(B))
			{
				return false;
			}
			if (!CallContext.PopName(A))
			{
				return false;
			}

			if (CallContext.LocalVariables.Contains(A))
			{
				CallContext.LocalVariables[A] = B;
			}
			else if (CallContext.MathVM.HasGlobalVariable(A))
			{
				CallContext.MathVM.SetGlobalVariable(A, B);
			}
			else
			{
				CallContext.LocalVariables.Add(A, B);
			}

			return true;
		};

	Reset();
}

void FMathVMBase::Reset()
{
	CurrentLine = 0;
	CurrentColumn = 0;
	CurrentOffset = 0;
	Tokens.Empty();
}

bool FMathVMBase::SetError(const FString& InError)
{
	LastError = InError;
#if WITH_EDITOR
	UE_LOG(LogTemp, Error, TEXT("%s"), *LastError);
#endif
	return false;
}

bool FMathVMBase::RegisterFunction(const FString& Name, FMathVMFunction Callable, const int32 NumArgs)
{
	if (!MathVM::Utils::SanitizeName(Name))
	{
		return false;
	}

	if (Functions.Contains(Name))
	{
		Functions[Name] = { Callable, NumArgs };
	}
	else
	{
		Functions.Add(Name, { Callable, NumArgs });
	}

	return true;
}

bool FMathVMBase::HasGlobalVariable(const FString& Name) const
{
	return GlobalVariables.Contains(Name);
}

bool FMathVMBase::HasConst(const FString& Name) const
{
	return Constants.Contains(Name);
}

void FMathVMBase::SetGlobalVariable(const FString& Name, const double Value)
{
	FRWScopeLock Lock(GlobalVariablesLock, FRWScopeLockType::SLT_Write);
	GlobalVariables[Name] = Value;
}

double FMathVMBase::GetGlobalVariable(const FString& Name)
{
	FRWScopeLock Lock(GlobalVariablesLock, FRWScopeLockType::SLT_ReadOnly);
	return GlobalVariables[Name];
}

double FMathVMBase::GetConst(const FString& Name)
{
	return Constants[Name];
}

bool MathVM::Utils::SanitizeName(const FString& Name)
{
	if (Name.IsEmpty())
	{
		return false;
	}

	TCHAR FirstChar = Name[0];
	const bool bValidStart = (FirstChar >= 'A' && FirstChar <= 'Z') || (FirstChar >= 'a' && FirstChar <= 'z') || FirstChar == '_';
	if (!bValidStart)
	{
		return false;
	}

	for (int32 CharIndex = 1; CharIndex < Name.Len(); CharIndex++)
	{
		const TCHAR Char = Name[CharIndex];
		const bool bValidChar = (Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z') || (Char >= '0' && Char <= '9') || Char == '_';
		if (!bValidChar)
		{
			return false;
		}
	}

	return true;
}

void MathVM::Utils::DrawLine(const FVector2D Point0, const FVector2D Point1, const FColor Color, TFunction<void(const int32 X, const int32 Y, const FColor Color)> DrawPixel)
{
	// https://en.wikipedia.org/wiki/Xiaolin_Wu's_line_algorithm

	auto ColorWithBrightNess = [Color](const double InBrightness) -> FColor
		{
			FColor OutColor = Color;
			OutColor.A = InBrightness * 255.0;
			return OutColor;
		};

	auto FPart = [](const double Value) -> double
		{
			return Value - static_cast<int32>(Value);
		};

	double X0 = static_cast<int32>(Point0.X + 0.5);
	double Y0 = static_cast<int32>(Point0.Y + 0.5);

	double X1 = static_cast<int32>(Point1.X + 0.5);
	double Y1 = static_cast<int32>(Point1.Y + 0.5);

	bool bSteep = FMath::Abs(Y1 - Y0) > FMath::Abs(X1 - X0);

	if (bSteep)
	{
		Swap(X0, Y0);
		Swap(X1, Y1);
	}

	if (X0 > X1)
	{
		Swap(X0, X1);
		Swap(Y0, Y1);
	}

	double DX = X1 - X0;
	double DY = Y1 - Y0;

	double Gradient = 1.0;

	if (DX > 0.0)
	{
		Gradient = DY / DX;
	}

	const double W = 4 * FMath::Sqrt(1 + (Gradient * Gradient));

	// handle first endpoint
	double XEnd = static_cast<int32>(X0);
	double YEnd = Y0 - (W - 1) * 0.5 + Gradient * (XEnd - X0);
	double XGap = 1.0 - (X0 + 0.5 - XEnd);
	int32 XPixel1 = XEnd;// this will be used in the main loop
	int32 YPixel1 = static_cast<int32>(YEnd);
	if (bSteep)
	{
		DrawPixel(YPixel1, XPixel1, ColorWithBrightNess((1.0 - FPart(YEnd)) * XGap));
		for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
		{
			DrawPixel(YPixel1 + WIndex + 1, XPixel1, ColorWithBrightNess(1));
		}
		DrawPixel(YPixel1 + W, XPixel1, ColorWithBrightNess(FPart(YEnd) * XGap));
	}
	else
	{
		DrawPixel(XPixel1, YPixel1, ColorWithBrightNess((1.0 - FPart(YEnd)) * XGap));
		for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
		{
			DrawPixel(XPixel1, YPixel1 + WIndex + 1, ColorWithBrightNess(1));
		}
		DrawPixel(XPixel1, YPixel1 + W, ColorWithBrightNess(FPart(YEnd) * XGap));
	}

	double InterY = YEnd + Gradient; // first y-intersection for the main loop

	// handle second endpoint
	XEnd = static_cast<int32>(X1);
	YEnd = Y1 - (W - 1) * 0.5 + Gradient * (XEnd - X1);
	XGap = 1 - (X1 + 0.5 - XEnd);
	int32 XPixel2 = XEnd; //this will be used in the main loop
	int32 YPixel2 = static_cast<int32>(YEnd);
	if (bSteep)
	{
		DrawPixel(YPixel2, XPixel2, ColorWithBrightNess((1.0 - FPart(YEnd)) * XGap));
		for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
		{
			DrawPixel(YPixel2 + WIndex + 1, XPixel2, ColorWithBrightNess(1));
		}
		DrawPixel(YPixel2 + static_cast<int32>(W), XPixel2, ColorWithBrightNess(FPart(YEnd) * XGap));
	}
	else
	{
		DrawPixel(XPixel2, YPixel2, ColorWithBrightNess((1.0 - FPart(YEnd)) * XGap));
		for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
		{
			DrawPixel(XPixel2, YPixel2 + WIndex + 1, ColorWithBrightNess(1));
		}
		DrawPixel(XPixel2, YPixel2 + static_cast<int32>(W), ColorWithBrightNess(FPart(YEnd) * XGap));
	}

	// main loop
	if (bSteep)
	{
		for (int32 X = XPixel1 + 1; X < XPixel2; X++)
		{
			DrawPixel(static_cast<int32>(InterY), X, ColorWithBrightNess(1.0 - FPart(InterY)));
			for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
			{
				DrawPixel(static_cast<int32>(InterY) + WIndex + 1, X, ColorWithBrightNess(1));
			}
			DrawPixel(static_cast<int32>(InterY) + static_cast<int32>(W), X, ColorWithBrightNess(FPart(InterY)));
			InterY += Gradient;
		}
	}
	else
	{
		for (int32 X = XPixel1 + 1; X < XPixel2; X++)
		{
			DrawPixel(X, static_cast<int32>(InterY), ColorWithBrightNess(1.0 - FPart(InterY)));
			for (int32 WIndex = 0; WIndex < static_cast<int32>(W); WIndex++)
			{
				DrawPixel(X, static_cast<int32>(InterY) + WIndex + 1, ColorWithBrightNess(1));
			}
			DrawPixel(X, static_cast<int32>(InterY) + static_cast<int32>(W), ColorWithBrightNess(FPart(InterY)));
			InterY += Gradient;
		}
	}
}

bool FMathVMBase::RegisterConst(const FString& Name, const double Value)
{
	if (!MathVM::Utils::SanitizeName(Name))
	{
		return false;
	}

	if (Constants.Contains(Name))
	{
		return false;
	}

	Constants.Add(Name, Value);

	return true;
}

bool FMathVMBase::RegisterGlobalVariable(const FString& Name, const double Value)
{
	if (!MathVM::Utils::SanitizeName(Name))
	{
		return false;
	}

	if (GlobalVariables.Contains(Name))
	{
		GlobalVariables[Name] = Value;
	}
	else
	{
		GlobalVariables.Add(Name, Value);
	}

	return true;
}

int32 FMathVMBase::RegisterResource(TSharedPtr<IMathVMResource> Resource)
{
	return Resources.Add(Resource);
}

TSharedPtr<IMathVMResource> FMathVMBase::GetResource(const int32 Index) const
{
	if (Resources.IsValidIndex(Index))
	{
		return Resources[Index];
	}

	return nullptr;
}

FMathVM::FMathVM()
{
	RegisterFunction("abs", MathVM::BuiltinFunctions::Abs, MathVM::BuiltinFunctions::AbsArgs);
	RegisterFunction("acos", MathVM::BuiltinFunctions::ACos, MathVM::BuiltinFunctions::ACosArgs);
	RegisterFunction("all", MathVM::BuiltinFunctions::All, MathVM::BuiltinFunctions::AllArgs);
	RegisterFunction("any", MathVM::BuiltinFunctions::Any, MathVM::BuiltinFunctions::AnyArgs);
	RegisterFunction("asin", MathVM::BuiltinFunctions::ASin, MathVM::BuiltinFunctions::ASinArgs);
	RegisterFunction("atan", MathVM::BuiltinFunctions::ATan, MathVM::BuiltinFunctions::ATanArgs);
	RegisterFunction("ceil", MathVM::BuiltinFunctions::Ceil, MathVM::BuiltinFunctions::CeilArgs);
	RegisterFunction("clamp", MathVM::BuiltinFunctions::Clamp, MathVM::BuiltinFunctions::ClampArgs);
	RegisterFunction("cos", MathVM::BuiltinFunctions::Cos, MathVM::BuiltinFunctions::CosArgs);
	RegisterFunction("degrees", MathVM::BuiltinFunctions::Degrees, MathVM::BuiltinFunctions::DegreesArgs);
	RegisterFunction("distance", MathVM::BuiltinFunctions::Distance, MathVM::BuiltinFunctions::DistanceArgs);
	RegisterFunction("dot", MathVM::BuiltinFunctions::Dot, MathVM::BuiltinFunctions::DotArgs);
	RegisterFunction("equal", MathVM::BuiltinFunctions::Equal, MathVM::BuiltinFunctions::EqualArgs);
	RegisterFunction("exp", MathVM::BuiltinFunctions::Exp, MathVM::BuiltinFunctions::ExpArgs);
	RegisterFunction("exp2", MathVM::BuiltinFunctions::Exp2, MathVM::BuiltinFunctions::Exp2Args);
	RegisterFunction("floor", MathVM::BuiltinFunctions::Floor, MathVM::BuiltinFunctions::FloorArgs);
	RegisterFunction("fract", MathVM::BuiltinFunctions::Fract, MathVM::BuiltinFunctions::FractArgs);
	RegisterFunction("gradient", MathVM::BuiltinFunctions::Gradient, MathVM::BuiltinFunctions::GradientArgs);
	RegisterFunction("greater", MathVM::BuiltinFunctions::GreaterThan, MathVM::BuiltinFunctions::GreaterThanArgs);
	RegisterFunction("greater_equal", MathVM::BuiltinFunctions::GreaterEqual, MathVM::BuiltinFunctions::GreaterEqualArgs);
	RegisterFunction("hue2b", MathVM::BuiltinFunctions::Hue2B, MathVM::BuiltinFunctions::Hue2BArgs);
	RegisterFunction("hue2g", MathVM::BuiltinFunctions::Hue2G, MathVM::BuiltinFunctions::Hue2GArgs);
	RegisterFunction("hue2r", MathVM::BuiltinFunctions::Hue2R, MathVM::BuiltinFunctions::Hue2RArgs);
	RegisterFunction("length", MathVM::BuiltinFunctions::Length, MathVM::BuiltinFunctions::LengthArgs);
	RegisterFunction("lerp", MathVM::BuiltinFunctions::Lerp, MathVM::BuiltinFunctions::LerpArgs);
	RegisterFunction("less", MathVM::BuiltinFunctions::LessThan, MathVM::BuiltinFunctions::LessArgs);
	RegisterFunction("less_equal", MathVM::BuiltinFunctions::LessEqual, MathVM::BuiltinFunctions::LessEqualArgs);
	RegisterFunction("log", MathVM::BuiltinFunctions::Log, MathVM::BuiltinFunctions::LogArgs);
	RegisterFunction("log2", MathVM::BuiltinFunctions::Log2, MathVM::BuiltinFunctions::Log2Args);
	RegisterFunction("map", MathVM::BuiltinFunctions::Map, MathVM::BuiltinFunctions::MapArgs);
	RegisterFunction("max", MathVM::BuiltinFunctions::Max, MathVM::BuiltinFunctions::MaxArgs);
	RegisterFunction("min", MathVM::BuiltinFunctions::Min, MathVM::BuiltinFunctions::MinArgs);
	RegisterFunction("mod", MathVM::BuiltinFunctions::Mod, MathVM::BuiltinFunctions::ModArgs);
	RegisterFunction("not", MathVM::BuiltinFunctions::Not, MathVM::BuiltinFunctions::NotArgs);
	RegisterFunction("pow", MathVM::BuiltinFunctions::Pow, MathVM::BuiltinFunctions::PowArgs);
	RegisterFunction("radians", MathVM::BuiltinFunctions::Radians, MathVM::BuiltinFunctions::RadiansArgs);
	RegisterFunction("rand", MathVM::BuiltinFunctions::Rand, MathVM::BuiltinFunctions::RandArgs);
	RegisterFunction("round", MathVM::BuiltinFunctions::Round, MathVM::BuiltinFunctions::RoundArgs);
	RegisterFunction("round_even", MathVM::BuiltinFunctions::RoundEven, MathVM::BuiltinFunctions::RoundEvenArgs);
	RegisterFunction("sign", MathVM::BuiltinFunctions::Sign, MathVM::BuiltinFunctions::SignArgs);
	RegisterFunction("sin", MathVM::BuiltinFunctions::Sin, MathVM::BuiltinFunctions::SinArgs);
	RegisterFunction("sqrt", MathVM::BuiltinFunctions::Sqrt, MathVM::BuiltinFunctions::SqrtArgs);
	RegisterFunction("step", MathVM::BuiltinFunctions::Step, MathVM::BuiltinFunctions::StepArgs);
	RegisterFunction("tan", MathVM::BuiltinFunctions::Tan, MathVM::BuiltinFunctions::TanArgs);
	RegisterFunction("trunc", MathVM::BuiltinFunctions::Trunc, MathVM::BuiltinFunctions::TruncArgs);

	// Resources functions
	RegisterFunction("read", MathVM::BuiltinFunctions::Read, MathVM::BuiltinFunctions::ReadArgs);
	RegisterFunction("write", MathVM::BuiltinFunctions::Write, MathVM::BuiltinFunctions::WriteArgs);

	RegisterConst("PI", UE_PI);
}