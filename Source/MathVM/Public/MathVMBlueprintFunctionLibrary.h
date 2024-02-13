// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MathVM.h"
#include "MathVMBlueprintFunctionLibrary.generated.h"

class MATHVM_API FMathVMTexture2DResource : public IMathVMResource
{
public:
	FMathVMTexture2DResource(UTexture2D* Texture);
	virtual double Read(const TArray<double>& Args) const override;
	virtual void Write(const TArray<double>& Args) override;

protected:
	TArray<uint8> Pixels;
	int32 Width = 0;
	int32 Height = 0;
	EPixelFormat PixelFormat = EPixelFormat::PF_Unknown;
};

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
	static TSharedPtr<IMathVMResource> MathVMResourceFromTexture2D(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "LocalVariables,Resources"), Category = "MathVM")
	static bool MathVMRunSimple(const FString& Code, const TMap<FString, double>& LocalVariables, const TArray<UObject*>& Resources, double& Result, FString& Error);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Constants,GlobalVariables,Resources,PlotterConfig"), Category = "MathVM")
	static void MathVMPlotter(UObject* WorldContextObject, const FString& Code, const int32 NumSamples, const TMap<FString, FMathVMPlot>& VariablesToPlot, const TMap<FString, double>& Constants, TMap<FString, double>& GlobalVariables, const TArray<UObject*>& Resources, const FMathVMTextureGenerated& OnTextureGenerated, const FMathVMPlotterConfig& PlotterConfig, const double DomainMin = 0, const double DomainMax = 1, const FString& SampleLocalVariable = "i");
};
