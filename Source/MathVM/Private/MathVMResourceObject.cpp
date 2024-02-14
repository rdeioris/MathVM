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

double UMathVMResourceObject::Read(const TArray<double>& Args)
{
	if (!MathVMResource)
	{
		return 0;
	}

	return MathVMResource->Read(Args);
}

void UMathVMResourceObject::Write(const TArray<double>& Args)
{
	if (!MathVMResource)
	{
		return;
	}

	MathVMResource->Write(Args);
}