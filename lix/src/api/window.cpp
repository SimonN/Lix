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
}

void Window::set_subtitle(const std::string& s)
{
    subtitle = s;
    label_title.set_text(subtitle.empty() ? title : title + " - " + subtitle);
}



void Window::set_exit()
{
    exit = true;
    Manager::remove_focus(this);
}



void Window::draw_self()
{
    // Now draw anything other than the title
    ALLEGRO_BITMAP*   g        = get_ground().get_al_bitmap();
    const ALLEGRO_COLOR& col_1 = color[COL_API_L];
    const ALLEGRO_COLOR& col_2 = color[COL_API_M];
    const ALLEGRO_COLOR& col_3 = color[COL_API_D];
    const int x1               = get_x_here();
    const int y1               = get_y_here() + title_bar_height;
    const int x2               = get_x_here() + get_xl();
    const int y2               = get_y_here() + get_yl();

    // Jetzt wird's ernst
    al_set_target_bitmap    (g);
    al_draw_filled_rectangle(x1+2, y1+2, x2-2, y2-2, col_2); // Mittelfläche
    al_draw_filled_rectangle(x1,   y1,   x1+2, y2-1, col_1); // left
    al_draw_filled_rectangle(x1,   y1,   x2-1, y1+2, col_1); // top
    al_draw_filled_rectangle(x2-2, y1+1, x2,   y2-1, col_3); // right
    al_draw_filled_rectangle(x1+1, y2-2, x2-1, y2,   col_3); // bottom
    al_put_pixel            (x1,   y2-1,             col_2); // lower left
    al_put_pixel            (x1+1, y2-2,             col_2); // lower left
    al_put_pixel            (x2-2, y1+1,             col_2); // upper right
    al_put_pixel            (x2-1, y1,               col_2); // upper right

    draw_title();
}



void Window::draw_title()
{
    // Variablen fuer die Titelflaeche
    ALLEGRO_BITMAP *g          = get_ground().get_al_bitmap();
    const ALLEGRO_COLOR& col_1 = color[COL_API_ON_L];
    const ALLEGRO_COLOR& col_2 = color[COL_API_ON_M];
    const ALLEGRO_COLOR& col_3 = color[COL_API_ON_D];
    const int x1               = get_x_here();
    const int y1               = get_y_here();
    const int x2               = get_x_here() + get_xl();
    const int y2               = get_y_here() + title_bar_height;

    // Jetzt wird's ernst
    al_set_target_bitmap(g);
    al_draw_filled_rectangle(x1+2, y1+2, x2-2, y2-2, col_2); // Mittelfläche
    al_draw_filled_rectangle(x1,   y1,   x1+2, y2-1, col_1); // left
    al_draw_filled_rectangle(x1,   y1,   x2-1, y1+2, col_1); // top
    al_draw_filled_rectangle(x2-2, y1+1, x2,   y2-1, col_3); // right
    al_draw_filled_rectangle(x1+1, y2-2, x2-1, y2,   col_3); // bottom
    al_put_pixel            (x1,   y2-1,             col_2); // lower left
    al_put_pixel            (x1+1, y2-2,             col_2); // lower left
    al_put_pixel            (x2-2, y1+1,             col_2); // upper right
    al_put_pixel            (x2-1, y1,               col_2); // upper right
}

}
// Ende von namespace Api
