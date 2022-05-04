// Copyright Daniel Amthauer. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeshDescription.h"
#include "StaticMeshRuntimeDescriptors.generated.h"

/**
 * 
 */
UCLASS()
class STATICMESHRUNTIMEGENERATION_API UStaticMeshRuntimeDescriptors : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	UStaticMesh* OriginalMesh;
	
	TArray<FMeshDescription> MeshDescriptions;

	virtual void Serialize(FArchive& Ar) override;

	UFUNCTION(BlueprintCallable)
	static UStaticMesh* CreateRuntimeStaticMeshFromDescriptors(UStaticMeshRuntimeDescriptors* Descriptors);

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void RefreshDescriptors();
#endif
};
