#include "Serialize.h"
#include "ActorManager.h"

using namespace OM;

void Serialize::Save(SKSE::SerializationInterface* a_intfc)
{
	logger::info("Serialize::Save");
	if (!a_intfc->OpenRecord(RecordName, 0)) {
		logger::error("Unable to open record to write cosave data.");
		return;
	}

	ActorManager::Save(a_intfc);
}

void Serialize::Load(SKSE::SerializationInterface* a_intfc)
{
	logger::info("Serialize::Load");
	
	uint32_t type;
	uint32_t version;
	uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (type == RecordName) {
			ActorManager::Load(a_intfc);
			break;
		}
	}
	logger::info("Serialize::Load done");
}

void Serialize::Revert(SKSE::SerializationInterface*)
{
	logger::info("Serialize::Revert");
	ActorManager::Revert();
}