#include "MathVMBuiltinFunctions.h"

namespace MathVM
{
	namespace BuiltinFunctions
	{
		bool Sin(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Sin(Args[0]));
		}


		bool Cos(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Cos(Args[0]));
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

		bool Map(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::GetMappedRangeValueClamped(TRange<double>(Args[1], Args[2]), TRange<double>(Args[3], Args[4]), Args[0]));
		}

		bool Rand(MATHVM_ARGS)
		{
			static FRandomStream RandomStream;
			return CallContext.PushResult(RandomStream.FRandRange(Args[0], Args[1]));
		}

		bool Pow(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Pow(Args[0], Args[1]));
		}

		bool Hue2R(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(FMath::Abs(Args[0] * 6 - 3) - 1, 0, 1));
		}

		bool Hue2G(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 2), 0, 1));
		}


		bool Hue2B(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Clamp(2 - FMath::Abs(Args[0] * 6 - 4), 0, 1));
		}

		bool Lerp(MATHVM_ARGS)
		{
			return CallContext.PushResult(FMath::Lerp(Args[0], Args[1], Args[2]));
		}

		bool Gradient(MATHVM_ARGS)
		{
			if (Args.Num() < 5)
			{
				return CallContext.SetError("gradient expects at least 5 arguments");
			}

			if (Args.Num() - 1 % 2 != 0)
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