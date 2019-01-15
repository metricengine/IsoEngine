#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "window.h"

namespace iso
{

class Drawable
{
    friend class Window;

public:
    virtual ~Drawable() {}

private:
    virtual void draw(Window & window) const = 0;
};

} // namespace iso

#endif // DRAWABLE_H
