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
	GlobalVariables[Name] = Value;
}

double FMathVMBase::GetGlobalVariable(const FString& Name)
{
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