// Offsets.hpp — FF 32-bit (Steph Wolf Dumper 2026-10-15)
#pragma once
#include <cstdint>

class Offsets {
public:
    // BASE (runtime)
    static inline uintptr_t Il2Cpp      = 0x0;
    static inline uintptr_t UnityCpp    = 0x0;
    static inline uintptr_t InitBase    = 0xA342EFC;
    static inline uintptr_t StaticClass = 0x5C;

    // BỔ SUNG CHO DATA.CPP
    static inline uintptr_t Il2CppDictionaryDataPtr = 0x0; // Cần tìm offset thật
    static inline uintptr_t Il2CppDictionaryCount  = 0x0; // Cần tìm offset thật

    // MATCH
    static inline uintptr_t CurrentMatch       = 0x50;
    static inline uintptr_t MatchStatus        = 0x8C;
    static inline uintptr_t LocalPlayer        = 0x94;
    static inline uintptr_t DictionaryEntities = 0x68;

    // PLAYER
    static inline uintptr_t Player_IsDead     = 0x50;
    static inline uintptr_t Player_Name       = 0x31C;
    static inline uintptr_t OriginalName      = 0x320;
    static inline uintptr_t Player_Data       = 0x48;
    static inline uintptr_t Player_PhysXData  = 0x14E8;
    static inline uintptr_t Player_IsBot      = 0x324;
    static inline uintptr_t IsClientBot       = 0x324;
    static inline uintptr_t IsFrozenKnockDown = 0x6C;
    static inline uintptr_t IsCadet           = 0x2A0;
    static inline uintptr_t IsFrustrum        = 0x7BC;
    static inline uintptr_t LocalPlayerAttributes = 0x500;
    static inline uintptr_t CharacterController   = 0x348;
    static inline uintptr_t IsInVehicle       = 0x364;
    static inline uintptr_t IsSkillActive     = 0x774;
    static inline uintptr_t Speed             = 0x3BC;
    static inline uintptr_t IsFiring          = 0x58C;

    // CAMERA
    static inline uintptr_t AimRotation         = 0x440;
    static inline uintptr_t FollowCamera        = 0x494;
    static inline uintptr_t Camera              = 0x18;
    static inline uintptr_t MainCameraTransform = 0x28C;
    static inline uintptr_t ViewMatrix          = 0xE8;

    // AVATAR
    static inline uintptr_t AvatarManager       = 0x504;
    static inline uintptr_t Avatar              = 0xA8;
    static inline uintptr_t Avatar_IsVisible    = 0x00;
    static inline uintptr_t Avatar_Data         = 0x6C;
    static inline uintptr_t Avatar_Data_IsTeam  = 0x59;
    static inline uintptr_t UMA_IsLocalPlayer   = 0x58;
    static inline uintptr_t UMA_IsCastShadow    = 0x5A;
    static inline uintptr_t UMA_IsHideHead      = 0x5B;
    static inline uintptr_t UMA_Renderer        = 0x10;
    static inline uintptr_t UMA_Animator        = 0xC8;

    // SHADOW / POSE
    static inline uintptr_t ShadowState         = 0x14E8;
    static inline uintptr_t Player_ShadowBase   = 0x14E8;
    static inline uintptr_t XPose               = 0x78;

    // WEAPON
    static inline uintptr_t Weapon                = 0x434;
    static inline uintptr_t WeaponData            = 0x58;
    static inline uintptr_t WeaponDataPtr         = 0x64;
    static inline uintptr_t Weapon_ItemId         = 0x4D4;
    static inline uintptr_t WeaponRecoil          = 0xC;
    static inline uintptr_t Weapon_AddFireSpeed   = 0x4A4;
    static inline uintptr_t WeaponOnHand          = 0x54;
    static inline uintptr_t ActiveUISightingWeapon= 0x434;
    static inline uintptr_t CombineWeaponOnHand   = 0x58;
    static inline uintptr_t IsCombineWeapon       = 0xD0;
    static inline uintptr_t Ammo_CurrentClip      = 0x4D4;
    static inline uintptr_t AmmoOffs              = 0x6A4;
    static inline uintptr_t Lock2X                = 0x500;
    static inline uintptr_t FastSwitch            = 0x568;
    static inline uintptr_t DobleGunOffs1         = 0x4B4;
    static inline uintptr_t DobleGunOffs2         = 0x4D0;

    // ATTRIBUTES
    static inline uintptr_t PlayerAttributes      = 0x500;
    static inline uintptr_t FireInterval          = 0x100;
    static inline uintptr_t UnlimitedAmmo         = 0x114;
    static inline uintptr_t WeaponDamageScale     = 0xF8;
    static inline uintptr_t WeaponAmmoClip        = 0x104;
    static inline uintptr_t WeaponScatterScale    = 0xC4;
    static inline uintptr_t DamageAdditionScale   = 0xE8;
    static inline uintptr_t ExecuteDamageScale    = 0xF0;
    static inline uintptr_t SuperArmorEffecting   = 0x64;
    static inline uintptr_t CanAimassistOnStrop   = 0x70;
    static inline uintptr_t ReloadNoConsumeAmmo   = 0xC0;
    static inline uintptr_t NoReload              = 0xC1;
    static inline uintptr_t NoReload2             = 0xC0;
    static inline uintptr_t FallingSpeedUpScale   = 0x20C;
    static inline uintptr_t InfiniteIceWall       = 0x78;
    static inline uintptr_t InfiniteHealer        = 0x200;
    static inline uintptr_t IsSuperArmorEnable    = 0x208;
    static inline uintptr_t BuffEcaHpLossScale    = 0xDC;
    static inline uintptr_t ShowEnemyFootStep     = 0x142;
    static inline uintptr_t ShowEnermyTargetOnMap = 0x140;
    static inline uintptr_t ShowEnermyTargetOnHud = 0x141;
    static inline uintptr_t EnemyFootStepMaxDistanceDelta = 0x144;
    static inline uintptr_t EnemyFootStepMinDistanceDelta = 0x148;
    static inline uintptr_t EnemyFootStepLimitNumDelta    = 0x14C;
    static inline uintptr_t BuffEcaIgnoreWeaponScatter    = 0x27C;
    static inline uintptr_t BuffEcaIgnoreWeaponAimDecay   = 0x278;
    static inline uintptr_t IsSmoothMovement      = 0x4F;
    static inline uintptr_t SkillCDReduction      = 0x164;
    static inline uintptr_t ActiveSkillCdReduction= 0x160;
    static inline uintptr_t PetSkillCDReduction   = 0x15C;
    static inline uintptr_t ForceSetRunAndDashSpeed = 0x2B8;
    static inline uintptr_t RunSpeedUpScale       = 0x210;
    static inline uintptr_t SpinBotDelta          = 0x2BC;
    static inline uintptr_t GrenadeThrowSpeedScale= 0x1BC;

    // OBSERVER
    static inline uintptr_t CurrentObserver       = 0xB4;
    static inline uintptr_t ObserverPlayer        = 0x28;

    // TIMER / SPEED
    static inline uintptr_t GameTimer             = 0x10;
    static inline uintptr_t FixedDeltaTime        = 0x24;
    static inline uintptr_t DeltaTime             = 0x10;

    // AIM
    static inline uintptr_t sAim1                 = 0x58C;
    static inline uintptr_t sAim2                 = 0x9D0;
    static inline uintptr_t sAim3                 = 0x38;
    static inline uintptr_t sAim4                 = 0x2C;
    static inline uintptr_t LastAimingInfoFromWeaponAdjusted = 0x9D8;
    static inline uintptr_t Hit_HitLoc            = 0x14;
    static inline uintptr_t Hit_Damage            = 0x44;
    static inline uintptr_t Hit_Part              = 0x50;

    // AIM-HIT
    static inline uintptr_t Player_AimHitA        = 0x788;
    static inline uintptr_t Player_AimHitB        = 0x790;
    static inline uintptr_t m_CapsuleHuman        = 0x794;
    static inline uintptr_t m_SniperFireCollider  = 0x798;

    // PHYSICS / CCT
    static inline uintptr_t CCT_UseGravity        = 0x38;
    static inline uintptr_t CCT_GravityScale      = 0x3C;
    static inline uintptr_t CCT_JumpHeight        = 0x68;
    static inline uintptr_t CCT_JumpMaxCount      = 0x6C;
    static inline uintptr_t CCT_JumpCount         = 0x70;
    static inline uintptr_t CCT_MaxSpeed          = 0x34;
    static inline uintptr_t CCT_Velocity          = 0x150;
    static inline uintptr_t CCT_CanMoveInAir      = 0x74;
    static inline uintptr_t CCT_SolveMovementCollisions = 0x7D;
    static inline uintptr_t CCT_CharacterController     = 0x184;
    static inline uintptr_t CCT_Transform              = 0x188;
    static inline uintptr_t CCT_DeltaTime              = 0x134;

    // WINGS
    static inline uintptr_t Wing_FlySpeedH        = 0x14;
    static inline uintptr_t Wing_FlySpeedV        = 0x18;
    static inline uintptr_t Wing_FallSpeed        = 0x1C;
    static inline uintptr_t Wing_FlyMaxHeight     = 0x24;
    static inline uintptr_t Wing_OverheightFallSpeed = 0x28;
    static inline uintptr_t Wing_TurnSpeed        = 0x20;

    // PROFILE
    static inline uintptr_t BaseProfileInfo       = 0x1A74;
    static inline uintptr_t Profile_RankPoint     = 0x5C;
    static inline uintptr_t Profile_AccountID     = 0x8;
    static inline uintptr_t Profile_Level         = 0x14;
    static inline uintptr_t Profile_Rank          = 0x58;
    static inline uintptr_t Profile_CSRank        = 0x64;
    static inline uintptr_t Profile_ClanName      = 0x30;
    static inline uintptr_t Profile_Region        = 0x10;
    static inline uintptr_t Profile_NickName      = 0x18;
    static inline uintptr_t Profile_Banner        = 0x50;
    static inline uintptr_t Profile_HeadPic       = 0x54;
    static inline uintptr_t Profile_CSRankPoint   = 0x68;
    static inline uintptr_t Profile_ClanID        = 0x38;
    static inline uintptr_t PrimeLevel            = 0x14;

    // COLLIDERS / HEAD
    static inline uintptr_t HeadCollider          = 0x4E8;
    static inline uintptr_t LockedCollider        = 0x54;
    static inline uintptr_t BoneList              = 0x7B4;
    static inline uintptr_t ColliderHECFNHJKOMN   = 0x4E8;
    static inline uintptr_t ColliderINICDNFOFJB   = 0x54;

    // VIDA / SHORT
    static inline uintptr_t Vida                  = 0xC;

    // TRANSFORM
    static inline uintptr_t TransformNode_Transform = 0x8;
    static inline uintptr_t PhysX_MovingDirection   = 0x14;
    static inline uintptr_t PhysX_Velocity          = 0x20;

    // MISC
    static inline uintptr_t NoGravity             = 0x14E8;
    static inline uintptr_t Player_JumpCount      = 0x180;
    static inline uintptr_t Player_BlockUserMoveControl = 0x18D8;
    static inline uintptr_t Player_JSGhost        = 0x13D0;
    static inline uintptr_t Player_JSGhostModel   = 0x13E4;
    static inline uintptr_t PlayerNetwork_LastValidTick = 0x1A58;

    // BỔ SUNG CHO GHOSTHACK.CPP VÀ TELEPNEU.CPP
    static inline uintptr_t GhostMode             = 0x13D0;
    static inline uintptr_t telepneu              = 0x1A58;

    // CAMERA-3RD
    static inline uintptr_t Cam_RightOffset       = 0x44;
    static inline uintptr_t Cam_UpOffset          = 0x48;
    static inline uintptr_t Cam_BackOffset        = 0x4C;
    static inline uintptr_t Cam_FOVOffset         = 0x50;
    static inline uintptr_t Cam_IsOnHead          = 0x7C;
    static inline uintptr_t Cam_NeedUpdate        = 0x7D;
    static inline uintptr_t Cam_CollisionOffsetRight = 0x80;
    static inline uintptr_t Cam_CollisionOffsetBack  = 0x84;
    static inline uintptr_t RightcameraOffset     = 0x44;
    static inline uintptr_t FOVcameraoffset       = 0x50;
    static inline uintptr_t VisionOffs            = 0x4C;

    // BONES
    class Bones {
    public:
        static inline uintptr_t Head          = 0x49C;
        static inline uintptr_t Neck          = 0x4A4;
        static inline uintptr_t Hip           = 0x4A0;
        static inline uintptr_t Spine         = 0x4A8;
        static inline uintptr_t Root          = 0x4B0;
        static inline uintptr_t LeftCalf      = 0x4B8;
        static inline uintptr_t RightCalf     = 0x4BC;
        static inline uintptr_t LeftFoot      = 0x4C0;
        static inline uintptr_t RightFoot     = 0x4C4;
        static inline uintptr_t RightWrist    = 0x4C8;
        static inline uintptr_t LeftShoulder  = 0x4D0;
        static inline uintptr_t RightShoulder = 0x4D4;
        static inline uintptr_t RightHand     = 0x4D8;
        static inline uintptr_t LeftHand      = 0x4DC;
        static inline uintptr_t RightElbow    = 0x4E0;
        static inline uintptr_t LeftElbow     = 0x4E4;

        static inline uintptr_t LeftWrist     = 0x4DC;
        static inline uintptr_t LeftAnkle     = 0x4C0;
        static inline uintptr_t RightAnkle    = 0x4C4;
        static inline uintptr_t Hand          = 0x4D8;
        static inline uintptr_t RootBone      = 0x4B0;
        static inline uintptr_t Groin         = 0x4A0;
    };
};
