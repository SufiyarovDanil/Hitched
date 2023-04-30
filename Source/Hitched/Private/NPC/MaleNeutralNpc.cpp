/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/MaleNeutralNpc.h"


AMaleNeutralNpc::AMaleNeutralNpc()
{
    // Init manequin
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleMeshAsset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/MaleManequin.MaleManequin'"));

    if (MaleMeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MaleMeshAsset.Object);
    }

    // Init hairs
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleHair1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Hairs/MaleHair1.MaleHair1'"));

    if (MaleHair1Asset.Succeeded())
    {
        Hairs.Add(MaleHair1Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleHair2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Hairs/MaleHair2.MaleHair2'"));
    
    if (MaleHair2Asset.Succeeded())
    {
        Hairs.Add(MaleHair2Asset.Object);
    }

    // Init top clothes
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Top/Jacket1.Jacket1'"));
    
    if (Jacket1Asset.Succeeded())
    {
        TopClothes.Add(Jacket1Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Top/Jacket2.Jacket2'"));
    
    if (Jacket2Asset.Succeeded())
    {
        TopClothes.Add(Jacket2Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Shirt1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Top/Shirt1.Shirt1'"));
    
    if (Shirt1Asset.Succeeded())
    {
        TopClothes.Add(Shirt1Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Shirt2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Top/Shirt2.Shirt2'"));
    
    if (Shirt2Asset.Succeeded())
    {
        TopClothes.Add(Shirt2Asset.Object);
    }

    // Init bottom clothes
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants1Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Bottom/Pants1.Pants1'"));
    
    if (Pants1Asset.Succeeded())
    {
        BottomClothes.Add(Pants1Asset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants2Asset(
        TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Bottom/Pants2.Pants2'"));
    
    if (Pants2Asset.Succeeded())
    {
        BottomClothes.Add(Pants2Asset.Object);
    }
}
