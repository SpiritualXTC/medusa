#pragma once

#include <boost/signals2/signal.hpp>

#include <SDL.h>

namespace medusa
{
	/// <summary>
	///
	/// </summary>
	class IEventHandler
	{
	public:
		//
		IEventHandler() {}

		//
		virtual ~IEventHandler() {}

		//
		virtual bool handleEvents(const SDL_Event& evt) = 0;

	private:

	};

}
