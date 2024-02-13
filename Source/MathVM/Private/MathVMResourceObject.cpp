// Copyright 2024, Roberto De Ioris.


#include "MathVMResourceObject.h"

void UMathVMResourceObject::SetMathVMResource(TSharedPtr<IMathVMResource> InMathVMResource)
{
	MathVMResource = InMathVMResource;
}

TSharedPtr<IMathVMResource> UMathVMResourceObject::GetMathVMResource() const
{
	return MathVMResource;
}