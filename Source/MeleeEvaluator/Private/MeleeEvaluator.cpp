// Copyright (c) 2019 Drowning Dragons Limited. All Rights Reserved.

#include "MeleeEvaluator.h"

#define LOCTEXT_NAMESPACE "FMeleeEvaluatorModule"

void FMeleeEvaluatorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMeleeEvaluatorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeleeEvaluatorModule, MeleeEvaluator)