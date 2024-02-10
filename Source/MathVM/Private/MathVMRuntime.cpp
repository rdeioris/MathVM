// Copyright 2024, Roberto De Ioris.

#include "MathVM.h"

bool FMathVMBase::TokenizeAndCompile(const FString& Code)
{
	if (!Tokenize(Code))
	{
		return false;
	}

	return Compile();
}

bool FMathVMBase::ExecuteStatement(const TArray<const FMathVMToken*> Statement, TMap<FString, double>& LocalVariables, const int32 PopResults, TArray<double>& Results, FString& Error, void* LocalContext)
{
	FMathVMCallContext CallContext(*this, LocalVariables, LocalContext);
	// this ensures no dangling pointers (no reallocation)
	CallContext.TempTokens.Reserve(Statement.Num());

	for (const FMathVMToken* Token : Statement)
	{
		if (Token->TokenType == EMathVMTokenType::Operator)
		{
			if (!Token->Operator(CallContext))
			{
				Error = CallContext.LastError;
				return false;
			}
		}
		else if (Token->TokenType == EMathVMTokenType::Function)
		{
			TArray<double> Args;
			Args.AddUninitialized(Token->DetectedNumArgs);

			for (int32 ArgIndex = 0; ArgIndex < Token->DetectedNumArgs; ArgIndex++)
			{
				if (!CallContext.PopArgument(Args[ArgIndex]))
				{
					Error = CallContext.LastError;
					return false;
				}
			}
			if (!Token->Function(CallContext, Args))
			{
				Error = CallContext.LastError;
				return false;
			}
		}
		else
		{
			CallContext.Stack.Add(Token);
		}
	}

	for (int32 PopIndex = 0; PopIndex < PopResults; PopIndex++)
	{
		if (CallContext.Stack.IsEmpty())
		{
			Error = FString::Printf(TEXT("Unable to pop result %d"), PopIndex);
			return false;
		}

		const FMathVMToken* LastToken = CallContext.Stack.Pop(false);

		if (LastToken->TokenType != EMathVMTokenType::Number)
		{
			Error = FString::Printf(TEXT("result %d is not a number"), PopIndex);
			return false;
		}

		Results.Add(LastToken->NumericValue);
	}

	return true;
}

bool FMathVMBase::Execute(TMap<FString, double>& LocalVariables, const int32 PopResults, TArray<double>& Results, FString& Error, void* LocalContext)
{
	for (const TArray<const FMathVMToken*> Statement : Statements)
	{
		if (!ExecuteStatement(Statement, LocalVariables, PopResults, Results, Error, LocalContext))
		{
			return false;
		}
	}

	return true;
}

bool FMathVMBase::ExecuteAndDiscard(TMap<FString, double>& LocalVariables, FString& Error, void* LocalContext)
{
	TArray<double> EmptyResults;
	return Execute(LocalVariables, 0, EmptyResults, Error, LocalContext);
}

bool FMathVMBase::ExecuteOne(TMap<FString, double>& LocalVariables, double& Result, FString& Error, void* LocalContext)
{
	TArray<double> SingleResult;
	if (!Execute(LocalVariables, 1, SingleResult, Error, LocalContext))
	{
		return false;
	}
	Result = SingleResult[0];
	return true;
}

bool FMathVMBase::ExecuteStealth(TMap<FString, double>& LocalVariables, void* LocalContext)
{
	FString DiscardedError;
	return ExecuteAndDiscard(LocalVariables, DiscardedError, LocalContext);
}

bool FMathVMCallContext::SetError(const FString& InError)
{
	LastError = InError;
	return false;
}

bool FMathVMCallContext::PopArgument(double& Value)
{
	if (Stack.IsEmpty())
	{
		return false;
	}

	const FMathVMToken* Token = Stack.Pop(false);

	if (Token->TokenType == EMathVMTokenType::Variable)
	{
		if (LocalVariables.Contains(Token->Value))
		{
			Value = LocalVariables[Token->Value];
			return true;
		}

#if 0
		if (MathVM.GlobalVariables.Contains(Token->Value))
		{
			Value = GlobalVariables[Token->Value];
			return true;
		}
#endif

		return false;
	}

	if (Token->TokenType == EMathVMTokenType::Number)
	{
		Value = Token->NumericValue;
		return true;
	}

	return false;
}

bool FMathVMCallContext::PopName(FString& Name)
{
	if (Stack.IsEmpty())
	{
		return false;
	}

	const FMathVMToken* Token = Stack.Pop(false);

	if (Token->TokenType == EMathVMTokenType::Variable)
	{
		Name = Token->Value;
		return true;
	}

	return false;
}

bool FMathVMCallContext::PushResult(const double Value)
{
	const int32 NewTempToken = TempTokens.Add(FMathVMToken(Value));
	Stack.Add(&(TempTokens[NewTempToken]));
	return true;
}