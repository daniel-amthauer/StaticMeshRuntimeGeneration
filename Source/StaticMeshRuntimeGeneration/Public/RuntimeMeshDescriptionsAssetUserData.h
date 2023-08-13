// Copyright Daniel Amthauer. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeshDescription.h"

#include "RuntimeMeshDescriptionsAssetUserData.generated.h"

/**
 * An asset user data that can cache editor-only data for a static mesh so that it can be used at runtime. 
 * This can be used to generate a modified static mesh at runtime based on the original without paying for the extra
 * rendering cost and limitations of alternatives like UProceduralMeshComponent or UDynamicMesh.
 
 * The intended use case is tile-based procedural games with an irregular grid.
 */
UCLASS(Within=StaticMesh)
class STATICMESHRUNTIMEGENERATION_API URuntimeMeshDescriptionsAssetUserData : public UAssetUserData
{
	GENERATED_BODY()
public:
	
	TArray<FMeshDescription> MeshDescriptions;

	virtual void Serialize(FArchive& Ar) override;

	static URuntimeMeshDescriptionsAssetUserData* GetFrom(const UStaticMesh* StaticMesh);

	/**
	 * Creates a static mesh at runtime based on the descriptors stored in a cached asset, after applying a transform to the vertex positions
	 * @param OriginalMesh The original static mesh. Needs to have the URuntimeMeshDescriptionsAssetUserData for this to work
	 * @param VertexTransform A function that can read and transform the vertex positions of the mesh description. Called once for each mesh description (one for each LOD index)
	 */
	static UStaticMesh* CreateTransformedStaticMesh(const UStaticMesh* OriginalMesh, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform, UObject*
	                                                          Outer = GetTransientPackage());

	/**
	 * Creates a static mesh at runtime based on an array of mesh descriptions
	 * @param Descriptions Array containing descriptions for each LOD
	 */
	static UStaticMesh* CreateRuntimeStaticMeshFromDescriptions(TArray<FMeshDescription> const& Descriptions, TArray<FStaticMaterial> const& StaticMaterials, UObject* Outer = GetTransientPackage());

	/**
	 * Creates a transformed copy at runtime based on the descriptors stored in a cached asset
	 * @param Descriptor The asset containing the cached mesh descriptions
	 * @param VertexTransform A function that can read and transform the vertex positions of the mesh description. Called once for each mesh description (one for each LOD index)
	 */
	static TArray<FMeshDescription> CreateTransformedMeshDescriptions(const URuntimeMeshDescriptionsAssetUserData* DescriptionsUserData, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform);

#if WITH_EDITOR
	virtual void PostEditChangeOwner() override;
#endif
};
