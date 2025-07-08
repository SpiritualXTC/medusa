#pragma once

#include <boost/signals2/signal.hpp>

#include <medusa/renderer/view.h>


namespace medusa
{
    // Forward Declarations
    class ILayer;
    class IContext;

    class View : public IView
    {
    public:
        typedef boost::signals2::signal<void(float&, float&)> OnViewResize;

    public:
        View(std::shared_ptr<IContext> context, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
        virtual ~View();

        bool resize(uint32_t width, uint32_t height) override;

        bool render() override;


        OnViewResize onViewResize;


        inline const glm::ivec2& screen() const { return _screen; }
        inline const glm::vec2& position() const { return _position; }
        inline const glm::vec2& size() const { return _size; }


        virtual std::shared_ptr<ILayer> layer() = 0;

    private:
        std::shared_ptr<IContext> _context;

        glm::ivec2 _screen;
        glm::vec2 _position;
        glm::vec2 _size;
    };


    class MultiView : public IView
    {
    public:
        MultiView();
        virtual ~MultiView();

        bool addView(std::shared_ptr<IView> view);


        bool resize(uint32_t width, uint32_t height) override;

        bool render() override;

    private:
        std::vector<std::shared_ptr<IView>> _views;
    };
}
