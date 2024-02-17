#include "MathVMBuiltinFunctions.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		bool Abs(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Abs(Args[0]));
		}

		bool ACos(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Acos(Args[0]));
		}

		bool All(MATHVM_ARGS)
		{
			for (const double Value : Args)
			{
				if (Value == 0.0)
				{
					MATHVM_RETURN(0);
				}
			}
			MATHVM_RETURN(1);
		}

		bool Any(MATHVM_ARGS)
		{
			for (const double Value : Args)
			{
				if (Value != 0.0)
				{
					MATHVM_RETURN(1);
				}
			}
			MATHVM_RETURN(0);
		}

		bool ASin(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Asin(Args[0]));
		}

		bool ATan(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Atan(Args[0]));
		}

		bool Ceil(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::CeilToDouble(Args[0]));
		}

		bool Clamp(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Clamp(Args[0], Args[1], Args[2]));
		}

		bool Cos(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Cos(Args[0]));
		}

		bool Degrees(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::RadiansToDegrees(Args[0]));
		}

		bool Distance(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				MATHVM_ERROR("distance expects at least 2 arguments");
			}

			if (Args.Num() % 2 != 0)
			{
				MATHVM_ERROR("distance number of arguments must be even");
			}

			const int32 NumElements = Args.Num() / 2;

			TArray<double> Values;
			Values.AddUninitialized(NumElements);

			for (int32 ElementIndex = 0; ElementIndex < NumElements; ElementIndex++)
			{
				Values[ElementIndex] = Args[NumElements + ElementIndex] - Args[ElementIndex];
			}

			double TotalLength = 0;

			for (int32 ElementIndex = 0; ElementIndex < NumElements; ElementIndex++)
			{
				TotalLength += Values[ElementIndex] * Values[ElementIndex];
			}

			MATHVM_RETURN(FMath::Sqrt(TotalLength));
		}

		bool Dot(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				MATHVM_ERROR("dot expects at least 2 arguments");
			}

			if (Args.Num() % 2 != 0)
			{
				MATHVM_ERROR("dot number of arguments must be even");
			}

			const int32 NumElements = Args.Num() / 2;

			float Total = 0;

			for (int32 ElementIndex = 0; ElementIndex < NumElements; ElementIndex++)
			{
				Total += Args[ElementIndex] * Args[NumElements + ElementIndex];
			}

			MATHVM_RETURN(Total);
		}

		bool Equal(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				MATHVM_ERROR("equal expects at least 2 arguments");
			}

			for (int32 ArgIndex = 1; ArgIndex < Args.Num(); ArgIndex++)
			{
				if (Args[ArgIndex] != Args[0])
				{
					MATHVM_RETURN(0);
				}
			}

			MATHVM_RETURN(1);
		}

		bool Exp(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Exp(Args[0]));
		}

		bool Exp2(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Exp2(Args[0]));
		}

		bool Gradient(MATHVM_ARGS)
		{
			if (Args.Num() < 5)
			{
				MATHVM_ERROR("gradient expects at least 5 arguments");
			}

			if ((Args.Num() - 1) % 2 != 0)
			{
				return false;
			}

			if (Args[3] < Args[1])
			{
				return false;
			}

			const double Value = Args[0];
			TArray<TPair<double, double>> Ranges;

			Ranges.Add({ Args[1], Args[2] });
			Ranges.Add({ Args[3], Args[4] });

			for (int32 ArgIndex = 5; ArgIndex < Args.Num(); ArgIndex += 2)
			{
				if (Args[ArgIndex] < Args[ArgIndex - 2])
				{
					return false;
				}
				Ranges.Add({ Args[ArgIndex], Args[ArgIndex + 1] });
			}

			int32 FoundRangeIndex = -1;

			for (int32 RangeIndex = 0; RangeIndex < Ranges.Num(); RangeIndex++)
			{
				if (Ranges[RangeIndex].Key >= Value)
				{
					if (RangeIndex == 0)
					{
						FoundRangeIndex = 0;
					}
					else
					{
						FoundRangeIndex = RangeIndex - 1;
					}
					break;
				}
			}

			if (FoundRangeIndex < 0)
			{
				FoundRangeIndex = Ranges.Num() - 1;
			}

			const double Delta = Value - Ranges[FoundRangeIndex].Key;
			const double Fraction = Delta / (Ranges[FoundRangeIndex + 1].Key - Ranges[FoundRangeIndex].Key);

			MATHVM_RETURN(FMath::Lerp(Ranges[FoundRangeIndex].Value, Ranges[FoundRangeIndex + 1].Value, Fraction));
		}

		bool Floor(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Floor(Args[0]));
		}

		bool Fract(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Fractional(Args[0]));
		}

		bool GreaterThan(MATHVM_ARGS)
		{
			MATHVM_RETURN(Args[0] > Args[1] ? 1 : 0);
		}

		bool GreaterEqual(MATHVM_ARGS)
		{
			MATHVM_RETURN(Args[0] >= Args[1] ? 1 : 0);
		}

		bool Hue2B(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 4), 0, 1));
		}

		bool Hue2G(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 2), 0, 1));
		}

		bool Hue2R(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Clamp(FMath::Abs(Args[0] * 6 - 3) - 1, 0, 1));
		}

		bool Length(MATHVM_ARGS)
		{
			double TotalLength = 0;
			for (const double Arg : Args)
			{
				TotalLength += Arg * Arg;
			}

			MATHVM_RETURN(FMath::Sqrt(TotalLength));
		}

		bool Lerp(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Lerp(Args[0], Args[1], Args[2]));
		}

		bool LessThan(MATHVM_ARGS)
		{
			MATHVM_RETURN(Args[0] < Args[1] ? 1 : 0);
		}

		bool LessEqual(MATHVM_ARGS)
		{
			MATHVM_RETURN(Args[0] <= Args[1] ? 1 : 0);
		}

		bool Log(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Loge(Args[0]));
		}

		bool Log2(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Log2(Args[0]));
		}

		bool Map(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::GetMappedRangeValueUnclamped(FVector2D(Args[1], Args[2]), FVector2D(Args[3], Args[4]), Args[0]));
		}

		bool Max(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				MATHVM_ERROR("max expects at least 2 arguments");
			};

			double MaxValue = FMath::Max(Args[0], Args[1]);
			for (int32 ArgIndex = 2; ArgIndex < Args.Num(); ArgIndex++)
			{
				MaxValue = FMath::Max(MaxValue, Args[ArgIndex]);
			}

			MATHVM_RETURN(MaxValue);
		}

		bool Min(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				MATHVM_ERROR("min expects at least 2 arguments");
			};

			double MinValue = FMath::Min(Args[0], Args[1]);
			for (int32 ArgIndex = 2; ArgIndex < Args.Num(); ArgIndex++)
			{
				MinValue = FMath::Min(MinValue, Args[ArgIndex]);
			}

			MATHVM_RETURN(MinValue);
		}

		bool Mod(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Fmod(Args[0], Args[1]));
		}

		bool Not(MATHVM_ARGS)
		{
			MATHVM_RETURN(Args[0] == 0.0 ? 1 : 0);
		}

		bool Pow(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Pow(Args[0], Args[1]));
		}

		bool Radians(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::DegreesToRadians(Args[0]));
		}

		bool Rand(MATHVM_ARGS)
		{
			static FRandomStream RandomStream;
			MATHVM_RETURN(RandomStream.FRandRange(Args[0], Args[1]));
		}

		bool Round(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::RoundToDouble(Args[0]));
		}

		bool RoundEven(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::RoundHalfToEven(Args[0]));
		}

		bool Sign(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Sign(Args[0]));
		}

		bool Sin(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Sin(Args[0]));
		}

		bool Sqrt(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Sqrt(Args[0]));
		}

		bool Tan(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::Tan(Args[0]));
		}

		bool Trunc(MATHVM_ARGS)
		{
			MATHVM_RETURN(FMath::TruncToDouble(Args[0]));
		}

		// Resources

		bool Read(MATHVM_ARGS)
		{
			if (Args.Num() < 1)
			{
				MATHVM_ERROR("read expects at least 1 argument");
			};

			TArray<double> InterfaceArgs;
			for (int32 Index = 1; Index < Args.Num(); Index++)
			{
				InterfaceArgs.Add(Args[Index]);
			}

			MATHVM_RETURN(CallContext.ReadResource(static_cast<int32>(Args[0]), InterfaceArgs));
		}

		bool Write(MATHVM_ARGS)
		{
			if (Args.Num() < 1)
			{
				MATHVM_ERROR("write expects at least 1 argument");
			};

			TArray<double> InterfaceArgs;
			for (int32 Index = 1; Index < Args.Num(); Index++)
			{
				InterfaceArgs.Add(Args[Index]);
			}

			CallContext.WriteResource(static_cast<int32>(Args[0]), InterfaceArgs);
			return true;
		}
	}
}