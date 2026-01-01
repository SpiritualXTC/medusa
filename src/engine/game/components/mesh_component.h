#pragma once

#include <medusa/graphics.h>

#include <medusa/game/component.h>



namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class MeshComponent : public IComponent
    {
    public:
        MeshComponent(std::shared_ptr<IMeshInstance> instance);
        virtual ~MeshComponent();

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        bool update() override;

    private:
        std::shared_ptr<IMeshInstance> _instance;
    };


} // namespace medusa
