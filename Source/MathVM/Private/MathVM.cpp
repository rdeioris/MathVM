// Copyright 2024, Roberto De Ioris.

#include "MathVM.h"

#define LOCTEXT_NAMESPACE "FMathVMModule"



void FMathVMModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMathVMModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMathVMModule, MathVM)