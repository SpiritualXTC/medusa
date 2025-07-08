#pragma once

#include <medusa/medusa.h>

namespace medusa
{
	template <typename T>
	class ResourceID
	{
	private:
		static uint64_t generate_id()
		{
			static uint64_t AUTO_ID = 0;

			return ++AUTO_ID;
		}

	public:
		ResourceID() {}
		virtual ~ResourceID() {}

		inline const uint64_t id() const { return _id; }

	private:
		uint64_t _id = ResourceID::generate_id();
	};

}
