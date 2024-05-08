#include "GAS/GameplayEffectContext/TDAbilityTypes.h"

UScriptStruct* FTDGameplayEffectContext::GetScriptStruct() const
{
	// Returns the actual struct used for serialization.
	return StaticStruct();
}

FGameplayEffectContext* FTDGameplayEffectContext::Duplicate() const
{
	//* Creates a copy of this context, used to duplicate for later modifications */

	FGameplayEffectContext* NewContext = new FGameplayEffectContext();
	*NewContext = *this;

	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}

	return NewContext;
}

bool FTDGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		//***************************************************************************
		//* Custom 추가부분: BlockHit, CriticalHit, Debuff

		if (bBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (RagdollImpulse != FVector::ZeroVector)
		{
			RepBits |= 1 << 14;
		}

		//***************************************************************************
	}

	//** 변경된 RepBits값을 바탕으로 Serialize를 해야되는 것들을 Serialize 해준다.
	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	//***************************************************************************
	//* Custom 추가부분: BlockHit, CriticalHit, Debuff

	if (RepBits & (1 << 7))
	{
		Ar << bBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (false == DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag()); // DamageType 없다면 생성 후 빈 값으로 설정.
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		RagdollImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}

	//****************************************************************************

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // InstigatorAbilitySystemComponent 초기화
	}

	bOutSuccess = true;
	return true;
}
