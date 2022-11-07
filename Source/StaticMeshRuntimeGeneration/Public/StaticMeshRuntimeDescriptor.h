// Copyright Daniel Amthauer. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeshDescription.h"

#include "Runtime/Launch/Resources/Version.h"

#include "StaticMeshRuntimeDescriptor.generated.h"

#if ENGINE_MAJOR_VERSION < 5
typedef FVector FVector3f;
#endif

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
	static UStaticMesh* CreateRuntimeStaticMeshFromDescriptor(const UStaticMeshRuntimeDescriptor* Descriptor, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform);

	/**
	 * Creates a static mesh at runtime based on an array of mesh descriptions
	 * @param Descriptions Array containing descriptions for each LOD
	 */
	static UStaticMesh* CreateRuntimeStaticMeshFromDescriptions(TArray<FMeshDescription> const& Descriptions);

	/**
	 * Creates a transformed copy at runtime based on the descriptors stored in a cached asset
	 * @param Descriptor The asset containing the cached mesh descriptions
	 * @param VertexTransform A function that can read and transform the vertex positions of the mesh description. Called once for each mesh description (one for each LOD index)
	 */
	static TArray<FMeshDescription> CreateTransformedMeshDescriptors(const UStaticMeshRuntimeDescriptor* Descriptor, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform);

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void RefreshDescriptors();
#endif
};
