// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define MATHVM_ARGS FMathVMCallContext& CallContext, const TArray<double>& Args
#define MATHVM_LAMBDA [](MATHVM_ARGS) -> bool
#define MATHVM_LAMBDA_THIS [this](MATHVM_ARGS) -> bool
#define MATHVM_RETURN(x) return CallContext.PushResult(x)
#define MATHVM_ERROR(x) return CallContext.SetError(x)

enum class EMathVMTokenType : uint8
{
	Number,
	Operator,
	Function,
	OpenParenthesis,
	CloseParenthesis,
	Comma,
	Variable,
	Semicolon,
	Lock,
	Unlock
};

class FMathVMBase;
struct FMathVMCallContext;

struct MATHVM_API FMathVMToken
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

class MATHVM_API IMathVMResource
{
public:
	virtual ~IMathVMResource() = default;
	virtual double Read(const TArray<double>& Args) const = 0;
	virtual void Write(const TArray<double>& Args) = 0;
};

namespace MathVM
{
	namespace Utils
	{
		bool MATHVM_API SanitizeName(const FString& Name);
	}
}

class MATHVM_API FMathVMBase
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

	const FString& GetError() const;

	bool RegisterFunction(const FString& Name, FMathVMFunction Callable, const int32 NumArgs);

	bool RegisterGlobalVariable(const FString& Name, const double Value);

	bool RegisterConst(const FString& Name, const double Value);

	bool HasConst(const FString& Name) const;

	double GetConst(const FString& Name);

	bool HasGlobalVariable(const FString& Name) const;

	void SetGlobalVariable(const FString& Name, const double Value);
	double GetGlobalVariable(const FString& Name) const;

	int32 RegisterResource(TSharedPtr<IMathVMResource> Resource);

	TSharedPtr<IMathVMResource> GetResource(const int32 Index) const;

	const TMap<FString, double>& GetGlobalVariables() const;

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

	bool ExecuteStatement(FMathVMCallContext& CallContext, const TArray<const FMathVMToken*> Statement, FString& Error);

	bool CheckAndResetAccumulator();

	bool CheckAccumulator(const bool bLastCheck);

	bool AddToken(const FMathVMToken& Token);

	TArray<FMathVMToken> Tokens;
	FString LastError;

	int32 CurrentLine;
	int32 CurrentOffset;

	FString Accumulator;

	TMap<FString, TPair<FMathVMFunction, int32>> Functions;

	FMathVMOperator OperatorAdd;
	FMathVMOperator OperatorSub;
	FMathVMOperator OperatorMul;
	FMathVMOperator OperatorDiv;
	FMathVMOperator OperatorMod;
	FMathVMOperator OperatorAssign;

	TMap<FString, const double> Constants;

	TMap<FString, double> GlobalVariables;

	TArray<TArray<const FMathVMToken*>> Statements;

	TArray<TSharedPtr<IMathVMResource>> Resources;

	FCriticalSection Lock;
};

class MATHVM_API FMathVM : public FMathVMBase
{
public:
	FMathVM();
	FMathVM(const FMathVM& Other) = delete;
	FMathVM(FMathVM&& Other) = delete;
};

struct MATHVM_API FMathVMCallContext
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

	double ReadResource(const int32 Index, const TArray<double>& Args);

	void WriteResource(const int32 Index, const TArray<double>& Args);
};

class FMathVMModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
