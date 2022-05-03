#include "StaticMeshRuntimeDescriptorsFactory.h"

#include "StaticMeshRuntimeDescriptors.h"

UStaticMeshRuntimeDescriptorsFactory::UStaticMeshRuntimeDescriptorsFactory()
{
	SupportedClass = UStaticMeshRuntimeDescriptors::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UStaticMeshRuntimeDescriptorsFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class == SupportedClass);
	UStaticMeshRuntimeDescriptors* Asset = NewObject<UStaticMeshRuntimeDescriptors>(InParent, Name, Flags);

	return Asset;
}
