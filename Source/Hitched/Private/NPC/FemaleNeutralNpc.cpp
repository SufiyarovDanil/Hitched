/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/FemaleNeutralNpc.h"


AFemaleNeutralNpc::AFemaleNeutralNpc()
{
    // Init manequine mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleMeshAsset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/FemaleManequin.FemaleManequin'"));

    if (FemaleMeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(FemaleMeshAsset.Object);
    }

    // Init Hairs
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleHair1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Hairs/FemaleHair1.FemaleHair1'"));

    if (FemaleHair1Asset.Succeeded())
    {
        Hairs.Add(FemaleHair1Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleHair2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Hairs/FemaleHair2.FemaleHair2'"));
    
    if (FemaleHair2Asset.Succeeded())
    {
        Hairs.Add(FemaleHair2Asset.Object);
    }

    // Init top clothes
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> DressAsset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Top/Dress.Dress'"));
    
    if (DressAsset.Succeeded())
    {
        TopClothes.Add(DressAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShirtAsset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Top/Shirt.Shirt'"));
    
    if (ShirtAsset.Succeeded())
    {
        TopClothes.Add(ShirtAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skirt2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Top/Skirt2.Skirt2'"));
    
    if (Skirt2Asset.Succeeded())
    {
        TopClothes.Add(Skirt2Asset.Object);
    }

    // Init bottom clothes
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesAsset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Bottom/Shoes.Shoes'"));
    
    if (ShoesAsset.Succeeded())
    {
        BottomClothes.Add(ShoesAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skirt1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Female/Meshes/Outfits/Bottom/Skirt1.Skirt1'"));
    
    if (Skirt1Asset.Succeeded())
    {
        BottomClothes.Add(Skirt1Asset.Object);
    }
}
