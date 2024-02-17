// Copyright 2024 - Roberto De Ioris.

#if WITH_DEV_AUTOMATION_TESTS
#include "MathVM.h"
#include "Misc/AutomationTest.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Empty, "MathVM.Empty", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Empty::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("17");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 17.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Add, "MathVM.Add", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Add::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("1 + 2");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 3.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Precedence, "MathVM.Precedence", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Precedence::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("(1 + 2) * 3");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 9.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Assignment, "MathVM.Assignment", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Assignment::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("x = 17");

	TMap<FString, double> LocalVariables;
	FString Error;
	LocalVariables.Add("x", -1);

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteAndDiscard(LocalVariables, Error));

	TestEqual(TEXT("x"), LocalVariables["x"], 17.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_IncrementVar, "MathVM.IncrementVar", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_IncrementVar::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("x = 17; x = x + 5");

	TMap<FString, double> LocalVariables;
	FString Error;
	LocalVariables.Add("x", -1);

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteAndDiscard(LocalVariables, Error));

	TestEqual(TEXT("x"), LocalVariables["x"], 22.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_VarWithNum, "MathVM.VarWithNum", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_VarWithNum::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("x17 = 1000.123");

	TMap<FString, double> LocalVariables;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteAndDiscard(LocalVariables, Error));

	TestEqual(TEXT("x"), LocalVariables["x17"], 1000.123);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_HeavyChain, "MathVM.HeavyChain", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_HeavyChain::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("pow(0, 2);sin(cos(sin(cos(cos(5)))))");

	TMap<FString, double> LocalVariables;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteAndDiscard(LocalVariables, Error));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Comment, "MathVM.Comment", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Comment::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("# hello # 17");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 17.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_CommentNewLine, "MathVM.CommentNewLine", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_CommentNewLine::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("# hello \n# test# 22");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 22.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_Multi, "MathVM.Multi", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_Multi::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("1;2;3");

	TMap<FString, double> LocalVariables;
	TArray<double> Results;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.Execute(LocalVariables, 3, Results, Error));

	TestEqual(TEXT("Results"), Results.Num(), 3);

	TestEqual(TEXT("Result[0]"), Results[0], 3.0);
	TestEqual(TEXT("Result[1]"), Results[1], 2.0);
	TestEqual(TEXT("Result[2]"), Results[2], 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMTest_ParallelWithLock, "MathVM.ParallelWithLock", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMTest_ParallelWithLock::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.RegisterGlobalVariable("x", 0);
	MathVM.TokenizeAndCompile("{x = x + i;}");

	ParallelFor(10, [&](const int32 Index)
		{
			TMap<FString, double> LocalVariables;
			LocalVariables.Add("i", Index);

			MathVM.ExecuteStealth(LocalVariables);
		});

	TestEqual(TEXT("x"), MathVM.GetGlobalVariable("x"), 45.0);

	return true;
}

#endif
