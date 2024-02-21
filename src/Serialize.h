#pragma once

namespace OM
{
	namespace Serialize
	{
		inline const auto RecordName = _byteswap_ulong('OMAN');

		void Save(SKSE::SerializationInterface* a_intfc);
		void Load(SKSE::SerializationInterface* a_intfc);
		void Revert(SKSE::SerializationInterface* a_intfc);
	}
}