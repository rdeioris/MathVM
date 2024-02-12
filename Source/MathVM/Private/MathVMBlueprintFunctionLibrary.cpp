// Copyright 2024, Roberto De Ioris.


#include "MathVMBlueprintFunctionLibrary.h"


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

TSharedPtr<IMathVMResource> UMathVMBlueprintFunctionLibrary::MathVMResourceFromTexture2D(UTexture2D* Texture)
{
	if (!Texture)
	{
		return nullptr;
	}

	return MakeShared<FMathVMTexture2DResource>(Texture);
}

bool UMathVMBlueprintFunctionLibrary::MathVMRunSimple(const FString& Code, const TMap<FString, double>& LocalVariables, const TArray<UObject*>& Resources, double& Result, FString& Error)
{
	if (Code.IsEmpty())
	{
		Error = "Empty Code";
		return false;
	}

	FMathVM MathVM;
	for (int32 ResourceIndex = 0; ResourceIndex < Resources.Num(); ResourceIndex++)
	{
		UObject* Resource = Resources[ResourceIndex];
		if (!Resource)
		{
			Error = FString::Printf(TEXT("Null resource at index %d"), ResourceIndex);
			return false;
		}
		if (Resource->IsA<UTexture2D>())
		{
			TSharedPtr<IMathVMResource> NewResource = MathVMResourceFromTexture2D(Cast<UTexture2D>(Resource));
			if (!NewResource)
			{
				Error = FString::Printf(TEXT("Null resource at index %d"), ResourceIndex);
				return false;
			}
			if (MathVM.RegisterResource(NewResource) < 0)
			{
				Error = FString::Printf(TEXT("Unable to register resource at index %d"), ResourceIndex);
				return false;
			}
		}
		else
		{
			Error = FString::Printf(TEXT("Unsupported resource type: %s"), *(Resource->GetClass()->GetName()));
			return false;
		}
	}

	if (!MathVM.TokenizeAndCompile(Code))
	{
		Error = MathVM.GetError();
		return false;
	}

	TMap<FString, double> LocalVariablesCopy = LocalVariables;
	return MathVM.ExecuteOne(LocalVariablesCopy, Result, Error);
}

void UMathVMBlueprintFunctionLibrary::MathVMPlotter(const FString& Code, const int32 NumSamples, const TMap<FString, FColor>& VariablesToPlot, const TMap<FString, double>& Constants, TMap<FString, double>& GlobalVariables, const TArray<UObject*>& Resources, const FMathVMTextureGenerated& OnTextureGenerated, const double DomainMin, const double DomainMax, const FString& SampleLocalVariable, const int32 TextureWidth, const int32 TextureHeight)
{
	if (Code.IsEmpty())
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Empty Code");
		return;
	}

	if (NumSamples < 1)
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Invalid number of samples");
		return;
	}

	if (VariablesToPlot.IsEmpty())
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Empty VariablesToPlot");
		return;
	}

	if (SampleLocalVariable.IsEmpty())
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Empty Code");
	}

	if (TextureWidth < 1 || TextureHeight < 1)
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Invalid texture size");
		return;
	}

	FMathVM MathVM;
	if (!MathVM.TokenizeAndCompile(Code))
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, MathVM.GetError());
		return;
	}

	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	Mip->SizeX = TextureWidth;
	Mip->SizeY = TextureHeight;

#if !WITH_EDITOR
#if !NO_LOGGING
	ELogVerbosity::Type CurrentLogSerializationVerbosity = LogSerialization.GetVerbosity();
	bool bResetLogVerbosity = false;
	if (CurrentLogSerializationVerbosity >= ELogVerbosity::Warning)
	{
		LogSerialization.SetVerbosity(ELogVerbosity::Error);
		bResetLogVerbosity = true;
	}
#endif
#endif

#if !WITH_EDITOR
	// this is a hack for allowing texture streaming without messing around with deriveddata
	Mip->BulkData.SetBulkDataFlags(BULKDATA_PayloadInSeperateFile);
#endif
	Mip->BulkData.Lock(LOCK_READ_WRITE);

#if !WITH_EDITOR
#if !NO_LOGGING
	if (bResetLogVerbosity)
	{
		LogSerialization.SetVerbosity(CurrentLogSerializationVerbosity);
	}
#endif
#endif
	const int32 PixelsSize = TextureWidth * TextureHeight * 4;

	TMap<FString, TArray<FVector2D>> Points;
	for (const TPair<FString, FColor>& Pair : VariablesToPlot)
	{
		TArray<FVector2D>& PointsCoordinates = Points.Add(Pair.Key);
		// it is important to set it to invalid coordinates to avoid artifacts
		PointsCoordinates.AddUninitialized(NumSamples);
		for (int32 CoordIndex = 0; CoordIndex < NumSamples; CoordIndex++)
		{
			PointsCoordinates[CoordIndex] = FVector2D(-1, -1);
		}
	}

	ParallelFor(NumSamples, [&](const int32 SampleIndex)
		{
			const double X = FMath::GetMappedRangeValueUnclamped(FVector2D(0, NumSamples - 1), FVector2D(0, TextureWidth - 1), SampleIndex);

			TMap<FString, double> LocalVariables;
			LocalVariables.Add(SampleLocalVariable, SampleIndex);

			if (MathVM.ExecuteStealth(LocalVariables))
			{
				for (const TPair<FString, FColor>& Pair : VariablesToPlot)
				{
					if (LocalVariables.Contains(Pair.Key))
					{
						const double Y = FMath::GetMappedRangeValueUnclamped(FVector2D(DomainMin, DomainMax), FVector2D(0, TextureHeight - 1), FMath::Clamp(LocalVariables[Pair.Key], DomainMin, DomainMax));
						Points[Pair.Key][SampleIndex] = FVector2D(X, (TextureHeight - 1) - Y);
						UE_LOG(LogTemp, Error, TEXT("[%d] X: %d Y: %d [%f]"), SampleIndex, static_cast<int32>(X), static_cast<int32>(Y), LocalVariables[Pair.Key]);
					}
				}
			}
		});

	uint8* Data = reinterpret_cast<uint8*>(Mip->BulkData.Realloc(PixelsSize));

	FMemory::Memset(Data, 255, PixelsSize);

	for (const TPair<FString, TArray<FVector2D>>& Pair : Points)
	{
		const FColor& Color = VariablesToPlot[Pair.Key];

		ParallelFor(NumSamples - 1, [&](const int32 SampleIndex)
			{
				const FVector2D& Point0 = Pair.Value[SampleIndex];
				const FVector2D& Point1 = Pair.Value[SampleIndex + 1];

				if (Point0.X < 0 || Point1.X < 0 || Point0.Y < 0 || Point1.Y < 0)
				{
					return;
				}

				if (Point0.X >= TextureWidth || Point1.X >= TextureWidth || Point0.Y >= TextureHeight || Point1.Y >= TextureHeight)
				{
					return;
				}

				MathVM::Utils::DrawLine(Point0, Point1, Color, [Data, TextureWidth, TextureHeight](const int32 X, const int32 Y, const FColor Color)
					{
						if (X < 0 || Y < 0)
						{
							return;
						}

						if (X >= TextureWidth || Y >= TextureHeight)
						{
							return;
						}

						const int32 Offset = (Y * TextureWidth + X) * 4;

						const double Alpha = Color.A / 255.0;

						Data[Offset] = (Alpha * (Color.B / 255.0) + (1.0 - Alpha)) * 255;
						Data[Offset + 1] = (Alpha * (Color.G / 255.0) + (1.0 - Alpha)) * 255;
						Data[Offset + 2] = (Alpha * (Color.R / 255.0) + (1.0 - Alpha)) * 255;
						Data[Offset + 3] = 255;
					});
			});
	}

	Mip->BulkData.Unlock();

	UTexture2D* Texture = NewObject<UTexture2D>();

	FTexturePlatformData* PlatformData = new FTexturePlatformData();
	PlatformData->SizeX = TextureWidth;
	PlatformData->SizeY = TextureHeight;
	PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;

	PlatformData->Mips.Add(Mip);

	Texture->SetPlatformData(PlatformData);

	Texture->NeverStream = true;

	Texture->UpdateResource();

	OnTextureGenerated.ExecuteIfBound(Texture, "");
}