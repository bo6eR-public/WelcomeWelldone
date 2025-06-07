// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/WellGameplayTags.h"


namespace WellGameplayTags
{
	/** Input Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Sneak, "InputTag.Sneak");

	/** States Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Jump, "State.Jump");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Sneak, "State.Sneak");
}
