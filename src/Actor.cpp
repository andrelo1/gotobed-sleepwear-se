#include "Actor.h"
#include "Outfit.h"
#include "EquipConditions.h"
#include "Offsets.h"
#include "Settings.h"
#include "ActorData.h"
#include <ranges>

namespace Gotobed
{	
	namespace
	{
		void equip(Actor& a_actor, EquipParams const& a_params) {
			if (a_params.count) {
				if (a_params.count > 0) {
					a_actor.EquipItem(a_params.item, nullptr, a_params.count, a_params.slot);
				} else {
					a_actor.UnequipItem(a_params.item, nullptr, -a_params.count, a_params.slot);
				}
			}
		};

		void equip(Actor& a_actor, EquipSequence const& a_seq) {
			for (auto const& e : a_seq) {
				equip(a_actor, e);
			}
		}

		void reverse_equip(Actor& a_actor, EquipSequence const& a_seq) {
			for (auto e: a_seq | std::views::reverse) {
				e.count = - e.count;
				equip(a_actor, e);
			}
		}

		EquipSequence get_worn_items(Actor& a_actor) {
			EquipSequence items;
			auto inv = a_actor.GetInventory();

			for (auto const& [item, itemData] : inv) {
				auto const& [count, invData] = itemData;
				if (invData->extraLists) {
					for (auto const& extraList : *invData->extraLists) {
						if (extraList && (extraList->HasType(RE::ExtraDataType::kWorn) || extraList->HasType(RE::ExtraDataType::kWornLeft))) {
							EquipParams params;
							params.item = item;
							params.count = extraList->GetCount();

							if (item->IsWeapon()) {
								auto dom = RE::BGSDefaultObjectManager::GetSingleton();
								auto left = extraList->HasType(RE::ExtraDataType::kWornLeft);
								params.slot = dom->GetObject<RE::BGSEquipSlot>(left ? RE::DEFAULT_OBJECTS::kLeftHandEquip : RE::DEFAULT_OBJECTS::kRightHandEquip);
							}

							items.push_back(std::move(params));
						}
					}
				}
			}

			return items;
		}
	}

	RE::BGSLocation* Actor::GetCurrentLocation() {
		using func_t = decltype(&Actor::GetCurrentLocation);
		REL::Relocation<func_t> func{Offsets::TESObjectREFR::GetCurrentLocation};
		return func(this);
	}

	void Actor::EquipItem(RE::TESBoundObject* a_item, RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot, bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow) {
		RE::ActorEquipManager::GetSingleton()->EquipObject(this, a_item, a_extraData, a_count, a_slot, a_queueEquip, a_forceEquip, a_playSounds, a_applyNow);
	}

	void Actor::UnequipItem(RE::TESBoundObject* a_item, RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot, bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow, const RE::BGSEquipSlot* a_slotToReplace) {
		RE::ActorEquipManager::GetSingleton()->UnequipObject(this, a_item, a_extraData, a_count, a_slot, a_queueEquip, a_forceEquip, a_playSounds, a_applyNow, a_slotToReplace);
	}

	void Actor::SetOutfit(Outfit const& a_outfit) {
		EquipSequence seq;
		auto wornItems = get_worn_items(*this);
		auto items1 = wornItems | std::views::filter(a_outfit.mask);
		auto items2 = a_outfit.items | std::views::filter(a_outfit.mask);

		for (auto e1: items1) {
			if (e1.item->formType == RE::FormType::Armor) {
				if (std::ranges::find_if(items2, [&](auto const& e2) { return e1.item == e2.item; }) != items2.end()) {
					continue;
				}
			}
			e1.count = -e1.count;
			seq.push_back(std::move(e1));
		}

		for (auto const& e2: items2) {
			if (e2.item->formType == RE::FormType::Armor) {
				if (std::ranges::find_if(items1, [&](auto const& e1) { return e1.item == e2.item; }) != items1.end()) {
					continue;
				}
			}
			seq.push_back(e2);
		}

		equip(*this, seq);

		auto& data = ActorData::Get(*this);
		std::lock_guard<std::mutex> lg(data.lock);
		data.equipHistory.insert(data.equipHistory.end(), seq.begin(), seq.end());
	}

	void Actor::RevertOutfit() {
		auto& data = ActorData::Get(*this);
		std::lock_guard<std::mutex> lg(data.lock);
		reverse_equip(*this, data.equipHistory);
		data.equipHistory.clear();
	}

	std::optional<Outfit> Actor::GetSleepOutfit() {
		auto actorBase = GetActorBase();

		if (!actorBase) {
			return std::nullopt;
		}

		auto& settings = Settings::Get();
		auto& actorSettings = settings.actors.contains(actorBase->formID) ? settings.actors[actorBase->formID] : settings.actorDefault;

		if (actorSettings.useVanillaSleepOutfit) {
			if (!actorBase->sleepOutfit) {
				return std::nullopt;
			}

			return *actorBase->sleepOutfit;
		} else {
			if (actorSettings.sleepOutfit.empty() || !settings.outfits.contains(actorSettings.sleepOutfit)) {
				return std::nullopt;
			}

			return settings.outfits[actorSettings.sleepOutfit];
		}
	}

	void Actor::UpdateOutfit() {
		if (IsInCombat()) {
			return;
		}

		auto actorBase = GetActorBase();

		if (!actorBase) {
			return;
		}

		auto& settings = Settings::Get();
		auto& actorSettigns = settings.actors.contains(actorBase->formID) ? settings.actors[actorBase->formID] : settings.actorDefault;
		auto& conditions = actorSettigns.sleepOutfitEquipConditions;

		if (!conditions(*this)) {
			RevertOutfit();
			return;
		}

		auto outfit = GetSleepOutfit();

		if (!outfit) {
			RevertOutfit();
			return;
		}

		SetOutfit(*outfit);
	}
}