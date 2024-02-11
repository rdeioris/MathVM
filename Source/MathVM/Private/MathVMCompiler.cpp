// Copyright 2024, Roberto De Ioris.

#include "MathVM.h"

bool FMathVMBase::Compile()
{
	TArray<const FMathVMToken*> OutputQueue;
	TArray<const FMathVMToken*> OperatorStack;
	TArray<int32> FunctionsArgsStack;
	TArray<bool> FunctionHasFirstArgStack;

	for (const FMathVMToken& Token : Tokens)
	{
		if (Token.TokenType == EMathVMTokenType::Number || Token.TokenType == EMathVMTokenType::Variable)
		{
			if (!FunctionHasFirstArgStack.IsEmpty())
			{
				FunctionHasFirstArgStack.Last() = true;
			}
			OutputQueue.Add(&Token);
		}
		else if (Token.TokenType == EMathVMTokenType::Function)
		{
			OperatorStack.Add(&Token);
			if (!FunctionHasFirstArgStack.IsEmpty())
			{
				FunctionHasFirstArgStack.Last() = true;
			}
			FunctionHasFirstArgStack.Add(false);
			FunctionsArgsStack.Add(0);
		}
		else if (Token.TokenType == EMathVMTokenType::Operator)
		{
			while (!OperatorStack.IsEmpty() && OperatorStack.Last()->TokenType == EMathVMTokenType::Operator && OperatorStack.Last()->Precedence <= Token.Precedence)
			{
				OutputQueue.Add(OperatorStack.Pop(false));
			}
			OperatorStack.Add(&Token);
		}
		else if (Token.TokenType == EMathVMTokenType::Comma)
		{
			while (!OperatorStack.IsEmpty() && OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				OutputQueue.Add(OperatorStack.Pop(false));
			}

			if (FunctionHasFirstArgStack.IsEmpty() || !FunctionHasFirstArgStack.Last())
			{
				return SetError("Commas are expected only after the first argument of a function");
			}

			FunctionsArgsStack.Last()++;

			FunctionHasFirstArgStack.Last() = false;
		}
		else if (Token.TokenType == EMathVMTokenType::OpenParenthesis)
		{
			OperatorStack.Add(&Token);
		}
		else if (Token.TokenType == EMathVMTokenType::CloseParenthesis)
		{
			while (!OperatorStack.IsEmpty() && OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				OutputQueue.Add(OperatorStack.Pop(false));
			}
			if (OperatorStack.IsEmpty() || OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				return SetError("Expected open parenthesis");
			}
			OperatorStack.Pop(false);

			if (!OperatorStack.IsEmpty())
			{
				if (OperatorStack.Last()->TokenType == EMathVMTokenType::Function)
				{
					// this is basically the only case the compiler needs a const_cast
					FMathVMToken* FunctionToken = const_cast<FMathVMToken*>(OperatorStack.Last());

					FunctionToken->DetectedNumArgs = FunctionsArgsStack.Pop(false) + (FunctionHasFirstArgStack.Pop() ? 1 : 0);

					if (FunctionToken->NumArgs >= 0 && FunctionToken->DetectedNumArgs != FunctionToken->NumArgs)
					{
						return SetError(FString::Printf(TEXT("Function %s expects %d argument%s (detected %d)"), *(FunctionToken->Value), FunctionToken->NumArgs, FunctionToken->NumArgs == 1 ? TEXT("") : TEXT("s"), FunctionToken->DetectedNumArgs));
					}
					OutputQueue.Add(OperatorStack.Pop(false));
				}
			}
		}
		else if (Token.TokenType == EMathVMTokenType::Semicolon)
		{
			while (!OperatorStack.IsEmpty())
			{
				if (OperatorStack.Last()->TokenType == EMathVMTokenType::OpenParenthesis)
				{
					return SetError("Mismatched parenthesis");
				}
				OutputQueue.Add(OperatorStack.Pop(false));
			}
			Statements.Add(OutputQueue);
			OutputQueue.Empty();
			OperatorStack.Empty();
			FunctionsArgsStack.Empty();
			FunctionHasFirstArgStack.Empty();
		}
	}

	while (!OperatorStack.IsEmpty())
	{
		if (OperatorStack.Last()->TokenType == EMathVMTokenType::OpenParenthesis)
		{
			return SetError("Mismatched parenthesis");
		}
		OutputQueue.Add(OperatorStack.Pop(false));
	}
	Statements.Add(OutputQueue);

	return true;
}