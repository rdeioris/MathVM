// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FMathVMTextureGenerated, UTexture2D*, Texture, const FString&, Error);

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

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "Constants,GlobalVariables,Resources"), Category = "MathVM")
	static void MathVMPlotter(const FString& Code, const int32 NumSamples, const TMap<FString, FColor>& VariablesToPlot, const TMap<FString, double>& Constants, TMap<FString, double>& GlobalVariables, const TArray<UObject*>& Resources, const FMathVMTextureGenerated& OnTextureGenerated, const double DomainMin = 0, const double DomainMax = 1, const FString& SampleLocalVariable = "i", const int32 TextureWidth = 256, const int32 TextureHeight = 256);
};
