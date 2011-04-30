/*
 * b_combin.h
 *
 * class CombinedButton : public Button
 *
 * A button that displays a level object, i.e. either a terrain piece or a
 * special object. It can also be given a name to display below
 *
 */

#pragma once

#include "../../other/myalleg.h"
#include <string>

#include "button.h"

#include "../../level/object.h"

namespace Api {

class CombinedButton : public Button {

private:

    std::string   text;
    const Object* object;

public:

             CombinedButton(int=0, int=0, int=60, int=60);
             CombinedButton(const CombinedButton&);
    virtual ~CombinedButton();

    inline std::string&  get_text()           { return text;                  }
    inline void set_text(const std::string&s) { text = s; set_draw_required();}
    inline const Object* get_object()         { return object;                }
           void          set_object(const Object*);

protected:

    virtual void draw_self();

};

} // Ende Api
