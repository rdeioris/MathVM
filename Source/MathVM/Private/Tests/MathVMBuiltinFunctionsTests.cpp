// Copyright 2024 - Roberto De Ioris.

#if WITH_DEV_AUTOMATION_TESTS
#include "MathVM.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_ATan, "MathVMBuiltinFunctions.ATan", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_ATan::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("atan(PI / 2)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0038848218538872);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Abs, "MathVMBuiltinFunctions.Abs", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Abs::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("abs(-1.1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_ACos, "MathVMBuiltinFunctions.ACos", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_ACos::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("acos(0.5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0471975511965979);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_All, "MathVMBuiltinFunctions.All", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_All::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("all(1, 2, 3, 4, 5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_AllFails, "MathVMBuiltinFunctions.AllFails", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_AllFails::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("all(1, 2, 0, 4, 5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Ceil, "MathVMBuiltinFunctions.Ceil", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Ceil::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("ceil(1.1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Ceil1, "MathVMBuiltinFunctions.Ceil1", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Ceil1::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("ceil(3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 3);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Trunc, "MathVMBuiltinFunctions.Trunc", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Trunc::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("trunc(3.99)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 3.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Sqrt, "MathVMBuiltinFunctions.Sqrt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Sqrt::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("sqrt(4)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 2.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Radians, "MathVMBuiltinFunctions.Radians", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Radians::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("radians(180)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, static_cast<double>(UE_PI));

	return true;
}

#endif
