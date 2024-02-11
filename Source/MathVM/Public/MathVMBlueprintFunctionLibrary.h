// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathVM.h"
#include "MathVMBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MATHVM_API UMathVMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TUniquePtr<IMathVMResource> MathVMResourceFromTexture2D(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category="MathVM")
	bool MathVMRunSimple(const FString& Code, const TMap<FString, double>& LocalVariables, const TArray<UObject*>& Resources, double& Result, FString& Error);
};
