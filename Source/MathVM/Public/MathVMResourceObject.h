// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MathVMResources.h"
#include "MathVMResourceObject.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MATHVM_API UMathVMResourceObject : public UObject
{
	GENERATED_BODY()

public:
	void SetMathVMResource(TSharedPtr<IMathVMResource> InMathVMResource);

	TSharedPtr<IMathVMResource> GetMathVMResource() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MathVM")
	double Read(const TArray<double>& Args);

	UFUNCTION(BlueprintCallable, Category = "MathVM")
	void Write(const TArray<double>& Args);

protected:
	TSharedPtr<IMathVMResource> MathVMResource = nullptr;
};
