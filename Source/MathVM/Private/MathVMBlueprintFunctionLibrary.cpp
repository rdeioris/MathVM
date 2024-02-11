// Copyright 2024, Roberto De Ioris.


#include "MathVMBlueprintFunctionLibrary.h"

class FMathVMTexture2DResource : public IMathVMResource
{
public:
	FMathVMTexture2DResource(UTexture2D* Texture)
	{
		const TIndirectArray<FTexture2DMipMap>& Mips = Texture->GetPlatformMips();
		if (Mips.Num() > 0)
		{
			Width = Mips[0].SizeX;
			Height = Mips[0].SizeX;
			PixelFormat = Texture->GetPixelFormat();
			if (Width > 0 && Height > 0)
			{
				Pixels.AddUninitialized(Mips[0].BulkData.GetBulkDataSize());
				const void* Data = Mips[0].BulkData.LockReadOnly();
				FMemory::Memcpy(Pixels.GetData(), Data, Pixels.Num());
				Mips[0].BulkData.Unlock();
			}
		}
	}

	virtual double Read(const TArray<double>& Args) const override
	{
		if (Args.Num() < 1)
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
				return Pixels[Offset];
			}
		}
		break;
		case(EPixelFormat::PF_R8G8B8A8):
		{
			const int32 Offset = ((Y * Width + X) * 4) + Channel;
			if (Pixels.IsValidIndex(Offset))
			{
				return Pixels[Offset];
			}
		}
		break;
		default:
			break;
		}
		return 0;
	}

	virtual void Write(const double Value, const TArray<double>& Args) override
	{

	}

protected:
	TArray<uint8> Pixels;
	int32 Width = 0;
	int32 Height = 0;
	EPixelFormat PixelFormat = EPixelFormat::PF_Unknown;
};

TUniquePtr<IMathVMResource> UMathVMBlueprintFunctionLibrary::MathVResourceFromTexture2D(UTexture2D* Texture)
{
	if (!Texture)
	{
		return nullptr;
	}

	return MakeUnique<FMathVMTexture2DResource>(Texture);
}