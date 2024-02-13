// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MathVMResources.h"
#include "MathVMResourceObject.generated.h"

/**
 * 
 */
UCLASS()
class MATHVM_API UMathVMResourceObject : public UObject
{
	GENERATED_BODY()
	
public:
	void SetMathVMResource(TSharedPtr<IMathVMResource> InMathVMResource);

	TSharedPtr<IMathVMResource> GetMathVMResource() const;

protected:
	TSharedPtr<IMathVMResource> MathVMResource = nullptr;
};
