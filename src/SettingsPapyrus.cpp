#include "SettingsPapyrus.h"
#include "Settings.h"

namespace Gotobed::SettingsPapyrus
{
	void Write(RE::StaticFunctionTag*) {
		Settings::Get().Write();
	}

	std::vector<std::string> GetOutfits(RE::StaticFunctionTag*) {
		auto& settings = Settings::Get();
		std::vector<std::string> outfits;

		for (auto& [id, outfit]: settings.outfits) {
			outfits.push_back(id);
		}

		return outfits;
	}

	void RemoveOutfit(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();
		settings.outfits.erase(a_outfit);
	}

	std::string Outfit_GetName(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return "";
		}

		return settings.outfits[a_outfit].name;
	}

	void Outfit_SetName(RE::StaticFunctionTag*, std::string a_outfit, std::string a_name) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].name = a_name;
	}

	std::vector<RE::TESForm*> Outfit_GetItems(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return {};
		}

		std::vector<RE::TESForm*> items;

		for (auto& params: settings.outfits[a_outfit].items) {
			if (params.item) {
				items.push_back(params.item);
			}
		}

		return items;
	}

	void Outfit_SetItems(RE::StaticFunctionTag*, std::string a_outfit, std::vector<RE::TESForm*> a_items) {
		auto& settings = Settings::Get();
		auto& params = settings.outfits[a_outfit].items;
		params.clear();

		for (auto& item: a_items) {
			if (item && item->IsBoundObject()) {
				params.push_back({static_cast<RE::TESBoundObject*>(item), 1, nullptr});
			}
		}
	}

	std::int32_t Outfit_GetArmorMask(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return 0;
		}

		return static_cast<std::int32_t>(settings.outfits[a_outfit].mask.armor);
	}

	void Outfit_SetArmorMask(RE::StaticFunctionTag*, std::string a_outfit, std::int32_t a_mask) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].mask.armor = static_cast<std::uint32_t>(a_mask);
	}

	bool Outfit_GetLHandMask(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return false;
		}

		return settings.outfits[a_outfit].mask.lhand;
	}

	void Outfit_SetLHandMask(RE::StaticFunctionTag*, std::string a_outfit, bool a_mask) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].mask.lhand = a_mask;
	}

	bool Outfit_GetRHandMask(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return false;
		}

		return settings.outfits[a_outfit].mask.rhand;
	}

	void Outfit_SetRHandMask(RE::StaticFunctionTag*, std::string a_outfit, bool a_mask) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].mask.rhand = a_mask;
	}

	bool Outfit_GetAmmoMask(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return false;
		}

		return settings.outfits[a_outfit].mask.ammo;
	}

	void Outfit_SetAmmoMask(RE::StaticFunctionTag*, std::string a_outfit, bool a_mask) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].mask.ammo = a_mask;
	}

	bool Outfit_GetMiscMask(RE::StaticFunctionTag*, std::string a_outfit) {
		auto& settings = Settings::Get();

		if (!settings.outfits.contains(a_outfit)) {
			return false;
		}

		return settings.outfits[a_outfit].mask.misc;
	}

	void Outfit_SetMiscMask(RE::StaticFunctionTag*, std::string a_outfit, bool a_mask) {
		auto& settings = Settings::Get();
		settings.outfits[a_outfit].mask.misc = a_mask;
	}

	std::vector<RE::TESNPC*> GetActors(RE::StaticFunctionTag*) {
		auto& settings = Settings::Get();
		std::vector<RE::TESNPC*> actors;

		for (auto& [id, actor]: settings.actors) {
			auto form = RE::TESForm::LookupByID(id);
			if (form && form->formType == RE::FormType::NPC) {
				actors.push_back(static_cast<RE::TESNPC*>(form));
			}
		}

		return actors;
	}

	void RemoveActor(RE::StaticFunctionTag*, RE::TESNPC* a_actor) {
		auto& settings = Settings::Get();

		if (!a_actor) {
			return;
		}

		settings.actors.erase(a_actor->formID);
	}

	std::string Actor_GetSleepOutfit(RE::StaticFunctionTag*, RE::TESNPC* a_actor){
		auto& settings = Settings::Get();

		if (!a_actor) {
			return "";
		}

		if (!settings.actors.contains(a_actor->formID)) {
			return "";
		}

		return settings.actors[a_actor->formID].sleepOutfit;
	}

	void Actor_SetSleepOutfit(RE::StaticFunctionTag*, RE::TESNPC* a_actor, std::string a_outfit){
		auto& settings = Settings::Get();

		if (!a_actor) {
			return;
		}

		settings.actors[a_actor->formID].sleepOutfit = a_outfit;
	}

	std::vector<RE::TESForm*> Actor_GetSleepOutfitEquipConditions(RE::StaticFunctionTag*, RE::TESNPC* a_actor){
		auto& settings = Settings::Get();

		if (!a_actor) {
			return {};
		}

		if (!settings.actors.contains(a_actor->formID)) {
			return {};
		}

		std::vector<RE::TESForm*> cond;

		for (auto& type: settings.actors[a_actor->formID].sleepOutfitEquipConditions.locationType) {
			cond.push_back(type);
		}

		return cond;
	}

	void Actor_SetSleepOutfitEquipConditions(RE::StaticFunctionTag*, RE::TESNPC* a_actor, std::vector<RE::TESForm*> a_cond){
		auto& settings = Settings::Get();

		if (!a_actor) {
			return;
		}

		auto& types = settings.actors[a_actor->formID].sleepOutfitEquipConditions.locationType;
		types.clear();

		for (auto& form: a_cond) {
			if (form && form->formType == RE::FormType::Keyword) {
				types.push_back(static_cast<RE::BGSKeyword*>(form));
			}
		}
	}

	bool Actor_GetUseVanillaSleepOutfit(RE::StaticFunctionTag*, RE::TESNPC* a_actor) {
		auto& settings = Settings::Get();

		if (!a_actor) {
			return false;
		}

		if (!settings.actors.contains(a_actor->formID)) {
			return false;
		}

		return settings.actors[a_actor->formID].useVanillaSleepOutfit;
	}

	void Actor_SetUseVanillaSleepOutfit(RE::StaticFunctionTag*, RE::TESNPC* a_actor, bool a_val) {
		auto& settings = Settings::Get();

		if (!a_actor) {
			return;
		}

		settings.actors[a_actor->formID].useVanillaSleepOutfit = a_val;
	}

	void Register() {
		SKSE::GetPapyrusInterface()->Register([](RE::BSScript::IVirtualMachine* a_vm) {
			a_vm->RegisterFunction("Write", "GTB_SettingsSleepwear", Write);
			a_vm->RegisterFunction("GetOutfits", "GTB_SettingsSleepwear", GetOutfits);
			a_vm->RegisterFunction("RemoveOutfit", "GTB_SettingsSleepwear", RemoveOutfit);
			a_vm->RegisterFunction("Outfit_GetName", "GTB_SettingsSleepwear", Outfit_GetName);
			a_vm->RegisterFunction("Outfit_SetName", "GTB_SettingsSleepwear", Outfit_SetName);
			a_vm->RegisterFunction("Outfit_GetItems", "GTB_SettingsSleepwear", Outfit_GetItems);
			a_vm->RegisterFunction("Outfit_SetItems", "GTB_SettingsSleepwear", Outfit_SetItems);
			a_vm->RegisterFunction("Outfit_GetArmorMask", "GTB_SettingsSleepwear", Outfit_GetArmorMask);
			a_vm->RegisterFunction("Outfit_SetArmorMask", "GTB_SettingsSleepwear", Outfit_SetArmorMask);
			a_vm->RegisterFunction("Outfit_GetLHandMask", "GTB_SettingsSleepwear", Outfit_GetLHandMask);
			a_vm->RegisterFunction("Outfit_SetLHandMask", "GTB_SettingsSleepwear", Outfit_SetLHandMask);
			a_vm->RegisterFunction("Outfit_GetRHandMask", "GTB_SettingsSleepwear", Outfit_GetRHandMask);
			a_vm->RegisterFunction("Outfit_SetRHandMask", "GTB_SettingsSleepwear", Outfit_SetRHandMask);
			a_vm->RegisterFunction("Outfit_GetAmmoMask", "GTB_SettingsSleepwear", Outfit_GetAmmoMask);
			a_vm->RegisterFunction("Outfit_SetAmmoMask", "GTB_SettingsSleepwear", Outfit_SetAmmoMask);
			a_vm->RegisterFunction("Outfit_GetMiscMask", "GTB_SettingsSleepwear", Outfit_GetMiscMask);
			a_vm->RegisterFunction("Outfit_SetMiscMask", "GTB_SettingsSleepwear", Outfit_SetMiscMask);
			a_vm->RegisterFunction("GetActors", "GTB_SettingsSleepwear", GetActors);
			a_vm->RegisterFunction("RemoveActor", "GTB_SettingsSleepwear", RemoveActor);
			a_vm->RegisterFunction("Actor_GetSleepOutfit", "GTB_SettingsSleepwear", Actor_GetSleepOutfit);
			a_vm->RegisterFunction("Actor_SetSleepOutfit", "GTB_SettingsSleepwear", Actor_SetSleepOutfit);
			a_vm->RegisterFunction("Actor_GetSleepOutfitEquipConditions", "GTB_SettingsSleepwear", Actor_GetSleepOutfitEquipConditions);
			a_vm->RegisterFunction("Actor_SetSleepOutfitEquipConditions", "GTB_SettingsSleepwear", Actor_SetSleepOutfitEquipConditions);
			a_vm->RegisterFunction("Actor_GetUseVanillaSleepOutfit", "GTB_SettingsSleepwear", Actor_GetUseVanillaSleepOutfit);
			a_vm->RegisterFunction("Actor_SetUseVanillaSleepOutfit", "GTB_SettingsSleepwear", Actor_SetUseVanillaSleepOutfit);
			return true;
		});
	}
}