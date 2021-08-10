#include "TestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "UnrealEd.h"
#include "UObject/Package.h"
#include "AssetRegistryModule.h"
#include "TestPrimaryDataAsset.h"

void ATestGameMode::StartPlay()
{
	Super::StartPlay();

	DataAsset->Items.Add(TEXT("Test"));

	FString OutClassName;
	FString OutPackageName;
	FString OutObjectName;
	FString OutSubObjectName;
	FPackageName::SplitFullObjectPath(DataAsset->GetFullName(), OutClassName, OutPackageName, OutObjectName, OutSubObjectName);

	UE_LOG(LogTemp, Error, TEXT("OutClassName : %s  OutPackageName : %s  OutObjectName : %s  OutSubObjectName : %s"),
		*OutClassName,
		*OutPackageName,
		*OutObjectName,
		*OutSubObjectName);

	FString PackageName = OutPackageName;
	UPackage* Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(DataAsset);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(
		PackageName, FPackageName::GetAssetPackageExtension());

	bool bSaved = UPackage::SavePackage(
		Package,
		DataAsset,
		EObjectFlags::RF_Public | EObjectFlags::RF_Standalone,
		*PackageFileName,
		GError, nullptr, true, true, SAVE_NoError);


	// 에셋의 수정 내용을 에디터에도 적용시켜준다.
	TArray<UObject*> ObjectsToSync;
	ObjectsToSync.Add(DataAsset);
	GEditor->SyncBrowserToObjects(ObjectsToSync);
}