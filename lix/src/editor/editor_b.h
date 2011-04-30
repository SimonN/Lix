/*
 * editor/editor_b.h
 *
 * Das enum mit den Buttons am unteren Spielfeldrand. Wenn dies in der editor.h
 * stuende, muesste naehzu bei einer anderen Aenderung dort das ganze Projekt
 * neu kompiliert werden. Immerhin benutzt language.cpp dieses enum!
 *
 */

#pragma once

class Editor;

enum Editor::PanNr {
    FILE_NEW,
    FILE_EXIT,
    FILE_SAVE,
    FILE_SAVE_AS,
    GRID_2,
    GRID_5,
    GRID_10,
    GRID_25,
    SELECT_FRAME,
    SELECT_ADD,
    SELECT_ALL,
    SELECT_NONE,
    SELECT_COPY,
    SELECT_DELETE,
    SELECT_MINUS,
    SELECT_PLUS,
    VIEW_unused0,
    VIEW_ZOOM,
    VIEW_unused1,
    VIEW_unused2,
    MENU_SIZE,
    MENU_SCROLL,
    MENU_LEMMING,
    MENU_SKILL,
    ADD_TERRAIN,
    ADD_STEEL,
    ADD_HATCH,
    ADD_GOAL,
    ADD_HAZARD,
    ADD_unused0,
    ADD_unused1,
    ADD_unused2,
    MAX
};
