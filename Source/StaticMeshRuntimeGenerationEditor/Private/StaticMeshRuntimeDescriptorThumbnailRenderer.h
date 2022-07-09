// Copyright Daniel Amthauer. All Rights Reserved
#pragma once

#include "StaticMeshRuntimeDescriptor.h"
#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"

#include "StaticMeshRuntimeDescriptorThumbnailRenderer.generated.h"

UCLASS()
class UStaticMeshRuntimeDescriptorThumbnailRenderer : public UStaticMeshThumbnailRenderer
{
	GENERATED_BODY()
public:
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily) override
	{
		if (auto SMRD = Cast<UStaticMeshRuntimeDescriptor>(Object))
		{
			Super::Draw(SMRD->OriginalMesh, X, Y, Width, Height, Viewport, Canvas, bAdditionalViewFamily);
		}
	}
};
