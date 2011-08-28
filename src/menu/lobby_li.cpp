/*
 * menu/lobby_li.cpp
 *
 */

#include "lobby_li.h"

#include "../graphic/gra_lib.h"
#include "../network/network.h"
#include "../other/help.h"

namespace Api {

const unsigned LobbyList::LobbyDataButton::this_xl(140);
const unsigned LobbyList::LobbyDataButton::this_yl( 20);
const unsigned LobbyList::LobbyDataButton::text_xl(100);

LobbyList::LobbyDataButton::LobbyDataButton(const int x, const int y)
:
    Button(x, y, this_xl, this_yl),
    spec  (false),
    ready (false),
    name  (),
    style (LixEn::GARDEN),
    cutbit(&GraLib::get_lix(LixEn::GARDEN)),
    cutchk(&GraLib::get(gloB->file_bitmap_checkbox))
{
    set_undraw_color(color[COL_API_M]);
}

LobbyList::LobbyDataButton::~LobbyDataButton()
{
}

void LobbyList::LobbyDataButton::read_from_pldatcit(
    const Network::PlDatCIt& itr
) {
    set_hidden(itr->number == -1);
    spec   = itr->spec;
    ready  = itr->ready;
    style  = static_cast <const LixEn::Style> (itr->style);
    cutbit = &GraLib::get_lix(style);
    name   = itr->name;
    while ((unsigned) text_length(font_med, name.c_str()) > text_xl)
     name.erase(--name.end());
    set_draw_required();
}

void LobbyList::LobbyDataButton::draw_self()
{
    Button::draw_self();
    if (!spec) cutbit->draw(get_ground(), get_x_here(), get_y_here(),
     18, LixEn::WALKER-1);
    Help::draw_shadow_centered_text(get_ground(), font_med, name.c_str(),
     get_x_here() + this_xl/2, get_y_here(), color[COL_TEXT]);
    cutchk->draw(get_ground(), get_x_here()+get_xl()-20, get_y_here(),ready,0);
}



// ############################################################################
// ############################################################################
// ############################################################################



LobbyList::LobbyList(const int lx, const int ly)
:
    Frame(lx, ly, LobbyDataButton::this_xl,
       buttons_max * LobbyDataButton::this_yl)
{
    for (unsigned y = 0; y < buttons_max; ++y)
     button.push_back(LobbyDataButton(0, y * LobbyDataButton::this_yl));
    for (std::vector <LobbyDataButton> ::iterator itr = button.begin();
     itr != button.end(); ++itr) {
        itr->hide(); // don't show all before first network update
        add_child(*itr);
    }
}

LobbyList::~LobbyList()
{
}

void LobbyList::set_data(const std::list <PlayerData>& pdli)
{
    for (size_t i = 0; i < button.size(); ++i) button[i].hide();
    unsigned count = 0;
    // Active players
    for (Network::PlDatCIt itr = pdli.begin(); itr != pdli.end(); ++itr)
     if (!itr->spec && count < buttons_max)
     button[count++].read_from_pldatcit(itr);
    // Spectators
    for (Network::PlDatCIt itr = pdli.begin(); itr != pdli.end(); ++itr)
     if (itr->spec && count < buttons_max)
     button[count++].read_from_pldatcit(itr);
    set_draw_required();
}



// ############################################################################
// ############################################################################
// ############################################################################



const unsigned LobbyColorButton::this_xl(20);
const unsigned LobbyColorButton::this_yl(32);

LobbyColorButton::LobbyColorButton(
    const int nx,
    const int ny,
    const LixEn::Style st)
:
    Button(nx, ny, this_xl, this_yl),
    icon  (GraLib::get_lix(st), get_ground(), get_x() - 7, get_y() - 3)
{
    icon.set_y_frame(LixEn::WALKER - 1);
    set_undraw_color(color[COL_API_M]);
}

LobbyColorButton::~LobbyColorButton()
{
}

void LobbyColorButton::draw_self()
{
    Button::draw_self();
    icon.set_x_frame(19);
    icon.draw();
}






// Ende Namensraum Api
}
