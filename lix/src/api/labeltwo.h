/*
 * api/labeltwo.h
 *
 * A description with a number/string in white to its right.
 *
 * The white part is right-aligned and blocky, or left-aligned and
 * written where the first part stops.
 *
 */

#pragma once

#include "label.h"

namespace Api {

class LabelTwo : public Element {

public:

    //       x        y        xl
    LabelTwo(int = 0, int = 0, int = 0);
    virtual ~LabelTwo() {}

    void set_desc (const std::string& = "");
    void set_value(const std::string& = "");
    void set_value(const int);
    void set_value_seconds_as_time(const int);

private:

    Label desc;
    Label value;

};
}
