// Copyright 2024, Roberto De Ioris.

#include "MathVM.h"

bool FMathVMBase::Compile()
{
	TArray<const FMathVMToken*> OutputQueue;
	TArray<const FMathVMToken*> OperatorStack;
	TArray<int32> FunctionsArgsStack;
	TArray<bool> FunctionHasFirstArgStack;
	bool bLocked = false;

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
				OutputQueue.Add(MATHVM_POP(OperatorStack));
			}
			OperatorStack.Add(&Token);
		}
		else if (Token.TokenType == EMathVMTokenType::Comma)
		{
			while (!OperatorStack.IsEmpty() && OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				OutputQueue.Add(MATHVM_POP(OperatorStack));
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
		else if (Token.TokenType == EMathVMTokenType::Lock)
		{
			if (!OperatorStack.IsEmpty())
			{
				return SetError("Unexpected Lock");
			}

			if (bLocked)
			{
				return SetError("Lock without Unlock");
			}
			bLocked = true;
			Statements.Add({ &Token });
		}
		else if (Token.TokenType == EMathVMTokenType::Unlock)
		{
			if (!OperatorStack.IsEmpty())
			{
				return SetError("Unexpected Unlock");
			}

			if (!bLocked)
			{
				return SetError("Unlock without Lock");
			}
			bLocked = false;
			Statements.Add({ &Token });
		}
		else if (Token.TokenType == EMathVMTokenType::CloseParenthesis)
		{
			while (!OperatorStack.IsEmpty() && OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				OutputQueue.Add(MATHVM_POP(OperatorStack));
			}
			if (OperatorStack.IsEmpty() || OperatorStack.Last()->TokenType != EMathVMTokenType::OpenParenthesis)
			{
				return SetError("Expected open parenthesis");
			}
			MATHVM_POP(OperatorStack);

			if (!OperatorStack.IsEmpty())
			{
				if (OperatorStack.Last()->TokenType == EMathVMTokenType::Function)
				{
					// this is basically the only case the compiler needs a const_cast
					FMathVMToken* FunctionToken = const_cast<FMathVMToken*>(OperatorStack.Last());

					FunctionToken->DetectedNumArgs = MATHVM_POP(FunctionsArgsStack) + (FunctionHasFirstArgStack.Pop() ? 1 : 0);

					if (FunctionToken->NumArgs >= 0 && FunctionToken->DetectedNumArgs != FunctionToken->NumArgs)
					{
						return SetError(FString::Printf(TEXT("Function %s expects %d argument%s (detected %d)"), *(FunctionToken->Value), FunctionToken->NumArgs, FunctionToken->NumArgs == 1 ? TEXT("") : TEXT("s"), FunctionToken->DetectedNumArgs));
					}
					OutputQueue.Add(MATHVM_POP(OperatorStack));
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
				OutputQueue.Add(MATHVM_POP(OperatorStack));
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
		OutputQueue.Add(MATHVM_POP(OperatorStack));
	}
	Statements.Add(OutputQueue);

	if (bLocked)
	{
		return SetError("Lock without Unlock");
	}

	return true;
}