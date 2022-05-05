// Copyright Daniel Amthauer. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeshDescription.h"
#include "StaticMeshRuntimeDescriptor.generated.h"

/**
 * An asset that can cache editor-only data for a static mesh so that it can be used at runtime. This can be used to generate a static mesh at runtime based on another static mesh
 * (e.g. a deformed version of the original static mesh), without paying for the extra rendering cost and limitations of alternatives like UProceduralMeshComponent or FDynamicMesh3
 */
UCLASS()
class STATICMESHRUNTIMEGENERATION_API UStaticMeshRuntimeDescriptor : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	UStaticMesh* OriginalMesh;
	
	TArray<FMeshDescription> MeshDescriptions;

	virtual void Serialize(FArchive& Ar) override;

	/**
	 * Creates a static mesh at runtime based on the descriptors stored in a cached asset, after applying a transform to the vertex positions
	 * @param Descriptor The asset containing the cached mesh descriptions
	 * @param VertexTransform A function that can read and transform the vertex positions of the mesh description. Called once for each mesh description (one for each LOD index)
	 */
	static UStaticMesh* CreateRuntimeStaticMeshFromDescriptor(UStaticMeshRuntimeDescriptor* Descriptor, TFunction<void(TArrayView<FVector3f>)> VertexTransform);

	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(TArray<FVector>, FBP_VertexTransform, const TArray<FVector>&, VertexPositions);
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Create Runtime Static Mesh From Descriptor"))
	static UStaticMesh* K2_CreateRuntimeStaticMeshFromDescriptor(UStaticMeshRuntimeDescriptor* Descriptor, FBP_VertexTransform VertexTransform);

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void RefreshDescriptors();
#endif
};
