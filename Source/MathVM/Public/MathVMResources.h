// Copyright 2024, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveBase.h"
#include "Engine/Texture2D.h"
#include "MathVM.h"

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

class MATHVM_API FMathVMCurveBaseResource : public IMathVMResource
{
public:
	FMathVMCurveBaseResource(UCurveBase* Curve);
	virtual double Read(const TArray<double>& Args) const override;
	virtual void Write(const TArray<double>& Args) override;

protected:
	TArray<FRichCurveEditInfo> Curves;
};