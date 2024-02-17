// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MathVMResourceObject.h"
#include "MathVMBlueprintFunctionLibrary.generated.h"

namespace MathVM
{
	namespace BlueprintUtility
	{
		MATHVM_API bool RegisterResources(FMathVM& MathVM, const TArray<UMathVMResourceObject*>& Resources, FString& Error);
	}
}

UENUM()
enum class EMathVMPlotterShape : uint8
{
	Line,
	Box,
	LineAndPoint
};

USTRUCT(BlueprintType)
struct FMathVMPlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	EMathVMPlotterShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	float Thickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor Color2;

	FMathVMPlot()
	{
		Shape = EMathVMPlotterShape::Line;
		Color = FLinearColor::Black;
		Color2 = FLinearColor::Black;
		Thickness = 1;
	}
};

USTRUCT(BlueprintType)
struct FMathVMText
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	UFont* Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FVector2D Scaling;

	
	FMathVMText()
	{
		Color = FLinearColor::Black;
		Position = FVector2D::ZeroVector;
		Font = nullptr;
		Scaling = FVector2D::One();
	}
};

USTRUCT(BlueprintType)
struct FMathVMPlotterConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	int32 TextureWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	int32 TextureHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FMargin BorderSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor BorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	float BorderThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	float GridVerticalScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	float GridThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MathVM")
	FLinearColor GridColor;

	FMathVMPlotterConfig()
	{
		TextureWidth = 1024;
		TextureHeight = 1024;
		BackgroundColor = FLinearColor::White;
		BorderSize = FMargin(2, 2, 2, 2);
		BorderColor = FLinearColor::Black;
		BorderThickness = 1;
		GridVerticalScale = 0;
		GridThickness = 1;
		GridColor = FLinearColor::Gray;
	}
};

USTRUCT(BlueprintType)
struct FMathVMEvaluationResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathVM")
	bool bSuccess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathVM")
	FString Error;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathVM")
	TMap<FString, double> GlobalVariables;

	FMathVMEvaluationResult()
	{
		bSuccess = false;
	}

	FMathVMEvaluationResult(const FString& InError)
	{
		bSuccess = false;
		Error = InError;
	}

	FMathVMEvaluationResult(const TMap<FString, double>& InGlobalVariables)
	{
		bSuccess = true;
		GlobalVariables = InGlobalVariables;
	}
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FMathVMEvaluated, const bool, bSuccess, const FString&, Error);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMathVMEvaluatedWithResult, const FMathVMEvaluationResult&, EvaluationResult);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FMathVMPlotGenerated, UTexture*, Texture, const FMathVMEvaluationResult&, EvaluationResult);

/**
 *
 */
UCLASS()
class MATHVM_API UMathVMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MathVM")
	static UMathVMResourceObject* MathVMResourceObjectFromTexture2D(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "MathVM")
	static UMathVMResourceObject* MathVMResourceObjectFromCurveBase(UCurveBase* Curve);

	UFUNCTION(BlueprintCallable, Category = "MathVM")
	static UMathVMResourceObject* MathVMResourceObjectAsDoubleArray(const int32 ArraySize);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "LocalVariables,Resources"), Category = "MathVM")
	static bool MathVMRunSimple(const FString& Code, UPARAM(ref) TMap<FString, double>& LocalVariables, const TArray<UMathVMResourceObject*>& Resources, double& Result, FString& Error);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "GlobalVariables,Constants,Resources"), Category = "MathVM")
	static void MathVMRun(const FString& Code, const TMap<FString, double>& GlobalVariables, const TMap<FString, double>& Constants, const TArray<UMathVMResourceObject*>& Resources, const FMathVMEvaluatedWithResult& OnEvaluated, const int32 NumThreads = 1, const FString& ThreadIdLocalVariable = "i");

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "TextsToPlot,Constants,GlobalVariables,Resources,PlotterConfig"), Category = "MathVM")
	static void MathVMPlotter(UObject* WorldContextObject, const FString& Code, const int32 NumSamples, const TMap<FString, FMathVMPlot>& VariablesToPlot, const TArray<FMathVMText>& TextsToPlot, const TMap<FString, double>& Constants, const TMap<FString, double>& GlobalVariables, const TArray<UMathVMResourceObject*>& Resources, const FMathVMPlotGenerated& OnPlotGenerated, const FMathVMPlotterConfig& PlotterConfig, const double DomainMin = 0, const double DomainMax = 1, const FString& SampleLocalVariable = "i");
};
