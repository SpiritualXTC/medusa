#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>


namespace medusa
{
    class Model;

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
            std::shared_ptr<Model> load(const std::string& filename);

        private:
            std::weak_ptr<IContext> _context;
        };
    }
}
