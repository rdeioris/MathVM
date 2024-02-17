// Copyright 2024 - Roberto De Ioris.

#if WITH_DEV_AUTOMATION_TESTS
#include "MathVM.h"
#include "MathVMResources.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathVMResourcesTest_WriteDoubles, "MathVMResources.WriteDoubles", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMathVMResourcesTest_WriteDoubles::RunTest(const FString& Parameters)
{
	FMathVM MathVM;
	MathVM.RegisterResource(MakeShared<FMathVMDoubleArrayResource>(10));
	MathVM.TokenizeAndCompile("write(0, 0, 1); write(0, 1, 2); write(0, 2, 100); write(0, 3, -200); read(0, 0); read(0, 1); read(0, 2); read(0, 3)");

	TMap<FString, double> LocalVariables;
	TArray<double> Results;
	FString Error;

	TestTrue(TEXT("bSuccess"), MathVM.Execute(LocalVariables, 4, Results, Error));

	TestEqual(TEXT("Results"), Results.Num(), 4);

	TestEqual(TEXT("Results[0]"), Results[0], -200.0);
	TestEqual(TEXT("Results[1]"), Results[1], 100.0);
	TestEqual(TEXT("Results[2]"), Results[2], 2.0);
	TestEqual(TEXT("Results[3]"), Results[3], 1.0);

	return true;
}

#endif
