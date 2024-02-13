// Copyright 2024, Roberto De Ioris.


#include "MathVMBlueprintFunctionLibrary.h"
#include "Engine/Canvas.h"


bool MathVM::BlueprintUtility::RegisterResources(FMathVM& MathVM, const TArray<UMathVMResourceObject*>& Resources, FString& Error)
{
	for (int32 ResourceIndex = 0; ResourceIndex < Resources.Num(); ResourceIndex++)
	{
		UMathVMResourceObject* Resource = Resources[ResourceIndex];
		if (!Resource)
		{
			Error = FString::Printf(TEXT("Null resource at index %d"), ResourceIndex);
			return false;
		}

		TSharedPtr<IMathVMResource> NewResource = Resource->GetMathVMResource();
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

	return true;
}

UMathVMResourceObject* UMathVMBlueprintFunctionLibrary::MathVMResourceObjectFromTexture2D(UTexture2D* Texture)
{
	if (!Texture)
	{
		return nullptr;
	}

	UMathVMResourceObject* NewResourceObject = NewObject<UMathVMResourceObject>();

	NewResourceObject->SetMathVMResource(MakeShared<FMathVMTexture2DResource>(Texture));

	return NewResourceObject;
}

UMathVMResourceObject* UMathVMBlueprintFunctionLibrary::MathVMResourceObjectFromCurveBase(UCurveBase* Curve)
{
	if (!Curve)
	{
		return nullptr;
	}

	UMathVMResourceObject* NewResourceObject = NewObject<UMathVMResourceObject>();

	NewResourceObject->SetMathVMResource(MakeShared<FMathVMCurveBaseResource>(Curve));

	return NewResourceObject;
}

bool UMathVMBlueprintFunctionLibrary::MathVMRunSimple(const FString& Code, TMap<FString, double>& LocalVariables, const TArray<UMathVMResourceObject*>& Resources, double& Result, FString& Error)
{
	if (Code.IsEmpty())
	{
		Error = "Empty Code";
		return false;
	}

	FMathVM MathVM;

	if (!MathVM::BlueprintUtility::RegisterResources(MathVM, Resources, Error))
	{
		return false;
	}

	if (!MathVM.TokenizeAndCompile(Code))
	{
		Error = MathVM.GetError();
		return false;
	}

	return MathVM.ExecuteOne(LocalVariables, Result, Error);
}

void UMathVMBlueprintFunctionLibrary::MathVMPlotter(UObject* WorldContextObject, const FString& Code, const int32 NumSamples, const TMap<FString, FMathVMPlot>& VariablesToPlot, const TMap<FString, double>& Constants, TMap<FString, double>& GlobalVariables, const TArray<UMathVMResourceObject*>& Resources, const FMathVMTextureGenerated& OnTextureGenerated, const FMathVMPlotterConfig& PlotterConfig, const double DomainMin, const double DomainMax, const FString& SampleLocalVariable)
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

	if (PlotterConfig.TextureWidth < 1 || PlotterConfig.TextureHeight < 1)
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Invalid texture size");
		return;
	}

	FMathVM MathVM;

	FString RegisterResourcesError;
	if (!MathVM::BlueprintUtility::RegisterResources(MathVM, Resources, RegisterResourcesError))
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, RegisterResourcesError);
		return;
	}

	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
	if (!RenderTarget)
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, "Unable to create CanvasRenderTarget2D");
		return;
	}

	RenderTarget->InitAutoFormat(PlotterConfig.TextureWidth, PlotterConfig.TextureHeight);

	if (!MathVM.TokenizeAndCompile(Code))
	{
		OnTextureGenerated.ExecuteIfBound(nullptr, MathVM.GetError());
		return;
	}

	TMap<FString, TArray<FVector2D>> Points;
	for (const TPair<FString, FMathVMPlot>& Pair : VariablesToPlot)
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
			const double X = FMath::GetMappedRangeValueUnclamped(FVector2D(0, NumSamples - 1), FVector2D(PlotterConfig.BorderSize.Left + PlotterConfig.BorderThickness, PlotterConfig.TextureWidth - 1 - PlotterConfig.BorderSize.Right - PlotterConfig.BorderThickness), SampleIndex);

			TMap<FString, double> LocalVariables;
			LocalVariables.Add(SampleLocalVariable, SampleIndex);

			if (MathVM.ExecuteStealth(LocalVariables))
			{
				for (const TPair<FString, FMathVMPlot>& Pair : VariablesToPlot)
				{
					if (LocalVariables.Contains(Pair.Key))
					{
						const double Y = FMath::GetMappedRangeValueUnclamped(FVector2D(DomainMin, DomainMax), FVector2D(PlotterConfig.BorderSize.Bottom + PlotterConfig.BorderThickness, PlotterConfig.TextureHeight - 1 - PlotterConfig.BorderSize.Top - PlotterConfig.BorderThickness), FMath::Clamp(LocalVariables[Pair.Key], DomainMin, DomainMax));
						Points[Pair.Key][SampleIndex] = FVector2D(X, (PlotterConfig.TextureHeight - 1 - PlotterConfig.BorderSize.Top - PlotterConfig.BorderThickness) - Y + PlotterConfig.BorderSize.Bottom + PlotterConfig.BorderThickness);
					}
				}
			}
		});

	UCanvas* Canvas = nullptr;
	FVector2D CanvasSize;
	FDrawToRenderTargetContext DrawContext;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(WorldContextObject, RenderTarget, Canvas, CanvasSize, DrawContext);

	if (Canvas)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(WorldContextObject, RenderTarget, PlotterConfig.BackgroundColor);

		Canvas->K2_DrawLine(PlotterConfig.BorderSize.GetTopLeft(), FVector2D(PlotterConfig.TextureWidth - PlotterConfig.BorderSize.Right, PlotterConfig.BorderSize.Top), PlotterConfig.BorderThickness, PlotterConfig.BorderColor);
		Canvas->K2_DrawLine(PlotterConfig.BorderSize.GetTopLeft(), FVector2D(PlotterConfig.BorderSize.Left, PlotterConfig.TextureHeight - PlotterConfig.BorderSize.Bottom), PlotterConfig.BorderThickness, PlotterConfig.BorderColor);
		Canvas->K2_DrawLine(FVector2D(PlotterConfig.TextureWidth - PlotterConfig.BorderSize.Right, PlotterConfig.BorderSize.Top), FVector2D(PlotterConfig.TextureWidth - PlotterConfig.BorderSize.Right, PlotterConfig.TextureHeight - PlotterConfig.BorderSize.Bottom), PlotterConfig.BorderThickness, PlotterConfig.BorderColor);
		Canvas->K2_DrawLine(FVector2D(PlotterConfig.BorderSize.Left, PlotterConfig.TextureHeight - PlotterConfig.BorderSize.Bottom), FVector2D(PlotterConfig.TextureWidth - PlotterConfig.BorderSize.Right, PlotterConfig.TextureHeight - PlotterConfig.BorderSize.Bottom), PlotterConfig.BorderThickness, PlotterConfig.BorderColor);

		if (PlotterConfig.GridVerticalScale > 0)
		{
			const double Step = ((PlotterConfig.TextureWidth - PlotterConfig.BorderSize.Left - PlotterConfig.BorderSize.Right - (PlotterConfig.BorderThickness * 2)) * PlotterConfig.GridVerticalScale) / (NumSamples - 1);
			const int32 NumScaledSamples = (NumSamples - 1) / PlotterConfig.GridVerticalScale;

			for (int32 SampleIndex = 0; SampleIndex < NumScaledSamples + 1; SampleIndex++)
			{
				Canvas->K2_DrawLine(
					FVector2D(PlotterConfig.BorderSize.Left + PlotterConfig.BorderThickness + (Step * SampleIndex), PlotterConfig.TextureHeight - PlotterConfig.BorderSize.Bottom - PlotterConfig.BorderThickness),
					FVector2D(PlotterConfig.BorderSize.Left + PlotterConfig.BorderThickness + (Step * SampleIndex), PlotterConfig.BorderSize.Top + PlotterConfig.BorderThickness),
					PlotterConfig.GridThickness, PlotterConfig.GridColor);
			}
		}

		for (const TPair<FString, TArray<FVector2D>>& Pair : Points)
		{
			const FMathVMPlot& Plot = VariablesToPlot[Pair.Key];

			if (Plot.Shape == EMathVMPlotterShape::Line || Plot.Shape == EMathVMPlotterShape::LineAndPoint)
			{
				for (int32 PointIndex = 0; PointIndex < Pair.Value.Num() - 1; PointIndex++)
				{
					const FVector2D& Point0 = Pair.Value[PointIndex];
					const FVector2D& Point1 = Pair.Value[PointIndex + 1];
					if (Point0.X < 0 || Point0.Y < 0 || Point1.X < 0 || Point1.Y < 0)
					{
						continue;
					}
					Canvas->K2_DrawLine(Point0, Point1, Plot.Thickness, Plot.Color);
					if (Plot.Shape == EMathVMPlotterShape::LineAndPoint)
					{
						Canvas->K2_DrawBox(Point0 - FVector2D(Plot.Thickness, Plot.Thickness), FVector2D(Plot.Thickness, Plot.Thickness) * 2, Plot.Thickness * 2, Plot.Color2);
					}
				}

				if (Plot.Shape == EMathVMPlotterShape::LineAndPoint)
				{
					const FVector2D& Point0 = Pair.Value[Pair.Value.Num() - 1];
					Canvas->K2_DrawBox(Point0 - FVector2D(Plot.Thickness, Plot.Thickness), FVector2D(Plot.Thickness, Plot.Thickness) * 2, Plot.Thickness * 2, Plot.Color2);
				}
			}
			else if (Plot.Shape == EMathVMPlotterShape::Box)
			{
				for (int32 PointIndex = 0; PointIndex < Pair.Value.Num(); PointIndex++)
				{
					const FVector2D& Point = Pair.Value[PointIndex];
					if (Point.X < 0 || Point.Y < 0)
					{
						continue;
					}
					Canvas->K2_DrawBox(Point - FVector2D(Plot.Thickness, Plot.Thickness) * 0.5, FVector2D(Plot.Thickness, Plot.Thickness), Plot.Thickness, Plot.Color);
				}
			}
		}
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(WorldContextObject, DrawContext);

	OnTextureGenerated.ExecuteIfBound(RenderTarget, "");
}