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

bool FMathVMBase::RegisterGlobalVariable(const FString& Name, const double Value)
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

int32 FMathVMBase::RegisterResource(TUniquePtr<IMathVMResource> Resource)
{
	return Resources.Add(MoveTemp(Resource));
}

FMathVM::FMathVM()
{
	RegisterFunction("sin", MathVM::BuiltinFunctions::Sin, MathVM::BuiltinFunctions::SinArgs);
	RegisterFunction("cos", MathVM::BuiltinFunctions::Cos, MathVM::BuiltinFunctions::CosArgs);
	RegisterFunction("min", MathVM::BuiltinFunctions::Min, MathVM::BuiltinFunctions::MinArgs);
	RegisterFunction("max", MathVM::BuiltinFunctions::Max, MathVM::BuiltinFunctions::MaxArgs);
	RegisterFunction("map", MathVM::BuiltinFunctions::Map, MathVM::BuiltinFunctions::MapArgs);
	RegisterFunction("rand", MathVM::BuiltinFunctions::Rand, MathVM::BuiltinFunctions::RandArgs);
	RegisterFunction("pow", MathVM::BuiltinFunctions::Pow, MathVM::BuiltinFunctions::PowArgs);
	RegisterFunction("hue2r", MathVM::BuiltinFunctions::Hue2R, MathVM::BuiltinFunctions::Hue2RArgs);
	RegisterFunction("hue2g", MathVM::BuiltinFunctions::Hue2G, MathVM::BuiltinFunctions::Hue2GArgs);
	RegisterFunction("hue2b", MathVM::BuiltinFunctions::Hue2B, MathVM::BuiltinFunctions::Hue2BArgs);
	RegisterFunction("lerp", MathVM::BuiltinFunctions::Lerp, MathVM::BuiltinFunctions::LerpArgs);
	RegisterFunction("gradient", MathVM::BuiltinFunctions::Gradient, MathVM::BuiltinFunctions::GradientArgs);
}