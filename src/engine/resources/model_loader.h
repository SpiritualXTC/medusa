#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>


namespace medusa
{
    class Geometry;

    namespace loaders
    {
        /// <summary>
        ///
        /// </summary>
        class ModelLoader
        {
        public:
            ModelLoader() = delete;

            static std::shared_ptr<Geometry> loadModel(std::shared_ptr<IContext> context, const std::string& filename);
        };
    }
}
