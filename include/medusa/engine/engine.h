#pragma once

#include <medusa/medusa.h>

#include <medusa/engine_fwd.h>

namespace medusa
{
    class IApplication;

    class IEngine
    {
    public:
        IEngine() {}
        virtual ~IEngine() {}

        /// <summary>
        ///
        /// </summary>
        /// <param name="application"></param>
        virtual void run(std::shared_ptr<IApplication> application) = 0;


        /// <summary>
        ///
        /// </summary>
        virtual void quit() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual const inline std::shared_ptr<IWindow>& window() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual const inline std::shared_ptr<IContext>& context() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual const inline std::shared_ptr<Timing>& timing() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual const inline std::shared_ptr<IAssets>& assets() = 0;
    };

}
