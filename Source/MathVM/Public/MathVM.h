// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define MATHVM_ARGS FMathVMCallContext& CallContext, const TArray<double>& Args
#define MATHVM_LAMBDA [](MATHVM_ARGS) -> bool
#define MATHVM_LAMBDA_THIS [this](MATHVM_ARGS) -> bool

enum class EMathVMTokenType : uint8
{
	Number,
	Operator,
	Function,
	OpenParenthesis,
	CloseParenthesis,
	Comma,
	Variable,
	Semicolon
};

class FMathVMBase;
struct FMathVMCallContext;

struct FMathVMToken
{
	FMathVMToken() = delete;

	FMathVMToken(const EMathVMTokenType InTokenType) : NumericValue(0), Operator(nullptr), Function(nullptr), Precedence(0), NumArgs(0), TokenType(InTokenType)
	{

	}

	FMathVMToken(const EMathVMTokenType InTokenType, const FString& InValue) : NumericValue(0), Operator(nullptr), Function(nullptr), Precedence(0), NumArgs(0), Value(InValue), TokenType(InTokenType)
	{

	}

	// Number
	FMathVMToken(const double InNumericValue) : NumericValue(InNumericValue), Operator(nullptr), Function(nullptr), Precedence(0), NumArgs(0), TokenType(EMathVMTokenType::Number)
	{

	}

	// Operator
	FMathVMToken(TFunction<bool(FMathVMCallContext&)> Callable, const int32 InPrecedence) : NumericValue(0), Operator(Callable), Function(nullptr), Precedence(InPrecedence), NumArgs(0), TokenType(EMathVMTokenType::Operator)
	{

	}

	// Function
	FMathVMToken(const FString& InValue, TFunction<bool(FMathVMCallContext& CallContext, const TArray<double>& Args)> Callable, const int32 InNumArgs) : NumericValue(0), Operator(nullptr), Function(Callable), Precedence(0), NumArgs(InNumArgs), Value(InValue), TokenType(EMathVMTokenType::Function)
	{

	}

	const double NumericValue;
	const TFunction<bool(FMathVMCallContext&)> Operator;
	const TFunction<bool(FMathVMCallContext&, const TArray<double>& Args)> Function;
	const int32 Precedence;
	const int32 NumArgs;
	int32 DetectedNumArgs = 0;
	const FString Value;
	const EMathVMTokenType TokenType;
};

using FMathVMStack = TArray<const FMathVMToken*>;
using FMathVMFunction = TFunction<bool(FMathVMCallContext& CallContext, const TArray<double>& Args)>;
using FMathVMOperator = TFunction<bool(FMathVMCallContext& CallContext)>;

class FMathVMBase
{

public:
	FMathVMBase();

	virtual ~FMathVMBase() = default;

	bool Tokenize(const FString& Code);

	bool Compile();

	bool TokenizeAndCompile(const FString& Code);

	bool Execute(TMap<FString, double>& LocalVariables, const int32 PopResults, TArray<double>& Results, FString& Error, void* LocalContext = nullptr);

	bool ExecuteAndDiscard(TMap<FString, double>& LocalVariables, FString& Error, void* LocalContext = nullptr);

	bool ExecuteOne(TMap<FString, double>& LocalVariables, double& Result, FString& Error, void* LocalContext = nullptr);

	bool ExecuteStealth(TMap<FString, double>& LocalVariables, void* LocalContext = nullptr);

	void Reset();

	const FString& GetError() const
	{
		return LastError;
	}

	bool RegisterFunction(const FString& Name, FMathVMFunction Callable, const int32 NumArgs);

protected:

	bool SetError(const FString& InError);

	bool HasPreviousToken() const
	{
		return Tokens.Num() > 0;
	}

	const FMathVMToken& GetPreviousToken() const
	{
		return Tokens.Last();
	}

	bool ExecuteStatement(const TArray<const FMathVMToken*> Statement, TMap<FString, double>& LocalVariables, const int32 PopResults, TArray<double>& Results, FString& Error, void* LocalContext);

	bool CheckAndResetAccumulator();

	bool CheckAccumulator(const bool bLastCheck);

	bool AddToken(const FMathVMToken& Token);

	TArray<FMathVMToken> Tokens;
	FString LastError;

	uint32 CurrentLine;
	uint32 CurrentColumn;
	uint32 CurrentOffset;

	FString Accumulator;
	double NumberMultiplier;

	TMap<FString, TPair<FMathVMFunction, int32>> Functions;

	FMathVMOperator OperatorAdd;
	FMathVMOperator OperatorSub;
	FMathVMOperator OperatorMul;
	FMathVMOperator OperatorDiv;
	FMathVMOperator OperatorMod;
	FMathVMOperator OperatorAssign;

	TMap<FString, double> GlobalVariables;

	TArray<TArray<const FMathVMToken*>> Statements;
};

class FMathVM : public FMathVMBase
{
public:
	FMathVM();
	FMathVM(const FMathVM& Other) = delete;
	FMathVM(FMathVM&& Other) = delete;

protected:
	FRandomStream RandomStream;
};

struct FMathVMCallContext
{
	FMathVMBase& MathVM;
	FMathVMStack Stack;
	TMap<FString, double>& LocalVariables;
	TArray<FMathVMToken> TempTokens;
	FString LastError;
	void* LocalContext = nullptr;

	FMathVMCallContext() = delete;
	FMathVMCallContext(const FMathVMCallContext& Other) = delete;
	FMathVMCallContext(FMathVMCallContext&& Other) = delete;

	FMathVMCallContext(FMathVMBase& InMathVM, TMap<FString, double>& InLocalVariables, void* InLocalContext) : MathVM(InMathVM), LocalVariables(InLocalVariables), LocalContext(InLocalContext)
	{

	}

	bool SetError(const FString& InError);

	bool PopArgument(double& Value);

	bool PopName(FString& Name);

	bool PushResult(const double Value);
};

class FMathVMModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
