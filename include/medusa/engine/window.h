#pragma once

#include <boost/signals2/signal.hpp>

#include <medusa/medusa.h>
#include <medusa/engine/events.h>


namespace medusa
{
	// Window Events
	struct CloseEvent
	{

	};


	struct ResizeEvent
	{
		uint32_t width;
		uint32_t height;
	};


	// Event Definitions
	typedef boost::signals2::signal<void(ResizeEvent&)> OnResize;
	typedef boost::signals2::signal<void(CloseEvent&)> OnClose;


	/// <summary>
	///
	/// </summary>
	class IWindow : public medusa::IEventHandler
	{
	public:
		IWindow() {}
		virtual ~IWindow() {}

		//
		virtual glm::ivec2 getDisplaySize() const = 0;

		//
		virtual void setTitle(const std::string& title) = 0;


		// Signals
		OnResize onResize;
		OnClose onClose;

	private:
	};
}
