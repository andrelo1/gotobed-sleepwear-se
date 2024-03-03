#include "init.h"
#include "Settings.h"
#include "Offsets.h"
#include "AIProcess.h"
#include "SettingsPapyrus.h"
#include "ActorData.h"

namespace Gotobed
{
	void OnSave(SKSE::SerializationInterface* a_intfc) {
		if (!a_intfc) {
			return;
		}

		ActorData::Save(*a_intfc);
	}

	void OnLoad(SKSE::SerializationInterface* a_intfc) {
		if (!a_intfc) {
			return;
		}

		std::uint32_t type, version, length;

		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			ActorData::Load(*a_intfc, type, version, length);
		}
	}

	void OnDataLoaded() {
		auto& settings = Settings::Get();
		settings.Read();

		AIProcess::InstallHooks();

		// papyrus
		SettingsPapyrus::Register();

		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(0x360FC0D6);
		serialization->SetSaveCallback(OnSave);
		serialization->SetLoadCallback(OnLoad);
	}

	void Init() {
		SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
			if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
				OnDataLoaded();
			}
		});
	}
}