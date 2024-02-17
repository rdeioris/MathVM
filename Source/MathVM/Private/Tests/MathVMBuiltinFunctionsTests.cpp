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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Min, "MathVMBuiltinFunctions.Min", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Min::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("min(5, 3, 4, 5, 2.1, 9)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 2.1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Max, "MathVMBuiltinFunctions.Max", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Max::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("max(5, 3, 99, 4, 5, 2.1, 9)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 99.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Fract, "MathVMBuiltinFunctions.Fract", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Fract::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("fract(300.123)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.123);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Pow, "MathVMBuiltinFunctions.Pow", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Pow::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("pow(3, 2)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 9.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Sign, "MathVMBuiltinFunctions.Sign", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Sign::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("sign(-3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, -1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_SignZero, "MathVMBuiltinFunctions.SignZero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_SignZero::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("sign(0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Any, "MathVMBuiltinFunctions.Any", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Any::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("any(-3, 0, 0, 0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Any2, "MathVMBuiltinFunctions.Any2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Any2::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("any(0, 0, 0, 0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Equal, "MathVMBuiltinFunctions.Equal", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Equal::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("equal(1, 1, 1, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_EqualNot, "MathVMBuiltinFunctions.EqualNot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_EqualNot::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("equal(1, 0, 1, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Clamp, "MathVMBuiltinFunctions.Clamp", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Clamp::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("clamp(100, 0, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Sin, "MathVMBuiltinFunctions.Sin", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Sin::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("sin(PI)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Not, "MathVMBuiltinFunctions.Not", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Not::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("not(3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_NotZero, "MathVMBuiltinFunctions.NotZero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_NotZero::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("not(0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_NotNeg, "MathVMBuiltinFunctions.NotNeg", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_NotNeg::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("not(-10)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Length, "MathVMBuiltinFunctions.Length", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Length::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("length(1, 2, 3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 3.7416573867739413);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Degrees, "MathVMBuiltinFunctions.Degrees", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Degrees::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("degrees(PI)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 180.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Lerp, "MathVMBuiltinFunctions.Lerp", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Lerp::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("lerp(0, 100, 0.5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 50.0);

	return true;
}
#endif
