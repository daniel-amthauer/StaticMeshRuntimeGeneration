// Copyright Daniel Amthauer. All Rights Reserved
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "StaticMeshRuntimeDescriptorFactory.generated.h"

UCLASS(hidecategories=Object, MinimalAPI)
class UStaticMeshRuntimeDescriptorFactory : public UFactory
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	UStaticMesh* OriginalMesh;
	
	UStaticMeshRuntimeDescriptorFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	virtual FString GetDefaultNewAssetName() const override;
	//~ Begin UFactory Interface	
};



