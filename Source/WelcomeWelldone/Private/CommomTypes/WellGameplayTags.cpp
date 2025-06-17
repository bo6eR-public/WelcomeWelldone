// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/WellGameplayTags.h"

namespace WellGameplayTags
{
	/** Input Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Run, "Input.Run");

	//~ Weapon Input Tags ~//
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Weapon_Fire, "Input.Weapon.Fire");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Weapon_Aim, "Input.Weapon.Aim");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Weapon_Reload, "Input.Weapon.Reload");

	/** States Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Jump, "State.Jump");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Run, "State.Run");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Reload, "State.Reload");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(State_Equip, "State.Equip");

	/** Event Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Event_Reload, "Event.Reload");
}
