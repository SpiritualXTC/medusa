#pragma once

#include <medusa/medusa.h>

namespace medusa
{
	class IState
	{
	public:
		IState() {}
		virtual ~IState() {}

		/// <summary>
		///
		/// </summary>
		/// <returns></returns>
		virtual bool reset() = 0;
	};
}
