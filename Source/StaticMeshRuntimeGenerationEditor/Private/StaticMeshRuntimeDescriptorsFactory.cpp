#include "StaticMeshRuntimeDescriptorsFactory.h"

#include "ContentBrowserDelegates.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "StaticMeshRuntimeDescriptors.h"
#include "Widgets/Layout/SUniformGridPanel.h"

/*------------------------------------------------------------------------------
	Dialog to configure creation properties
------------------------------------------------------------------------------*/
class SStaticMeshRuntimeDescriptorsCreateDialog final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SStaticMeshRuntimeDescriptorsCreateDialog ){}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct( const FArguments& InArgs )
	{
		bOkClicked = false;
		
		ChildSlot
		[
			SNew(SBorder)
			.Visibility(EVisibility::Visible)
			.BorderImage(FEditorStyle::GetBrush("ChildWindow.Background"))
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(10.0f)
					[
						SNew(SBox)
						.Visibility(EVisibility::Visible)
						.WidthOverride(400.0f)
						.HeightOverride(400.0f)
						[
							SNew(SBorder)
							.BorderImage(FEditorStyle::GetBrush("NewAnimBlueprintDialog.AreaBorder"))
							[	
								SNew(SVerticalBox)
								+SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0.0f, 0.0f, 0.0f, 5.0f)
								[
									SNew(STextBlock)
									.Text(INVTEXT("Static Mesh"))
									.TextStyle( FEditorStyle::Get(), "NormalText" )
									.ToolTipText(INVTEXT("Choose the static mesh for which mesh descriptors will be cached, so you can generate a modified copy of it at runtime"))
								]
								+SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									MakeSkeletonPickerArea()
								]
							]
						]
					]
				]

				// Ok/Cancel buttons
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(10.0f)
				[
					SNew(SUniformGridPanel)
					.SlotPadding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
					.MinDesiredSlotWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
					.MinDesiredSlotHeight(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
					+SUniformGridPanel::Slot(0,0)
					[
						SNew(SButton)
						.ToolTipText(INVTEXT("Create the new static mesh runtime descriptors asset"))
						.IsEnabled_Lambda([this]()
						{
							return TargetStaticMesh.IsValid();
						})
						.HAlign(HAlign_Center)
						.ContentPadding( FEditorStyle::GetMargin("StandardDialog.ContentPadding") )
						.OnClicked(this, &SStaticMeshRuntimeDescriptorsCreateDialog::OkClicked)
						.Text(INVTEXT("Create"))
					]
					+SUniformGridPanel::Slot(1,0)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.ContentPadding( FEditorStyle::GetMargin("StandardDialog.ContentPadding") )
						.OnClicked(this, &SStaticMeshRuntimeDescriptorsCreateDialog::CancelClicked)
						.Text(INVTEXT("Cancel"))
					]
				]
			]
		];
	}
	
	/** Sets properties for the supplied AnimBlueprintFactory */
	bool ConfigureProperties(TWeakObjectPtr<UStaticMeshRuntimeDescriptorsFactory> InAnimBlueprintFactory)
	{
		StaticMeshRuntimeDescriptorsFactory = InAnimBlueprintFactory;

		TSharedRef<SWindow> Window = SNew(SWindow)
		.Title( INVTEXT("Create Static Mesh Runtime Descriptors") )
		.ClientSize(FVector2D(500, 500))
		.SizingRule(ESizingRule::Autosized)
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		[
			AsShared()
		];

		PickerWindow = Window;

		GEditor->EditorAddModalWindow(Window);
		StaticMeshRuntimeDescriptorsFactory.Reset();

		return bOkClicked;
	}

private:
	
	/** Creates the widgets for the target skeleton area */
	TSharedRef<SWidget> MakeSkeletonPickerArea()
	{
		const FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

		FAssetPickerConfig AssetPickerConfig;
		AssetPickerConfig.RefreshAssetViewDelegates.Add(&RefreshStaticMeshViewDelegate);
		AssetPickerConfig.Filter.ClassNames.Add(UStaticMesh::StaticClass()->GetFName());
		AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SStaticMeshRuntimeDescriptorsCreateDialog::OnStaticMeshSelected);
		//AssetPickerConfig.OnShouldFilterAsset = FOnShouldFilterAsset::CreateSP(this, &SStaticMeshRuntimeDescriptorsCreateDialog::FilterSkeletonBasedOnParentClass);
		AssetPickerConfig.bAllowNullSelection = false;
		AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
		AssetPickerConfig.InitialAssetSelection = TargetStaticMesh;
		AssetPickerConfig.HiddenColumnNames =
		{
			"DiskSize",
			"AdditionalPreviewSkeletalMeshes",
			"PreviewSkeletalMesh"
		};
		AssetPickerConfig.bShowPathInColumnView = false;
		AssetPickerConfig.bShowTypeInColumnView = false;
		AssetPickerConfig.bFocusSearchBoxWhenOpened = false;
	
		return SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(5.0f)
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			];
	}

	/** Handler for when a skeleton is selected */
	void OnStaticMeshSelected(const FAssetData& AssetData)
	{
		TargetStaticMesh = AssetData;
	}

	/** Handler for when ok is clicked */
	FReply OkClicked()
	{
		if ( StaticMeshRuntimeDescriptorsFactory.IsValid() )
		{
			StaticMeshRuntimeDescriptorsFactory->OriginalMesh = Cast<UStaticMesh>(TargetStaticMesh.GetAsset());
		}

		CloseDialog(true);

		return FReply::Handled();
	}

	void CloseDialog(bool bWasPicked=false)
	{
		bOkClicked = bWasPicked;
		if ( PickerWindow.IsValid() )
		{
			PickerWindow.Pin()->RequestDestroyWindow();
		}
	}

	/** Handler for when cancel is clicked */
	FReply CancelClicked()
	{
		CloseDialog();
		return FReply::Handled();
	}

	virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override
	{
		if (InKeyEvent.GetKey() == EKeys::Escape)
		{
			CloseDialog();
			return FReply::Handled();
		}
		return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
	}

private:
	/** The factory for which we are setting up properties */
	TWeakObjectPtr<UStaticMeshRuntimeDescriptorsFactory> StaticMeshRuntimeDescriptorsFactory;

	/** The container for the target skeleton picker*/
	TSharedPtr<SVerticalBox> StaticMeshContainer;

	/** A pointer to the window that is asking the user to select static mesh */
	TWeakPtr<SWindow> PickerWindow;

	/** The selected skeleton */
	FAssetData TargetStaticMesh;

	/** Delegate called to refresh the skeleton view */
	FRefreshAssetViewDelegate RefreshStaticMeshViewDelegate;
	
	/** True if Ok was clicked */
	bool bOkClicked;
};

UStaticMeshRuntimeDescriptorsFactory::UStaticMeshRuntimeDescriptorsFactory()
{
	SupportedClass = UStaticMeshRuntimeDescriptors::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UStaticMeshRuntimeDescriptorsFactory::ConfigureProperties()
{
	const TSharedRef<SStaticMeshRuntimeDescriptorsCreateDialog> Dialog = SNew(SStaticMeshRuntimeDescriptorsCreateDialog);
	return Dialog->ConfigureProperties(this);
}

UObject* UStaticMeshRuntimeDescriptorsFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class == SupportedClass);
	UStaticMeshRuntimeDescriptors* Asset = NewObject<UStaticMeshRuntimeDescriptors>(InParent, Name, Flags);
	Asset->OriginalMesh = OriginalMesh;
	Asset->RefreshDescriptors();
	return Asset;
}

FString UStaticMeshRuntimeDescriptorsFactory::GetDefaultNewAssetName() const
{
	if (OriginalMesh)
	{
		FString StaticMeshName = OriginalMesh->GetName();
		StaticMeshName.RemoveFromStart(TEXT("SM_"));
		return MakeUniqueObjectName(nullptr, UStaticMeshRuntimeDescriptors::StaticClass(), FName(FString::Printf(TEXT("SMRD_%s"), *StaticMeshName))).ToString();
	}
	return Super::GetDefaultNewAssetName();
}
