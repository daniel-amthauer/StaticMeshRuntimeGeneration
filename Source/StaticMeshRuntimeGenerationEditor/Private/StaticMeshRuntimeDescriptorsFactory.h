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
	UPROPERTY(Transient)
	UStaticMesh* OriginalMesh;
	
	UStaticMeshRuntimeDescriptorsFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	virtual FString GetDefaultNewAssetName() const override;
	//~ Begin UFactory Interface	
};



