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
		bool RegisterResources(FMathVM& MathVM, const TArray<UMathVMResourceObject*>& Resources, FString& Error);
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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FMathVMTextureGenerated, UTexture*, Texture, const FString&, Error);

/**
 *
 */
UCLASS()
class MATHVM_API UMathVMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathVM")
	static UMathVMResourceObject* MathVMResourceObjectFromTexture2D(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathVM")
	static UMathVMResourceObject* MathVMResourceObjectFromCurveBase(UCurveBase* Curve);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "LocalVariables,Resources"), Category = "MathVM")
	static bool MathVMRunSimple(const FString& Code, UPARAM(ref) TMap<FString, double>& LocalVariables, const TArray<UMathVMResourceObject*>& Resources, double& Result, FString& Error);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Constants,GlobalVariables,Resources,PlotterConfig"), Category = "MathVM")
	static void MathVMPlotter(UObject* WorldContextObject, const FString& Code, const int32 NumSamples, const TMap<FString, FMathVMPlot>& VariablesToPlot, const TMap<FString, double>& Constants, TMap<FString, double>& GlobalVariables, const TArray<UMathVMResourceObject*>& Resources, const FMathVMTextureGenerated& OnTextureGenerated, const FMathVMPlotterConfig& PlotterConfig, const double DomainMin = 0, const double DomainMax = 1, const FString& SampleLocalVariable = "i");
};
