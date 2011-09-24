/*
 * other/user.cpp
 *
 */

#include "myalleg.h" // KEY_*
#include "user.h"
#include "language.h"
#include "io.h"
#include "help.h"

#include "../lix/lix_enum.h" // LixEn::AC_MAX
#include "../network/net_t.h" // PlayerData::name_max_length

Result::Result()
:
    built         (gloB->empty_string),
    lix_saved     (0),
    skills_used   (0),
    updates_used  (0)
{
}

Result::Result(const Date& d, const int ls, const int su, const int uu)
:
    built         (d),
    lix_saved     (ls),
    skills_used   (su),
    updates_used  (uu)
{
}

Result::~Result()
{
}



bool Result::operator == (const Result& r) const
{
    return built       == r.built
     && lix_saved      == r.lix_saved
     && skills_used    == r.skills_used
     && updates_used   == r.updates_used;
}

bool Result::operator != (const Result& r) const
{
    return ! (*this == r);
}

bool Result::operator < (const Result& r) const
{
    // Wir muessen nicht nur updates_used > r.updates_used kontrollieren, um
    // festzustellen, ob r.updates_used besser ist, sondern r.updates_used
    // darf auch nicht null sein. Das ist der (im normalen Spiel unerreichbare)
    // Update-Wert eines standardkonstruierten Result-Objektes.
    return built      != r.built          ? built          < r.built
     : lix_saved != r.lix_saved ? lix_saved < r.lix_saved
     : skills_used    != r.skills_used    ? skills_used    > r.skills_used
     : updates_used   != r.updates_used   ? updates_used   > r.updates_used
                                            && r.updates_used > 0
     : false; // alle gleich
}

bool Result::operator > (const Result& r) const
{
    return *this != r && ! (*this < r);
}



// ############################################################################
// ############################################################################
// ############################################################################



User* useR(0);



void User::initialize()
{
    if (!useR) useR = new User();
}



void User::deinitialize()
{
    if (useR) {
        delete useR;
        useR = 0;
    }
}



User::User()
:
    language               (Language::NONE),
    option_group           (0),

    scroll_edge            (true),
    scroll_right           (true),
    scroll_middle          (true),
    mouse_speed            (15),
    scroll_speed_edge      (5),
    scroll_speed_click     (6),
    scroll_torus_x         (2),
    scroll_torus_y         (2),
    multiple_builders      (true),
    batter_priority        (true),
    prioinv_middle         (true),
    prioinv_right          (true),

    screen_scaling         (2),
    screen_border_colored  (true),
    screen_windowed        (false),
    arrows_replay          (true),
    arrows_network         (true),
    debris_amount          (2),
    debris_type            (1),
    gui_color_red          (0x8C),
    gui_color_green        (0x80),
    gui_color_blue         (0xA0),

    sound_volume           (10),

    editor_hex_level_size (false),
    editor_grid_selected  (1),
    editor_grid_custom    (8),

    key_skill              (LixEn::AC_MAX, 0),

    single_last_level      (gloB->dir_levels_single),
    network_last_level     (gloB->dir_levels_network),
    replay_last_level      (gloB->dir_replay),
    network_last_style     (0),

    editor_last_dir_terrain(gloB->dir_bitmap),
    editor_last_dir_steel  (gloB->dir_bitmap),
    editor_last_dir_hatch  (gloB->dir_bitmap),
    editor_last_dir_goal   (gloB->dir_bitmap),
    editor_last_dir_deco   (gloB->dir_bitmap),
    editor_last_dir_hazard (gloB->dir_bitmap)
{
    key_skill[LixEn::WALKER]     = KEY_W;
    key_skill[LixEn::RUNNER]     = KEY_LSHIFT;
    key_skill[LixEn::BASHER]     = KEY_E;
    key_skill[LixEn::BUILDER]    = KEY_R;
    key_skill[LixEn::PLATFORMER] = KEY_T;
    key_skill[LixEn::DIGGER]     = KEY_G;
    key_skill[LixEn::MINER]      = KEY_D;
    key_skill[LixEn::BLOCKER]    = KEY_V;
    key_skill[LixEn::CUBER]      = KEY_V;
    key_skill[LixEn::EXPLODER]   = KEY_X;
    key_skill[LixEn::EXPLODER2]  = KEY_X;

    key_skill[LixEn::CLIMBER]    = KEY_B;
    key_skill[LixEn::FLOATER]    = KEY_Q;
    key_skill[LixEn::JUMPER]     = KEY_A;
    key_skill[LixEn::ROPER]      = KEY_C;
    key_skill[LixEn::BATTER]     = KEY_C;

    key_force_left             = KEY_S;
    key_force_right            = KEY_F;
    key_rate_minus             = KEY_1;
    key_rate_plus              = KEY_2;
    key_pause                  = KEY_SPACE;
    key_speed_slow             = KEY_3;
    key_speed_fast             = KEY_4;
    key_speed_turbo            = KEY_5;
    key_restart                = KEY_F1;
    key_state_load             = KEY_F2;
    key_state_save             = KEY_F5;
    key_zoom                   = KEY_Y;
    key_nuke                   = KEY_F12;
    key_spec_tribe             = KEY_TAB;
    key_chat                   = KEY_ENTER;

    key_ed_left                = KEY_S;
    key_ed_right               = KEY_F;
    key_ed_up                  = KEY_E;
    key_ed_down                = KEY_D;
    key_ed_copy                = KEY_A;
    key_ed_delete              = KEY_X;
    key_ed_grid                = KEY_C;
    key_ed_sel_all             = KEY_ALT;
    key_ed_sel_frame           = KEY_LSHIFT;
    key_ed_sel_add             = KEY_V;
    key_ed_background          = KEY_T;
    key_ed_foreground          = KEY_G;
    key_ed_mirror              = KEY_W;
    key_ed_rotate              = KEY_R;
    key_ed_dark                = KEY_B;
    key_ed_noow                = KEY_N;
    key_ed_zoom                = KEY_Y;
    key_ed_help                = KEY_H;
    key_ed_menu_size           = KEY_5;
    key_ed_menu_vars           = KEY_Q;
    key_ed_menu_skills         = KEY_Z;
    key_ed_add_terrain         = KEY_SPACE;
    key_ed_add_steel           = KEY_TAB;
    key_ed_add_hatch           = KEY_1;
    key_ed_add_goal            = KEY_2;
    key_ed_add_deco            = KEY_3;
    key_ed_add_hazard          = KEY_4;
}



User::~User()
{
}



const Result* User::get_level_result(const Filename& filename) const
{
    std::map <Filename, Result>::const_iterator itr = result.find(filename);
    if (itr == result.end()) return 0;
    else return &itr->second;
}



void User::set_level_result(const Filename& filename, const Result& r)
{
    Result& saved_result = result[filename];
    if (saved_result < r) saved_result = r;
}



void User::load()
{
    result.clear();

    if ((int) gloB->user_name.size() > PlayerData::name_max_length)
     gloB->user_name.resize(PlayerData::name_max_length);

    Filename filename(gloB->dir_data_user.get_dir_rootful()
                      + gloB->user_name + gloB->ext_level);
    std::vector <IO::Line> lines;
    IO::fill_vector_from_file(lines, filename.get_rootful());

    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) switch(i->type) {

    case '$':
        if      (i->text1 == gloB->user_single_last_level      ) single_last_level  = Filename(i->text2);
        else if (i->text1 == gloB->user_network_last_level     ) network_last_level = Filename(i->text2);
        else if (i->text1 == gloB->user_replay_last_level      ) replay_last_level  = Filename(i->text2);

        else if (i->text1 == gloB->user_editor_last_dir_terrain) editor_last_dir_terrain = Filename(i->text2);
        else if (i->text1 == gloB->user_editor_last_dir_steel  ) editor_last_dir_steel   = Filename(i->text2);
        else if (i->text1 == gloB->user_editor_last_dir_hatch  ) editor_last_dir_hatch   = Filename(i->text2);
        else if (i->text1 == gloB->user_editor_last_dir_goal   ) editor_last_dir_goal    = Filename(i->text2);
        else if (i->text1 == gloB->user_editor_last_dir_deco   ) editor_last_dir_deco    = Filename(i->text2);
        else if (i->text1 == gloB->user_editor_last_dir_hazard ) editor_last_dir_hazard  = Filename(i->text2);
        break;

    case '#':
        if      (i->text1 == gloB->user_language               ) {
            language               = i->nr1;
        }
        else if (i->text1 == gloB->user_option_group           ) option_group           = i->nr1;

        else if (i->text1 == gloB->user_mouse_speed            ) mouse_speed            = i->nr1;
        else if (i->text1 == gloB->user_scroll_speed_edge      ) scroll_speed_edge      = i->nr1;
        else if (i->text1 == gloB->user_scroll_speed_click     ) scroll_speed_click     = i->nr1;
        else if (i->text1 == gloB->user_scroll_edge            ) scroll_edge            = i->nr1;
        else if (i->text1 == gloB->user_scroll_right           ) scroll_right           = i->nr1;
        else if (i->text1 == gloB->user_scroll_middle          ) scroll_middle          = i->nr1;
        else if (i->text1 == gloB->user_scroll_torus_x         ) scroll_torus_x         = i->nr1;
        else if (i->text1 == gloB->user_scroll_torus_y         ) scroll_torus_y         = i->nr1;
        else if (i->text1 == gloB->user_multiple_builders      ) multiple_builders      = i->nr1;
        else if (i->text1 == gloB->user_batter_priority        ) batter_priority        = i->nr1;
        else if (i->text1 == gloB->user_prioinv_middle         ) prioinv_middle         = i->nr1;
        else if (i->text1 == gloB->user_prioinv_right          ) prioinv_right          = i->nr1;

        else if (i->text1 == gloB->user_screen_scaling         ) screen_scaling         = i->nr1;
        else if (i->text1 == gloB->user_screen_border_colored  ) screen_border_colored  = i->nr1;
        else if (i->text1 == gloB->user_screen_windowed        ) screen_windowed        = i->nr1;
        else if (i->text1 == gloB->user_arrows_replay          ) arrows_replay          = i->nr1;
        else if (i->text1 == gloB->user_arrows_network         ) arrows_network         = i->nr1;
        else if (i->text1 == gloB->user_debris_amount          ) debris_amount          = i->nr1;
        else if (i->text1 == gloB->user_debris_type            ) debris_type            = i->nr1;
        else if (i->text1 == gloB->user_gui_color_red          ) gui_color_red          = i->nr1;
        else if (i->text1 == gloB->user_gui_color_green        ) gui_color_green        = i->nr1;
        else if (i->text1 == gloB->user_gui_color_blue         ) gui_color_blue         = i->nr1;

        else if (i->text1 == gloB->user_sound_volume           ) sound_volume           = i->nr1;

        else if (i->text1 == gloB->user_editor_hex_level_size  ) editor_hex_level_size  = i->nr1;
        else if (i->text1 == gloB->user_editor_grid_selected   ) editor_grid_selected   = i->nr1;
        else if (i->text1 == gloB->user_editor_grid_custom     ) editor_grid_custom     = i->nr1;

        else if (i->text1 == gloB->user_network_last_style     ) network_last_style     = i->nr1;

        else if (i->text1 == gloB->user_key_force_left         ) key_force_left         = i->nr1;
        else if (i->text1 == gloB->user_key_force_right        ) key_force_right        = i->nr1;
        else if (i->text1 == gloB->user_key_rate_minus         ) key_rate_minus         = i->nr1;
        else if (i->text1 == gloB->user_key_rate_plus          ) key_rate_plus          = i->nr1;
        else if (i->text1 == gloB->user_key_pause              ) key_pause              = i->nr1;
        else if (i->text1 == gloB->user_key_speed_slow         ) key_speed_slow         = i->nr1;
        else if (i->text1 == gloB->user_key_speed_fast         ) key_speed_fast         = i->nr1;
        else if (i->text1 == gloB->user_key_speed_turbo        ) key_speed_turbo        = i->nr1;
        else if (i->text1 == gloB->user_key_restart            ) key_restart            = i->nr1;
        else if (i->text1 == gloB->user_key_state_load         ) key_state_load         = i->nr1;
        else if (i->text1 == gloB->user_key_state_save         ) key_state_save         = i->nr1;
        else if (i->text1 == gloB->user_key_zoom               ) key_zoom               = i->nr1;
        else if (i->text1 == gloB->user_key_nuke               ) key_nuke               = i->nr1;
        else if (i->text1 == gloB->user_key_spec_tribe         ) key_spec_tribe         = i->nr1;
        else if (i->text1 == gloB->user_key_chat               ) key_chat               = i->nr1;

        else if (i->text1 == gloB->user_key_ed_left            ) key_ed_left            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_right           ) key_ed_right           = i->nr1;
        else if (i->text1 == gloB->user_key_ed_up              ) key_ed_up              = i->nr1;
        else if (i->text1 == gloB->user_key_ed_down            ) key_ed_down            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_copy            ) key_ed_copy            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_delete          ) key_ed_delete          = i->nr1;
        else if (i->text1 == gloB->user_key_ed_grid            ) key_ed_grid            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_sel_all         ) key_ed_sel_all         = i->nr1;
        else if (i->text1 == gloB->user_key_ed_sel_frame       ) key_ed_sel_frame       = i->nr1;
        else if (i->text1 == gloB->user_key_ed_sel_add         ) key_ed_sel_add         = i->nr1;
        else if (i->text1 == gloB->user_key_ed_background      ) key_ed_background      = i->nr1;
        else if (i->text1 == gloB->user_key_ed_foreground      ) key_ed_foreground      = i->nr1;
        else if (i->text1 == gloB->user_key_ed_mirror          ) key_ed_mirror          = i->nr1;
        else if (i->text1 == gloB->user_key_ed_rotate          ) key_ed_rotate          = i->nr1;
        else if (i->text1 == gloB->user_key_ed_dark            ) key_ed_dark            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_noow            ) key_ed_noow            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_zoom            ) key_ed_zoom            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_help            ) key_ed_help            = i->nr1;
        else if (i->text1 == gloB->user_key_ed_menu_size       ) key_ed_menu_size       = i->nr1;
        else if (i->text1 == gloB->user_key_ed_menu_vars       ) key_ed_menu_vars       = i->nr1;
        else if (i->text1 == gloB->user_key_ed_menu_skills     ) key_ed_menu_skills     = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_terrain     ) key_ed_add_terrain     = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_steel       ) key_ed_add_steel       = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_hatch       ) key_ed_add_hatch       = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_goal        ) key_ed_add_goal        = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_deco        ) key_ed_add_deco        = i->nr1;
        else if (i->text1 == gloB->user_key_ed_add_hazard      ) key_ed_add_hazard      = i->nr1;

        else {
            LixEn::Ac ac = LixEn::string_to_ac(i->text1);
            if (ac != LixEn::AC_MAX) key_skill[ac] = i->nr1;
        }
        break;

    case '<': {
        Result result_read(i->text2, i->nr1, i->nr2, i->nr3);
        // Wir nutzen hier, dass ein standardkontruiertes Datenobjekt immer
        // kleiner ist als die geladenen Daten. map::operator[] fuegt ja ein
        // neues standardkonstruiertes Objekt ein, wenn noch kein Eintrag mit
        // dem uebergebenen Schluessel existiert.
        Filename fn(i->text1);
        if (result[fn] < result_read) result[fn] = result_read;
        break; }

    default:
        break;

    }
    Language::set(language);
}



void User::save() const
{
    std::string filename = gloB->dir_data_user.get_rootful()
                         + gloB->user_name + gloB->ext_level;
    std::ofstream file(filename.c_str());

    file
     << IO::LineHash  (gloB->user_language,                language)
     << IO::LineHash  (gloB->user_option_group,            option_group)
     << std::endl

     << IO::LineHash  (gloB->user_mouse_speed,             mouse_speed)
     << IO::LineHash  (gloB->user_scroll_speed_edge,       scroll_speed_edge)
     << IO::LineHash  (gloB->user_scroll_speed_click,      scroll_speed_click)
     << IO::LineHash  (gloB->user_scroll_edge,             scroll_edge)
     << IO::LineHash  (gloB->user_scroll_right,            scroll_right)
     << IO::LineHash  (gloB->user_scroll_middle,           scroll_middle)
     << IO::LineHash  (gloB->user_scroll_torus_x,          scroll_torus_x)
     << IO::LineHash  (gloB->user_scroll_torus_y,          scroll_torus_y)
     << IO::LineHash  (gloB->user_multiple_builders,       multiple_builders)
     << IO::LineHash  (gloB->user_batter_priority,         batter_priority)
     << IO::LineHash  (gloB->user_prioinv_middle,          prioinv_middle)
     << IO::LineHash  (gloB->user_prioinv_right,           prioinv_right)
     << std::endl

     << IO::LineHash  (gloB->user_screen_scaling,          screen_scaling)
     << IO::LineHash  (gloB->user_screen_border_colored,   screen_border_colored)
     << IO::LineHash  (gloB->user_screen_windowed,         screen_windowed)
     << IO::LineHash  (gloB->user_arrows_replay,           arrows_replay)
     << IO::LineHash  (gloB->user_arrows_network,          arrows_network)
     << IO::LineHash  (gloB->user_debris_amount,           debris_amount)
     << IO::LineHash  (gloB->user_debris_type,             debris_type)
     << IO::LineHash  (gloB->user_gui_color_red,           gui_color_red)
     << IO::LineHash  (gloB->user_gui_color_green,         gui_color_green)
     << IO::LineHash  (gloB->user_gui_color_blue,          gui_color_blue)
     << std::endl

     << IO::LineHash  (gloB->user_sound_volume,            sound_volume)
     << std::endl

     << IO::LineHash  (gloB->user_editor_hex_level_size,   editor_hex_level_size)
     << IO::LineHash  (gloB->user_editor_grid_selected,    editor_grid_selected)
     << IO::LineHash  (gloB->user_editor_grid_custom,      editor_grid_custom)
     << std::endl

     << IO::LineDollar(gloB->user_single_last_level,       single_last_level.get_rootless())
     << IO::LineDollar(gloB->user_network_last_level,      network_last_level.get_rootless())
     << IO::LineDollar(gloB->user_replay_last_level,       replay_last_level.get_rootless())
     << IO::LineHash  (gloB->user_network_last_style,      network_last_style)
     << std::endl

     << IO::LineDollar(gloB->user_editor_last_dir_terrain, editor_last_dir_terrain.get_rootless())
     << IO::LineDollar(gloB->user_editor_last_dir_steel,   editor_last_dir_steel.get_rootless())
     << IO::LineDollar(gloB->user_editor_last_dir_hatch,   editor_last_dir_hatch.get_rootless())
     << IO::LineDollar(gloB->user_editor_last_dir_goal,    editor_last_dir_goal.get_rootless())
     << IO::LineDollar(gloB->user_editor_last_dir_deco,    editor_last_dir_deco.get_rootless())
     << IO::LineDollar(gloB->user_editor_last_dir_hazard,  editor_last_dir_hazard.get_rootless())
     << std::endl

     << IO::LineHash  (gloB->user_key_force_left,  key_force_left)
     << IO::LineHash  (gloB->user_key_force_right, key_force_right)
     << IO::LineHash  (gloB->user_key_rate_minus,  key_rate_minus)
     << IO::LineHash  (gloB->user_key_rate_plus,   key_rate_plus)
     << IO::LineHash  (gloB->user_key_pause,       key_pause)
     << IO::LineHash  (gloB->user_key_speed_slow,  key_speed_slow)
     << IO::LineHash  (gloB->user_key_speed_fast,  key_speed_fast)
     << IO::LineHash  (gloB->user_key_speed_turbo, key_speed_turbo)
     << IO::LineHash  (gloB->user_key_restart,     key_restart)
     << IO::LineHash  (gloB->user_key_state_load,  key_state_load)
     << IO::LineHash  (gloB->user_key_state_save,  key_state_save)
     << IO::LineHash  (gloB->user_key_zoom,        key_zoom)
     << IO::LineHash  (gloB->user_key_nuke,        key_nuke)
     << IO::LineHash  (gloB->user_key_spec_tribe,  key_spec_tribe)
     << IO::LineHash  (gloB->user_key_chat,        key_chat);
    for (size_t i = 0; i < key_skill.size(); ++i)
     if (key_skill[i] != 0) file << IO::LineHash(
     LixEn::ac_to_string(static_cast <LixEn::Ac> (i)), key_skill[i]);

    file
     << std::endl
     << IO::LineHash  (gloB->user_key_ed_left,        key_ed_left)
     << IO::LineHash  (gloB->user_key_ed_right,       key_ed_right)
     << IO::LineHash  (gloB->user_key_ed_up,          key_ed_up)
     << IO::LineHash  (gloB->user_key_ed_down,        key_ed_down)
     << IO::LineHash  (gloB->user_key_ed_copy,        key_ed_copy)
     << IO::LineHash  (gloB->user_key_ed_delete,      key_ed_delete)
     << IO::LineHash  (gloB->user_key_ed_grid,        key_ed_grid)
     << IO::LineHash  (gloB->user_key_ed_sel_all,     key_ed_sel_all)
     << IO::LineHash  (gloB->user_key_ed_sel_frame,   key_ed_sel_frame)
     << IO::LineHash  (gloB->user_key_ed_sel_add,     key_ed_sel_add)
     << IO::LineHash  (gloB->user_key_ed_background,  key_ed_background)
     << IO::LineHash  (gloB->user_key_ed_foreground,  key_ed_foreground)
     << IO::LineHash  (gloB->user_key_ed_mirror,      key_ed_mirror)
     << IO::LineHash  (gloB->user_key_ed_rotate,      key_ed_rotate)
     << IO::LineHash  (gloB->user_key_ed_dark,        key_ed_dark)
     << IO::LineHash  (gloB->user_key_ed_noow,        key_ed_noow)
     << IO::LineHash  (gloB->user_key_ed_zoom,        key_ed_zoom)
     << IO::LineHash  (gloB->user_key_ed_help,        key_ed_help)
     << IO::LineHash  (gloB->user_key_ed_menu_size,   key_ed_menu_size)
     << IO::LineHash  (gloB->user_key_ed_menu_vars,   key_ed_menu_vars)
     << IO::LineHash  (gloB->user_key_ed_menu_skills, key_ed_menu_skills)
     << IO::LineHash  (gloB->user_key_ed_add_terrain, key_ed_add_terrain)
     << IO::LineHash  (gloB->user_key_ed_add_steel,   key_ed_add_steel)
     << IO::LineHash  (gloB->user_key_ed_add_hatch,   key_ed_add_hatch)
     << IO::LineHash  (gloB->user_key_ed_add_goal,    key_ed_add_goal)
     << IO::LineHash  (gloB->user_key_ed_add_deco,    key_ed_add_deco)
     << IO::LineHash  (gloB->user_key_ed_add_hazard,  key_ed_add_hazard);

    if (!result.empty()) file << std::endl;

    for (std::map <Filename, Result> ::const_iterator itr = result.begin();
     itr != result.end(); ++itr) {
        std::string bstr;
        bstr << itr->second.built;
        file << IO::LineAngle(itr->first.get_rootless(),
                itr->second.lix_saved,
                itr->second.skills_used,
                itr->second.updates_used, bstr);
    }

    file.close();
}
