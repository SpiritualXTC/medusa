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
            ModelLoader(std::shared_ptr<IContext> context);
            virtual ~ModelLoader();

            //
            std::shared_ptr<Geometry> load(const std::string& filename);

        private:
            std::weak_ptr<IContext> _context;
        };
    }
}
