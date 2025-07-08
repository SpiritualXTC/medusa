#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/state.h>


namespace medusa
{
    /// <summary>
    /// Application / System State
    /// </summary>
    class MedusaState : public IState
    {
    public:
        MedusaState();
        virtual ~MedusaState();

        bool reset() override;

    private:


    };



}
