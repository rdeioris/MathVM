// Copyright 2024, Roberto De Ioris.

#include "MathVM.h"

bool FMathVMBase::Tokenize(const FString& Code)
{
	const int32 CodeLen = Code.Len();
	Accumulator = "";
	double NumberMultiplier = 1;
	bool bIsInComment = false;

	CurrentLine = 1;
	CurrentOffset = 0;
	Tokens.Empty();

	while (CurrentOffset < CodeLen)
	{
		TCHAR Char = Code[CurrentOffset++];

		if (Char == '\n')
		{
			CurrentLine++;
		}

		if (bIsInComment)
		{
			if (Char == '\n' || Char == '#')
			{
				bIsInComment = false;
			}
			continue;
		}

		const bool bStartsWithPoint = Char == '.';

		if ((Char >= '0' && Char <= '9') || bStartsWithPoint)
		{
			// number after variable/function name?
			if (!Accumulator.IsEmpty() && !bStartsWithPoint)
			{
				Accumulator += Char;
				continue;
			}

			if (!CheckAndResetAccumulator())
			{
				return false;
			}

			FString NumberAccumulator = "";
			NumberAccumulator += Char;

			while (CurrentOffset < CodeLen)
			{
				Char = Code[CurrentOffset];
				if (Char >= '0' && Char <= '9')
				{
					NumberAccumulator += Char;
				}
				else
				{
					break;
				}

				CurrentOffset++;
			}

			if (Char == '.' && !bStartsWithPoint)
			{
				NumberAccumulator += Char;
				CurrentOffset++;
				while (CurrentOffset < CodeLen)
				{
					Char = Code[CurrentOffset];
					if (Char >= '0' && Char <= '9')
					{
						NumberAccumulator += Char;
					}
					else
					{
						break;
					}

					CurrentOffset++;
				}
			}

			if (Char == 'e' || Char == 'E')
			{
				NumberAccumulator += Char;
				CurrentOffset++;
				Char = Code[CurrentOffset];

				if (Char == '+' || Char == '-')
				{
					NumberAccumulator += Char;
					CurrentOffset++;
					while (CurrentOffset < CodeLen)
					{
						Char = Code[CurrentOffset];
						if (Char >= '0' && Char <= '9')
						{
							NumberAccumulator += Char;
						}
						else
						{
							break;
						}

						CurrentOffset++;
					}
				}
			}

			const double NumericValue = FCString::Atod(*NumberAccumulator);
			if (!AddToken(FMathVMToken(NumericValue * NumberMultiplier)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '-')
		{
			if (!HasPreviousToken())
			{
				NumberMultiplier = -1;
			}
			else if (GetPreviousToken().TokenType == EMathVMTokenType::OpenParenthesis || GetPreviousToken().TokenType == EMathVMTokenType::Operator || GetPreviousToken().TokenType == EMathVMTokenType::Comma || GetPreviousToken().TokenType == EMathVMTokenType::Semicolon)
			{
				NumberMultiplier = -1;
			}
			else
			{
				if (!AddToken(FMathVMToken(OperatorSub, 6)))
				{
					return false;
				}
				NumberMultiplier = 1;
			}
		}
		else if (Char == '+')
		{
			if (!HasPreviousToken())
			{
				NumberMultiplier = 1;
			}
			else if (GetPreviousToken().TokenType == EMathVMTokenType::OpenParenthesis || GetPreviousToken().TokenType == EMathVMTokenType::Operator || GetPreviousToken().TokenType == EMathVMTokenType::Comma || GetPreviousToken().TokenType == EMathVMTokenType::Semicolon)
			{
				NumberMultiplier = 1;
			}
			else
			{
				if (!AddToken(FMathVMToken(OperatorAdd, 6)))
				{
					return false;
				}
				NumberMultiplier = 1;
			}
		}
		else if (Char == '*')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(OperatorMul, 5)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '/')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(OperatorDiv, 5)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '%')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(OperatorMod, 5)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '=')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(OperatorAssign, 17)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '(')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::OpenParenthesis)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == ')')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::CloseParenthesis)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '{')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::Lock)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '}')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::Unlock)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == ',')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::Comma)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == ';')
		{
			if (!CheckAndResetAccumulator() || !AddToken(FMathVMToken(EMathVMTokenType::Semicolon)))
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
		{
			if (!CheckAndResetAccumulator())
			{
				return false;
			}
			NumberMultiplier = 1;
		}
		else if (Char == '#')
		{
			if (!CheckAndResetAccumulator())
			{
				return false;
			}
			NumberMultiplier = 1;
			bIsInComment = true;
		}
		else if ((Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z') || Char == '_')
		{
			Accumulator += Char;
			NumberMultiplier = 1;
		}
		else
		{
			return SetError(FString::Printf(TEXT("Unexpected char: %c"), Char));
		}
	}

	return CheckAccumulator(true);
}

bool FMathVMBase::CheckAndResetAccumulator()
{
	bool bSuccess = CheckAccumulator(false);
	Accumulator = "";
	return bSuccess;
}

bool FMathVMBase::CheckAccumulator(const bool bLastCheck)
{
	if (Accumulator.IsEmpty())
	{
		return true;
	}

	if (Functions.Contains(Accumulator))
	{
		if (bLastCheck)
		{
			return SetError(FString::Printf(TEXT("Expected open parenthesis after function %s"), *Accumulator));
		}

		return AddToken(FMathVMToken(Accumulator, Functions[Accumulator].Key, Functions[Accumulator].Value));
	}

	return AddToken(FMathVMToken(EMathVMTokenType::Variable, Accumulator));
}

bool FMathVMBase::AddToken(const FMathVMToken& Token)
{
	if (!Tokens.IsEmpty())
	{
		if (Tokens.Last().TokenType == EMathVMTokenType::Function && Token.TokenType != EMathVMTokenType::OpenParenthesis)
		{
			return SetError(FString::Printf(TEXT("Expected open parenthesis after function %s"), *Token.Value));
		}
	}

	Tokens.Add(Token);
	return true;
}