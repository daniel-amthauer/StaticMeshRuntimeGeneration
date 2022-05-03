#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "StaticMeshRuntimeDescriptorsFactory.generated.h"

UCLASS(hidecategories=Object, MinimalAPI)
class UStaticMeshRuntimeDescriptorsFactory : public UFactory
{
	GENERATED_BODY()

public:
	UStaticMeshRuntimeDescriptorsFactory();

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	
};



