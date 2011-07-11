/*
 * menu/browser.h
 *
 * Dies enthaelt die erweiterten LevelBrowser-Klassen, naemlich den Browser
 * fuer den Einzelspielermodus sowie den Levelauswaehler im Netzwerk.
 *
 * Ferner ist hier der ReplayBrowser definiert, auch wenn seine Funktionen
 * nicht in der brow_sin.cpp, sondern in der brow_rep.cpp definiert sind.
 *
 */

#pragma once

#include "brow_big.h"

#include "../api/brow_sav.h"

namespace Api {
class SingleBrowser : public BrowserBig {

private:

    TextButton button_edit;

    LabelTwo info_initial,  info_required, info_clock;
    LabelTwo rec_saved,     rec_skills,    rec_updates;
    Label    rec_altered_1, rec_altered_2;

    SingleBrowser   (const SingleBrowser&);
    void operator = (const SingleBrowser&);

    virtual void on_file_highlight(const Filename&);
    virtual void on_file_select(   const Filename&);

public:

             SingleBrowser();
    virtual ~SingleBrowser();

protected:

    virtual void calc_self();

};
// Ende Klasse SingleBrowser



class NetworkBrowser : public BrowserBig {

private:

    LabelTwo info_hatches, info_goals, info_initial, info_skills, info_clock;

    NetworkBrowser  (const NetworkBrowser&);
    void operator = (const NetworkBrowser&);

    virtual void on_file_highlight(const Filename&);
    virtual void on_file_select(   const Filename&);

public:
             NetworkBrowser();
    virtual ~NetworkBrowser();

protected:

    virtual void calc_self();

};
// Ende Klasse NetworkBrowser



class ReplayBrowser : public BrowserBig {

private:

    TextButton      button_extract;
    TextButton      button_delete;
    Label           label_info_player,  desc_info_player,
                    label_info_version, desc_info_version,
                    label_info_built,   desc_info_built;

    SaveBrowser*    browser_save;
    BoxMessage*     box_delete;

    ReplayBrowser   (const ReplayBrowser&);
    void operator = (const ReplayBrowser&);
    virtual void on_file_highlight(const Filename&);
    virtual void on_file_select(   const Filename&);

public:

             ReplayBrowser();
    virtual ~ReplayBrowser();

protected:

    virtual void calc_self();

};
// Ende Klasse NetworkBrowser

}
// Ende Namensraum Api
