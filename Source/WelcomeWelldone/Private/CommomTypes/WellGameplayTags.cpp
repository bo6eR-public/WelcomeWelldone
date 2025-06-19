// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/WellGameplayTags.h"

namespace WellGameplayTags
{
	/** Input Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Move, "Input.Native.Move");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Look, "Input.Native.Look");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Jump, "Input.Native.Jump");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Run, "Input.Native.Run");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Optional1, "Input.Native.Optional1");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Input_Native_Optional2, "Input.Native.Optional2");

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

	/** Message Tags **/
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Message_Attribute, "Message.Attribute");
	WELCOMEWELLDONE_API UE_DEFINE_GAMEPLAY_TAG(Message_Instance_Weapon, "Message.Instance.Weapon");
}
