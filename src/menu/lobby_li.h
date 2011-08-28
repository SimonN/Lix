/*
 * menu/lobby_li.h
 *
 * Die Liste mit den derzeit im Netzwerk anwesenden Spielern
 *
 * Derzeit ist sie fuer acht Spieler angelegt, mehr geht nicht. Die Liste
 * versorgt sich auch nicht selbst aus dem Netzwerkobjekt, sondern muss von
 * der Lobby bei Bedarf mit neuen Daten gefuettert werden.
 *
 */

#pragma once

#include <string>

#include "../api/frame.h"
#include "../api/button/button.h"
#include "../graphic/graphic.h"
#include "../lix/lix_enum.h"
#include "../network/network.h"

namespace Api {
class LobbyList : public Frame {

private:

    static const unsigned buttons_max = 8;

    struct LobbyDataButton : public Button {
        static const unsigned this_xl;
        static const unsigned this_yl;
        static const unsigned text_xl;
        bool           spec;
        bool           ready;
        std::string    name;
        LixEn::Style   style;
        const Cutbit*  cutbit;
        const Cutbit*  cutchk;
        LobbyDataButton(const int = 0, const int = 0);
        virtual ~LobbyDataButton();
        void read_from_pldatcit(const Network::PlDatCIt&);
    protected:
        virtual void   draw_self();
    };

    std::vector <LobbyDataButton> button;

protected:

    inline virtual void calc() {} // don't appear pressed in when clicked

public:

             LobbyList(const int = 0, const int = 0);
    virtual ~LobbyList();

    void set_data(const std::list <PlayerData>&);

};



class LobbyColorButton : public Button {
    Graphic icon;
public:
    static const unsigned this_xl;
    static const unsigned this_yl;
    LobbyColorButton(const int = 0, const int = 0,
                     const LixEn::Style = LixEn::GARDEN);
    virtual ~LobbyColorButton();
protected:
    virtual void draw_self();
};

}
