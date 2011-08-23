#include "window.h"
#include "manager.h" // set_exit removes focus

namespace Api {

const int Window::title_bar_height(20);

Window::Window(const int nx, const int ny,
               const unsigned xl, const unsigned yl,
               const std::string& tit)
:
    Element(nx, ny, xl, yl),
    title(tit),
    subtitle(),
    label_title(xl/2, 0, tit, Label::CENTERED)
{
    exit = false;

    // Loeschen beim Entzeichnen

    set_undraw_color(color[COL_PINK]);
    label_title.set_color       (color[COL_WHITE]);
    label_title.set_undraw_color(color[COL_API_ON_M]);
    add_child(label_title);

    // Minimalbreite und -länge berücksichtigen
    if (get_xl() < 10)                  set_xl(10);
    if (get_yl() < title_bar_height+10) set_yl(title_bar_height+10);
}



Window::~Window()
{
}



void Window::set_title(const std::string& s)
{
    title = s;
    label_title.set_text(subtitle.empty() ? title : title + " - " + subtitle);
    // the lobby does hide_all_children. This is a bit
    // kludgy, but it's fair to always show this when set.
    label_title.set_undraw_color(color[COL_API_ON_M]);
    label_title.show();
}

void Window::set_subtitle(const std::string& s)
{
    subtitle = s;
    label_title.set_text(subtitle.empty() ? title : title + " - " + subtitle);
    label_title.set_undraw_color(color[COL_API_ON_M]);
    label_title.show();
}



void Window::set_exit()
{
    exit = true;
    Manager::remove_focus(this);
}



void Window::draw_self()
{
    // Now draw anything other than the title
    BITMAP*   g     = get_ground().get_al_bitmap();
    const int col_1 = color[COL_API_L];
    const int col_2 = color[COL_API_M];
    const int col_3 = color[COL_API_D];
    const int x1    = get_x_here();
    const int y1    = get_y_here() + title_bar_height;
    const int x2    = get_x_here() + get_xl() - 1; // -1, denn die Länge zählt
    const int y2    = get_y_here() + get_yl() - 1; // Pixel an Position 0 mit

    // Fenster malen
    acquire_bitmap(g);

    putpixel(g, x1,   y2,               col_2); // Unten links außen
    putpixel(g, x1+1, y2-1,             col_2); // Unten links innen
    rectfill(g, x1+2, y1,   x2-2, y2-2, col_2); // Mittelfläche
    vline   (g, x1,   y1,         y2-1, col_1); // Links  außen
    vline   (g, x1+1, y1,         y2-2, col_1); // Links  innen
    vline   (g, x2,   y1,         y2,   col_3); // Rechts außen
    vline   (g, x2-1, y1,         y2,   col_3); // Rechts innen
    hline   (g, x1+1, y2,   x2-2,       col_3); // Unten  außen
    hline   (g, x1+2, y2-1, x2-2,       col_3); // Unten  innen

    release_bitmap(g);

    draw_title();
}



void Window::draw_title()
{
    // Variablen fuer die Titelflaeche
    BITMAP *g       = get_ground().get_al_bitmap();
    const int col_1 = color[COL_API_ON_L];
    const int col_2 = color[COL_API_ON_M];
    const int col_3 = color[COL_API_ON_D];
    const int x1    = get_x_here();
    const int y1    = get_y_here();
    const int x2    = get_x_here() + get_xl() - 1; // -1, denn Länge zählt
    const int y2    = get_y_here() + title_bar_height - 1;

    // Jetzt wird's ernst
    acquire_bitmap(g);

    // Titel malen
    putpixel(g, x1,   y2,               col_2); // Unten links außen
    putpixel(g, x1+1, y2-1,             col_2); // Unten links innen
    putpixel(g, x2-1, y1+1,             col_2); // Oben rechts außen
    putpixel(g, x2,   y1,               col_2); // Oben rechts innen
    rectfill(g, x1+2, y1+2, x2-2, y2-2, col_2); // Mittelfläche
    vline   (g, x1,   y1,         y2-1, col_1); // Links  außen
    vline   (g, x1+1, y1,         y2-2, col_1); // Links  innen
    hline   (g, x1+2, y1,   x2-1,       col_1); // Oben   außen
    hline   (g, x1+2, y1+1, x2-2,       col_1); // Oben   innen
    vline   (g, x2,   y1+1,       y2,   col_3); // Rechts außen
    vline   (g, x2-1, y1+2,       y2,   col_3); // Rechts innen
    hline   (g, x1+1, y2,   x2-2,       col_3); // Unten  außen
    hline   (g, x1+2, y2-1, x2-2,       col_3); // Unten  innen

    release_bitmap(g);
}

}
// Ende von namespace Api
