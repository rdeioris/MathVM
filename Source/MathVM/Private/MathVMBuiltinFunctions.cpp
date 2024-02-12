#include "MathVMBuiltinFunctions.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		bool Abs(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Abs(Args[0]));
		}

		bool ACos(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Acos(Args[0]));
		}

		bool All(MATHVM_ARGS)
		{
			for (const double Value : Args)
			{
				if (Value == 0.0)
				{
					return CallContext.PushResult(0);
				}
			}
			return CallContext.PushResult(1);
		}

		bool Any(MATHVM_ARGS)
		{
			for (const double Value : Args)
			{
				if (Value != 0.0)
				{
					return CallContext.PushResult(1);
				}
			}
			return CallContext.PushResult(0);
		}

		bool ASin(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Asin(Args[0]));
		}

		bool ATan(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Atan(Args[0]));
		}

		bool Ceil(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::CeilToDouble(Args[0]));
		}

		bool Clamp(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(Args[0], Args[1], Args[2]));
		}

		bool Cos(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Cos(Args[0]));
		}

		bool Degrees(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::RadiansToDegrees(Args[0]));
		}

		bool Distance(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				return CallContext.SetError("distance expects at least 2 arguments");
			}

			if (Args.Num() % 2 != 0)
			{
				return CallContext.SetError("distance number of arguments must be even");
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

			return CallContext.PushResult(FMath::Sqrt(TotalLength));
		}

		bool Dot(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				return CallContext.SetError("dot expects at least 2 arguments");
			}

			if (Args.Num() % 2 != 0)
			{
				return CallContext.SetError("dot number of arguments must be even");
			}

			const int32 NumElements = Args.Num() / 2;

			float Total = 0;

			for (int32 ElementIndex = 0; ElementIndex < NumElements; ElementIndex++)
			{
				Total += Args[ElementIndex] * Args[NumElements + ElementIndex];
			}

			return CallContext.PushResult(Total);
		}

		bool Equal(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				return CallContext.SetError("equal expects at least 2 arguments");
			}

			for (int32 ArgIndex = 1; ArgIndex < Args.Num(); ArgIndex++)
			{
				if (Args[ArgIndex] != Args[0])
				{
					return CallContext.PushResult(0);
				}
			}

			return CallContext.PushResult(1);
		}

		bool Exp(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Exp(Args[0]));
		}

		bool Exp2(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Exp2(Args[0]));
		}

		bool Gradient(MATHVM_ARGS)
		{
			if (Args.Num() < 5)
			{
				return CallContext.SetError("gradient expects at least 5 arguments");
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

			return CallContext.PushResult(FMath::Lerp(Ranges[FoundRangeIndex].Value, Ranges[FoundRangeIndex + 1].Value, Fraction));
		}

		bool Floor(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Floor(Args[0]));
		}

		bool Fract(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Fractional(Args[0]));
		}

		bool GreaterThan(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[0] > Args[1] ? 1 : 0);
		}

		bool GreaterEqual(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[0] >= Args[1] ? 1 : 0);
		}

		bool Hue2B(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 4), 0, 1));
		}

		bool Hue2G(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 2), 0, 1));
		}

		bool Hue2R(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(FMath::Abs(Args[0] * 6 - 3) - 1, 0, 1));
		}

		bool Length(MATHVM_ARGS)
		{
			double TotalLength = 0;
			for (const double Arg : Args)
			{
				TotalLength += Arg;
			}

			return CallContext.PushResult(FMath::Sqrt(TotalLength));
		}

		bool Lerp(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Lerp(Args[0], Args[1], Args[2]));
		}

		bool LessThan(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[0] < Args[1] ? 1 : 0);
		}

		bool LessEqual(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[0] <= Args[1] ? 1 : 0);
		}

		bool Log(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Loge(Args[0]));
		}

		bool Log2(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Log2(Args[0]));
		}

		bool Map(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::GetMappedRangeValueUnclamped(FVector2D(Args[1], Args[2]), FVector2D(Args[3], Args[4]), Args[0]));
		}

		bool Max(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				return CallContext.SetError("max expects at least 2 arguments");
			};

			double MaxValue = FMath::Max(Args[0], Args[1]);
			for (int32 ArgIndex = 2; ArgIndex < Args.Num(); ArgIndex++)
			{
				MaxValue = FMath::Max(MaxValue, Args[ArgIndex]);
			}

			return CallContext.PushResult(MaxValue);
		}

		bool Min(MATHVM_ARGS)
		{
			if (Args.Num() < 2)
			{
				return CallContext.SetError("min expects at least 2 arguments");
			};

			double MinValue = FMath::Min(Args[0], Args[1]);
			for (int32 ArgIndex = 2; ArgIndex < Args.Num(); ArgIndex++)
			{
				MinValue = FMath::Min(MinValue, Args[ArgIndex]);
			}

			return CallContext.PushResult(MinValue);
		}

		bool Mod(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Fmod(Args[0], Args[1]));
		}

		bool Not(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[0] == 0.0 ? 1 : 0);
		}

		bool Pow(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Pow(Args[0], Args[1]));
		}

		bool Radians(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::DegreesToRadians(Args[0]));
		}

		bool Rand(MATHVM_ARGS)
		{
			static FRandomStream RandomStream;
			return CallContext.PushResult(RandomStream.FRandRange(Args[0], Args[1]));
		}

		bool Round(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::RoundToDouble(Args[0]));
		}

		bool RoundEven(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::RoundHalfToEven(Args[0]));
		}

		bool Sign(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Sign(Args[0]));
		}

		bool Sin(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Sin(Args[0]));
		}

		bool Sqrt(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Sqrt(Args[0]));
		}

		bool Step(MATHVM_ARGS)
		{
			return CallContext.PushResult(Args[1] < Args[0] ? 0 : 1);
		}

		bool Tan(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Tan(Args[0]));
		}

		bool Trunc(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::TruncToDouble(Args[0]));
		}

		// Resources

		bool Read(MATHVM_ARGS)
		{
			if (Args.Num() < 1)
			{
				return CallContext.SetError("read expects at least 1 argument");
			};

			TArray<double> InterfaceArgs;
			for (int32 Index = 1; Index < Args.Num(); Index++)
			{
				InterfaceArgs.Add(Args[Index]);
			}

			return CallContext.PushResult(CallContext.ReadResource(static_cast<int32>(Args[0]), InterfaceArgs));
		}

		bool Write(MATHVM_ARGS)
		{
			if (Args.Num() < 1)
			{
				return CallContext.SetError("write expects at least 1 argument");
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