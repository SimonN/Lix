/*
 * editor.h
 *
 * Der Editor benutzt die globalen Variablen fuer Einzelspieler-Verzeichnis
 * und Einzelspieler-Level.
 *
 */

#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>

#include "graph_ed.h"
#include "bit_brow.h"
#include "win_size.h"
#include "win_scro.h"
#include "win_var.h"
#include "win_skil.h"

#include "../api/brow_sav.h"
#include "../api/button/b_bitmap.h" // Steuerleiste besteht aus BitmapButtons
#include "../graphic/graph_bg.h"
#include "../graphic/map.h"
#include "../other/globals.h"
#include "../level/level.h"
#include "../other/help.h"  // Maus

class Editor {

private:

    bool     exit;

    unsigned timer_ticks_offset;
    unsigned ticks;

    int  mx; // Maus-Koordinaten relativ zur Landschaft, nicht zum OSD
    int  my;
    const int mouse_cursor_offset;
    GraphicBG mouse_cursor;
    bool      mouse_hold_started_outside_panel;

    Level level;
    int   bg_color;
    Map   map;
    Map   map_frames; // Frames are drawn upon here. This map is resized
                      // and scrolled only in the drawing code, and only if
                      // there are changes that have been made to Map map.

    typedef std::list <EdGraphic> GraLi;

    typedef GraLi::iterator       GraIt;
    typedef GraLi::const_iterator GraCIt;

    std::vector <GraLi> object;

    struct Selection {
        GraIt  o;                                 // ein ausgewaehltes Objekt
        GraLi* l;                                 // zeigt auf dessen Liste
        inline Selection() { l = 0; }             // ungueltig initialisiern
        inline Selection(GraLi* nl, GraIt no)  { l = nl; o = no; }
        inline bool operator == (Selection& s) { return o == s.o && l == s.l; }
        inline bool is_valid() { return l != 0; } // gueltig: zeigt auf Objekt
    };

    std::list         <Selection> selection; // Fasst mehrere, etwa im Rahmen
    std::list         <Selection> hover;     // Worauf zeigt die Maus gerade
                                             // oder was kaeme in den Rahmen?
    typedef std::list <Selection> ::iterator SelIt;

    Selection find_under_mouse_cursor();
    void find_check      (Selection&, GraLi&, GraIt);
    void find_check_at   (Selection&, GraLi&, GraIt,    const int, const int);
    void find_check_frame(GraLi&, const int, const int, const int, const int);

    bool get_overlap     (const EdGraphic&, int, int, int, int, bool, bool);
    bool get_overlap_at  (const EdGraphic&, int, int, int, int);
    bool get_overlap_at_2(const EdGraphic&, int, int, int, int);

    void delete_dublicates_from_selection();
    void delete_from_selection(Selection&);

    void rotate_selection_individual();
    void rotate_selection();
    void flip_selection_individual();
    void flip_selection();

    void selection_fore_background(const bool, const bool);

    // Rahmen ziehen
    int  frame_start_x; // Wo auf der Karte
    int  frame_start_y;
    int  frame_draw_x1;
    int  frame_draw_y1;
    int  frame_draw_x2;
    int  frame_draw_y2;
    bool frame_torus_x; // Ueber die Nahtstelle zeichnen anstatt den
    bool frame_torus_y; // normalen Weg entlang?

    // Diese Variablen sind für das Umherziehen von Objekten wichtig
    int  mx_grid_last;
    int  my_grid_last;

    bool draw_required; // draw the landscape during the next draw() call
    bool draw_dragging; // if level drawing takes very long, do not redraw
                        // when dragging objects with the mouse, instead
                        // only redraw after releasing mouse button.

    std::vector <Api::BitmapButton> panel;

    ////////////////
    // Funktionen //
    ////////////////

    void calc_self(); // Wenn kein Fenster offen ist

    int  get_grid();
    void save_lists_and_level();
    void save_lists();
    void delete_everything(); // Neu-Button

    void draw_selection_borders();
    void draw_selection_border   (const Selection&, const int);
    void draw_selection_border_at(const Selection&, const int,
                                  const int,        const int);
    void draw_selection_frame    (const int);
    void draw_selection_frame_at (const int,        const int,
                                  const int,        const int, const int);
    void draw_object_with_numbers   (const GraIt&,  const int, const int);
    void draw_object_with_numbers_at(const GraIt&,  const int, const int,
                                                    const int, const int);
    // Kopierverbot
    Editor(const Editor&);
    void operator = (const Editor&);



public:

    enum PanNr {
        FILE_NEW,
        FILE_EXIT,
        FILE_SAVE,
        FILE_SAVE_AS,
        GRID_2,
        GRID_CUSTOM,
        GRID_16,
        SELECT_ALL,
        SELECT_FRAME,
        SELECT_ADD,
        SELECT_COPY,
        SELECT_DELETE,
        SELECT_MINUS,
        SELECT_PLUS,
        SELECT_BACK,
        SELECT_FRONT,
        SELECT_FLIP,
        SELECT_ROTATE,
        SELECT_DARK,
        SELECT_NOOW,
        VIEW_ZOOM,
        HELP,
        MENU_SIZE,
        MENU_SCROLL,
        MENU_VARS,
        MENU_SKILL,
        ADD_TERRAIN,
        ADD_STEEL,
        ADD_HATCH,
        ADD_GOAL,
        ADD_DECO,
        ADD_HAZARD,
        MAX
    };

    Editor(); // Geladen wird immer der letzte Einzelspielerlevel, falls vorh.
    ~Editor();

    inline bool get_exit() { return exit; }

    void undraw();
    void calc();
    void draw();



// ############################################################################
// ################################################################## Submenues
// ############################################################################



private:

    int                   browser_terrain_last_page;

    Api::SaveBrowser*     browser_save;
    Api::BitmapBrowser*   browser_bitmap;
    Api::WindowSize*      window_size;
    Api::WindowScroll*    window_scroll;
    Api::WindowVariables* window_variable;
    Api::WindowSkill*     window_skill;
    Api::BoxMessage*      box_unsaved_data;

    // Funktionen
    void calc_bitmap_browser(Filename&); // which Last Dir string

    static Api::BoxMessage* new_box_unsaved_data(const Level&);

    static bool search_criterion_terrain(const Filename&);
    static bool search_criterion_steel  (const Filename&);
    static bool search_criterion_hatch  (const Filename&);
    static bool search_criterion_goal   (const Filename&);
    static bool search_criterion_deco   (const Filename&);
    static bool search_criterion_hazard (const Filename&);

};
