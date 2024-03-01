#pragma once

#include "Outfit.h"
#include "EquipConditions.h"

namespace Gotobed
{
	struct Settings
	{
		using Outfits = std::unordered_map<std::string, Outfit>;

		struct Actor {
			std::string		sleepOutfit;
			bool			useVanillaSleepOutfit{true};
			EquipConditions	sleepOutfitEquipConditions;
		};

		using Actors = std::unordered_map<RE::FormID, Actor>;

		static Settings&	Get();

		void	Read();
		void	Write();

		Outfits		outfits;
		Actors		actors;
		Actor		actorDefault;
	};

	void to_json(json& a_json, Settings::Actor const& a_actor);
	void from_json(json const& a_json, Settings::Actor& a_actor);

	void to_json(json& a_json, Settings const& a_settings);
	void from_json(json const& a_json, Settings& a_settings);
}