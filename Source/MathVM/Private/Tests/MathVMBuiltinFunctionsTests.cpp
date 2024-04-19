// Copyright 2024 - Roberto De Ioris.

#if WITH_DEV_AUTOMATION_TESTS
#include "MathVM.h"
#include "Misc/AutomationTest.h"

// this is just a hack for allowing compilation on UE < 5.3, the test suite is meant to be run on UE >= 5.3
#if ENGINE_MINOR_VERSION < 3
static bool TestNearlyEqual(const TCHAR* What, const double A, const double B)
{
	return true;
}
#endif

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Mean, "MathVMBuiltinFunctions.Mean", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Mean::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("mean(3, 3, 3, 3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestEqual(TEXT("Result"), Result, 3.0);

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Floor, "MathVMBuiltinFunctions.Floor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Floor::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("floor(1.9)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_FloorZero, "MathVMBuiltinFunctions.FloorZero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_FloorZero::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("floor(0.7)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Cos, "MathVMBuiltinFunctions.Cos", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Cos::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("cos(0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Less, "MathVMBuiltinFunctions.Less", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Less::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("less(0, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_LessNot, "MathVMBuiltinFunctions.LessNot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_LessNot::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("less(3, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_LessEqual, "MathVMBuiltinFunctions.LessEqual", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_LessEqual::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("less_equal(1, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Gradient, "MathVMBuiltinFunctions.Gradient", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Gradient::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("gradient(0.75, 0, 1, 0.5, 100, 1, 1000)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 550.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_RoundEven, "MathVMBuiltinFunctions.RoundEven", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_RoundEven::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("round_even(3.5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 4.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_RoundEven2, "MathVMBuiltinFunctions.RoundEven2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_RoundEven2::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("round_even(4.5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 4.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Exp, "MathVMBuiltinFunctions.Exp", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Exp::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("exp(2)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 7.38905609893065);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Exp2, "MathVMBuiltinFunctions.Exp2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Exp2::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("exp2(8)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 256.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Greater, "MathVMBuiltinFunctions.Greater", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Greater::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("greater(5, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_GreaterNot, "MathVMBuiltinFunctions.GreaterNot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_GreaterNot::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("greater(2, 5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_GreaterEqual, "MathVMBuiltinFunctions.GreaterEqual", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_GreaterEqual::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("greater_equal(5, 5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_GreaterEqualNot, "MathVMBuiltinFunctions.GreaterEqualNot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_GreaterEqualNot::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("greater_equal(4, 5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Rand, "MathVMBuiltinFunctions.Rand", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Rand::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("rand(0, 1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestTrue(TEXT("Result"), Result >= 0.0 && Result <= 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_RandZero, "MathVMBuiltinFunctions.RandZero", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_RandZero::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("rand(0, 0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_ASin, "MathVMBuiltinFunctions.ASin", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_ASin::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("asin(1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.5707963267948966);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Map, "MathVMBuiltinFunctions.Map", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Map::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("map(0.5, 0, 100, 1, 200)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 150.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Distance, "MathVMBuiltinFunctions.Distance", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Distance::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("distance(2, 2, 2, 3, 3, 3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.7320508075688772);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Dot, "MathVMBuiltinFunctions.Dot", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Dot::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("dot(1, 0, 0, 1, 0, 0)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Log, "MathVMBuiltinFunctions.Log", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Log::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("log(10)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 2.302585092994046);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Log2, "MathVMBuiltinFunctions.Log2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Log2::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("log2(10)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 3.321928094887362);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Log10, "MathVMBuiltinFunctions.Log10", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Log10::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("log10(3)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0.47712125471966244);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_LogX, "MathVMBuiltinFunctions.LogX", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_LogX::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("logx(3, 4)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.2618595071429148);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Tan, "MathVMBuiltinFunctions.Tan", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Tan::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("tan(1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.5574077246549023);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Round, "MathVMBuiltinFunctions.Round", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Round::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("round(2.5)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 3.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Round2, "MathVMBuiltinFunctions.Round2", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Round2::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("round(2.4)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 2.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Mod, "MathVMBuiltinFunctions.Mod", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Mod::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("mod(1, 2)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1.0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Hue2B, "MathVMBuiltinFunctions.Hue2B", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Hue2B::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("hue2b(1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Hue2G, "MathVMBuiltinFunctions.Hue2G", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Hue2G::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("hue2g(1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MathVMBuiltinFunctions_Hue2R, "MathVMBuiltinFunctions.Hue2R", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MathVMBuiltinFunctions_Hue2R::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.TokenizeAndCompile("hue2r(1)");

	TMap<FString, double> LocalVariables;
	double Result = 0;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.ExecuteOne(LocalVariables, Result, Error));

	TestNearlyEqual(TEXT("Result"), Result, 1);

	return true;
}
#endif
