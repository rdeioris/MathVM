// Copyright 2024, Roberto De Ioris.

#include "MathVMResources.h"
#include "TextureResource.h"

FMathVMTexture2DResource::FMathVMTexture2DResource(UTexture2D* Texture)
{
	const TIndirectArray<FTexture2DMipMap>& Mips = Texture->GetPlatformMips();
	if (Mips.Num() > 0)
	{
		Width = Mips[0].SizeX;
		Height = Mips[0].SizeY;
		PixelFormat = Texture->GetPixelFormat();
		if (Width > 0 && Height > 0)
		{
			const void* Data = Mips[0].BulkData.LockReadOnly();
			if (Data)
			{
				Pixels.AddUninitialized(Mips[0].BulkData.GetBulkDataSize());
				FMemory::Memcpy(Pixels.GetData(), Data, Pixels.Num());
				Mips[0].BulkData.Unlock();
			}
		}
	}
}

double FMathVMTexture2DResource::Read(const TArray<double>& Args) const
{
	if (Args.Num() < 1 || Pixels.IsEmpty())
	{
		return 0;
	}

	const int32 Channel = static_cast<int32>(Args[0]);

	double U = 0;
	double V = 0;

	if (Args.Num() > 1)
	{
		U = Args[1];
		if (Args.Num() > 2)
		{
			V = Args[2];
		}
	}

	int32 X = 0;
	int32 Y = 0;

	if (Width > 1)
	{
		X = static_cast<int32>(Width * U) % (Width - 1);
	}

	if (Height > 1)
	{
		Y = static_cast<int32>(Height * V) % (Height - 1);
	}

	switch (PixelFormat)
	{
	case(EPixelFormat::PF_B8G8R8A8):
	{
		int32 BGRChannel = Channel;
		if (BGRChannel == 0)
		{
			BGRChannel = 2;
		}
		else if (BGRChannel == 2)
		{
			BGRChannel = 0;
		}
		const int32 Offset = ((Y * Width + X) * 4) + BGRChannel;
		if (Pixels.IsValidIndex(Offset))
		{
			return Pixels[Offset] / 255.0;
		}
	}
	break;
	case(EPixelFormat::PF_R8G8B8A8):
	{
		const int32 Offset = ((Y * Width + X) * 4) + Channel;
		if (Pixels.IsValidIndex(Offset))
		{
			return Pixels[Offset] / 255.0;
		}
	}
	break;
	default:
		break;
	}
	return 0;
}

void FMathVMTexture2DResource::Write(const TArray<double>& Args)
{

}

FMathVMCurveBaseResource::FMathVMCurveBaseResource(UCurveBase* Curve)
{
	Curves = Curve->GetCurves();
}

double FMathVMCurveBaseResource::Read(const TArray<double>& Args) const
{
	if (Args.Num() < 2)
	{
		return 0;
	}

	if (Curves.IsValidIndex(Args[0]))
	{
		return Curves[Args[0]].CurveToEdit->Eval(Args[1], 0);
	}

	return 0;
}

void FMathVMCurveBaseResource::Write(const TArray<double>& Args)
{

}

FMathVMDoubleArrayResource::FMathVMDoubleArrayResource(const int32 ArraySize)
{
	Data.AddZeroed(ArraySize);
}

double FMathVMDoubleArrayResource::Read(const TArray<double>& Args) const
{
	if (Args.Num() < 1)
	{
		return 0;
	}

	if (Data.IsValidIndex(Args[0]))
	{
		return Data[Args[0]];
	}

	return 0;
}

void FMathVMDoubleArrayResource::Write(const TArray<double>& Args)
{
	if (Args.Num() < 2)
	{
		return;
	}

	if (Data.IsValidIndex(Args[0]))
	{
		Data[Args[0]] = Args[1];
	}
}

FMathVMDataTableResource::FMathVMDataTableResource(UDataTable* DataTable, const TArray<FString>& FieldNames)
{
	bool bFailed = false;
	for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
	{
		if (bFailed)
		{
			return;
		}

		TArray<double> Fields;
		Fields.AddZeroed(FieldNames.Num());

		for (int32 FieldIndex = 0; FieldIndex < FieldNames.Num(); FieldIndex++)
		{
			const FString& FieldName = FieldNames[FieldIndex];

			FProperty* Property = DataTable->FindTableProperty(*FieldName);
			if (!Property)
			{
				bFailed = true;
				break;
			}

			FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property);
			if (!NumericProperty)
			{
				bFailed = true;
				break;
			}

			uint8* RowData = NumericProperty->ContainerPtrToValuePtr<uint8>(RowIt.Value());

			if (NumericProperty->IsFloatingPoint())
			{
				Fields[FieldIndex] = NumericProperty->GetFloatingPointPropertyValue(RowData);
			}
			else if (NumericProperty->IsInteger())
			{
				Fields[FieldIndex] = static_cast<double>(NumericProperty->GetSignedIntPropertyValue(RowData));
			}
		}

		if (bFailed)
		{
			return;
		}

		Data.Add(MoveTemp(Fields));
	}
}

double FMathVMDataTableResource::Read(const TArray<double>& Args) const
{
	if (Args.Num() < 2)
	{
		return 0;
	}

	if (Data.IsValidIndex(Args[0]) && Data[Args[0]].IsValidIndex(Args[1]))
	{
		return Data[Args[0]][Args[1]];
	}

	return 0;
}

void FMathVMDataTableResource::Write(const TArray<double>& Args)
{
}