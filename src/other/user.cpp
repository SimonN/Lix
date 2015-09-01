/*
 * other/user.cpp
 *
 */

#include <sstream>

#include "myalleg.h" // KEY_*
#include "user.h"
#include "language.h"
#include "help.h"
#include "./file/io.h"

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
     : lix_saved      != r.lix_saved      ? lix_saved      < r.lix_saved
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
    replay_cancel          (true),
    replay_cancel_at       (30),
    mouse_speed            (20),
    mouse_acceleration     (0),
    scroll_speed_edge      (5),
    scroll_speed_click     (6),
    allow_builder_queuing  (true),
    avoid_builder_queuing  (false),
    avoid_batter_to_exploder(false),
    prioinv_middle         (true),
    prioinv_right          (true),

    screen_scaling         (1), // 0 = stretch, 1 = aspect ratio, 2 = integer
    screen_border_colored  (true),
    screen_windowed        (true),
    arrows_replay          (true),
    arrows_network         (true),
    game_show_tooltips     (true),
    game_show_skill_keys   (true),
    debris_amount          (2),
    debris_type            (1),
    gui_color_red          (0x70),
    gui_color_green        (0x80),
    gui_color_blue         (0xA0),

    sound_volume           (10),

    editor_hex_level_size  (false),
    editor_grid_selected   (1),
    editor_grid_custom     (8),

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
    skill_sort.push_back(LixEn::WALKER);
    skill_sort.push_back(LixEn::JUMPER);
    skill_sort.push_back(LixEn::RUNNER);
    skill_sort.push_back(LixEn::CLIMBER);
    skill_sort.push_back(LixEn::FLOATER);
    skill_sort.push_back(LixEn::BATTER);
    skill_sort.push_back(LixEn::EXPLODER2);
    skill_sort.push_back(LixEn::BLOCKER);
    skill_sort.push_back(LixEn::CUBER);
    skill_sort.push_back(LixEn::BUILDER);
    skill_sort.push_back(LixEn::PLATFORMER);
    skill_sort.push_back(LixEn::BASHER);
    skill_sort.push_back(LixEn::MINER);
    skill_sort.push_back(LixEn::DIGGER);

    key_skill[LixEn::WALKER]     = KEY_D;
    key_skill[LixEn::RUNNER]     = KEY_LSHIFT;
    key_skill[LixEn::BASHER]     = KEY_E;
    key_skill[LixEn::BUILDER]    = KEY_A;
    key_skill[LixEn::PLATFORMER] = KEY_T;
    key_skill[LixEn::DIGGER]     = KEY_W;
    key_skill[LixEn::MINER]      = KEY_G;
    key_skill[LixEn::BLOCKER]    = KEY_Z;
    key_skill[LixEn::CUBER]      = KEY_X;
    key_skill[LixEn::EXPLODER]   = KEY_V;
    key_skill[LixEn::EXPLODER2]  = KEY_V;

    key_skill[LixEn::CLIMBER]    = KEY_B;
    key_skill[LixEn::FLOATER]    = KEY_Q;
    key_skill[LixEn::JUMPER]     = KEY_R;
    key_skill[LixEn::BATTER]     = KEY_C;

    key_force_left             = KEY_S;
    key_force_right            = KEY_F;
    key_scroll                 = KEY_MINUS_PAD;
    key_priority               = KEY_MINUS_PAD;
    key_rate_minus             = KEY_F4;
    key_rate_plus              = KEY_F5;
    key_pause                  = KEY_SPACE;
    key_speed_back_one         = KEY_1;
    key_speed_back_many        = KEY_2;
    key_speed_ahead_one        = KEY_3;
    key_speed_ahead_many       = KEY_4;
    key_speed_fast             = KEY_5;
    key_speed_turbo            = KEY_6;
    key_restart                = KEY_F1;
    key_state_load             = KEY_F2;
    key_state_save             = KEY_F3;
    key_zoom                   = KEY_Y;
    key_nuke                   = KEY_F12;
    key_spec_tribe             = KEY_TAB;
    key_chat                   = KEY_ENTER;
    key_ga_exit                = KEY_ESC;

    key_me_okay                = KEY_SPACE;
    key_me_edit                = KEY_F;
    key_me_export              = KEY_R;
    key_me_delete              = KEY_G;
    key_me_up_dir              = KEY_A;
    key_me_up_1                = KEY_S;
    key_me_up_5                = KEY_W;
    key_me_down_1              = KEY_D;
    key_me_down_5              = KEY_E;
    key_me_exit                = KEY_ESC;
    key_me_main_single         = KEY_F;
    key_me_main_network        = KEY_D;
    key_me_main_replay         = KEY_S;
    key_me_main_options        = KEY_A;

    key_ed_left                = KEY_S;
    key_ed_right               = KEY_F;
    key_ed_up                  = KEY_E;
    key_ed_down                = KEY_D;
    key_ed_copy                = KEY_A;
    key_ed_delete              = KEY_G;
    key_ed_grid                = KEY_C;
    key_ed_sel_all             = KEY_ALT;
    key_ed_sel_frame           = KEY_LSHIFT;
    key_ed_sel_add             = KEY_V;
    key_ed_background          = KEY_T;
    key_ed_foreground          = KEY_B;
    key_ed_mirror              = KEY_W;
    key_ed_rotate              = KEY_R;
    key_ed_dark                = KEY_N;
    key_ed_noow                = KEY_M;
    key_ed_zoom                = KEY_Y;
    key_ed_help                = KEY_H;
    key_ed_menu_size           = KEY_5;
    key_ed_menu_vars           = KEY_Q;
    key_ed_menu_skills         = KEY_X;
    key_ed_add_terrain         = KEY_SPACE;
    key_ed_add_steel           = KEY_TAB;
    key_ed_add_hatch           = KEY_1;
    key_ed_add_goal            = KEY_2;
    key_ed_add_deco            = KEY_3;
    key_ed_add_hazard          = KEY_4;
    key_ed_exit                = KEY_ESC;
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



void User::set_level_result_carefully(
    const Filename& filename, const Result& r, const int required
) {
    Result& saved_result = result[filename];
    if (saved_result.built == r.built) {
        // carefully means: if the level build times are the same, use the
        // better result of these two
        if (saved_result < r) saved_result = r;
    }
    else {
        // carefully also means: when the bulid times differ, a non-solving
        // result of a new version is worse than a solving result of the old
        // version. Otherwise, the new version is always preferred.
        // required should be supplied by Gameplay, it's the required count
        // for the new Result r
        if (saved_result.lix_saved >= required && r.lix_saved < required) {
            // keep the old result
        }
        else saved_result = r;
    }
}



void
User::string_to_skill_sort(const std::string& text)
{
    std::istringstream stream(text);
    std::string s;
    size_t pos = 0;
    while (stream >> s && pos < skill_sort.size()) {
        LixEn::Ac ac = LixEn::string_to_ac(s);
        if (ac == LixEn::AC_MAX)
            continue;
        // sort the skills like this:
        // find the specified skill in the vector and swap with vector[pos],
        // then increase pos to be ready for the next loop iteration
        size_t named = pos; // because everything < pos is sorted
        while (named < skill_sort.size() && skill_sort[named] != ac)
            ++named;
        if (named == skill_sort.size())
            continue;
        else {
            skill_sort[named] = skill_sort[pos];
            skill_sort[pos] = ac;
            ++pos;
        }
    }
}




void User::load()
{
    result.clear();

    // note: PlayerData::name_max_length defines the buffer size of
    // (ie. maximum number of bytes) in the network data protocol.
    // Therefore we really need to use size() here rather than
    // number of UTF-8 characters in string.
    if (gloB->user_name.size() > (std::string::size_type)PlayerData::name_max_length) {
        // truncate in a UTF-8 aware manner
        std::string& name = gloB->user_name;
        std::string::const_iterator iter = name.begin() + PlayerData::name_max_length;
        Help::move_iterator_utf8(name, iter, -1);
        std::string::const_iterator iter2 = iter;
        Help::move_iterator_utf8(name, iter2, +1);

        // if iter is right on starting byte of a UTF8-char, then
        // moving backwards will go back to previous UTF8-char. Then
        // going forward again should put us back to right where we
        // started (ie. name_max_length)
        //
        // if iter is in middle of a multi-byte UTF8 sequence, then
        // moving backwards will land it on the starting byte of that
        // sequence.  Then moving forwards will put it past end of
        // sequence and hence beyond name_max_length.
        if (iter2 <= name.begin() + PlayerData::name_max_length) {
            name.resize(iter2 - name.begin());
        } else {
            name.resize(iter - name.begin());
        }
    }

    std::vector <IO::Line> lines;

    // Try loading the correctly escaped filename first
    Filename filename(gloB->dir_data_user.get_dir_rootful()
                      + Help::escape_utf8_with_ascii(gloB->user_name)
                      + gloB->ext_level);
    if (! IO::fill_vector_from_file(lines, filename.get_rootful())) {
        // A return of false from IO::fill... means the file doesn't exist.
        // Legacy support for non-UTF-8 versions before 2015-04:
        // Try loading the unescaped file
        Filename filename_unesc(gloB->dir_data_user.get_dir_rootful()
                                + gloB->user_name + gloB->ext_level);
        IO::fill_vector_from_file(lines, filename_unesc.get_rootful());
    }

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

        else if (i->text1 == gloB->user_sorted_skills          ) string_to_skill_sort(i->text2);
        break;

    case '#':
        if      (i->text1 == gloB->user_language               ) language               = i->nr1;
        else if (i->text1 == gloB->user_option_group           ) option_group           = i->nr1;

        else if (i->text1 == gloB->user_mouse_speed            ) mouse_speed            = i->nr1;
        else if (i->text1 == gloB->user_mouse_acceleration     ) mouse_acceleration     = i->nr1;
        else if (i->text1 == gloB->user_scroll_speed_edge      ) scroll_speed_edge      = i->nr1;
        else if (i->text1 == gloB->user_scroll_speed_click     ) scroll_speed_click     = i->nr1;
        else if (i->text1 == gloB->user_scroll_edge            ) scroll_edge            = i->nr1;
        else if (i->text1 == gloB->user_scroll_right           ) scroll_right           = i->nr1;
        else if (i->text1 == gloB->user_scroll_middle          ) scroll_middle          = i->nr1;
        else if (i->text1 == gloB->user_replay_cancel          ) replay_cancel          = i->nr1;
        else if (i->text1 == gloB->user_replay_cancel_at       ) replay_cancel_at       = i->nr1;
        else if (i->text1 == gloB->user_allow_builder_queuing  ) allow_builder_queuing  = i->nr1;
        else if (i->text1 == gloB->user_avoid_builder_queuing  ) avoid_builder_queuing  = i->nr1;
        else if (i->text1 == gloB->user_avoid_batter_to_exploder)avoid_batter_to_exploder=i->nr1;
        else if (i->text1 == gloB->user_prioinv_middle         ) prioinv_middle         = i->nr1;
        else if (i->text1 == gloB->user_prioinv_right          ) prioinv_right          = i->nr1;

        else if (i->text1 == gloB->user_screen_scaling         ) screen_scaling         = i->nr1;
        else if (i->text1 == gloB->user_screen_border_colored  ) screen_border_colored  = i->nr1;
        else if (i->text1 == gloB->user_screen_windowed        ) screen_windowed        = i->nr1;
        else if (i->text1 == gloB->user_arrows_replay          ) arrows_replay          = i->nr1;
        else if (i->text1 == gloB->user_arrows_network         ) arrows_network         = i->nr1;
        else if (i->text1 == gloB->user_game_show_tooltips     ) game_show_tooltips     = i->nr1;
        else if (i->text1 == gloB->user_game_show_skill_keys   ) game_show_skill_keys   = i->nr1;
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
        else if (i->text1 == gloB->user_key_scroll             ) key_scroll             = i->nr1;
        else if (i->text1 == gloB->user_key_priority           ) key_priority           = i->nr1;
        else if (i->text1 == gloB->user_key_rate_minus         ) key_rate_minus         = i->nr1;
        else if (i->text1 == gloB->user_key_rate_plus          ) key_rate_plus          = i->nr1;
        else if (i->text1 == gloB->user_key_pause              ) key_pause              = i->nr1;
        else if (i->text1 == gloB->user_key_speed_back_one     ) key_speed_back_one     = i->nr1;
        else if (i->text1 == gloB->user_key_speed_back_many    ) key_speed_back_many    = i->nr1;
        else if (i->text1 == gloB->user_key_speed_ahead_one    ) key_speed_ahead_one    = i->nr1;
        else if (i->text1 == gloB->user_key_speed_ahead_many   ) key_speed_ahead_many   = i->nr1;
        else if (i->text1 == gloB->user_key_speed_fast         ) key_speed_fast         = i->nr1;
        else if (i->text1 == gloB->user_key_speed_turbo        ) key_speed_turbo        = i->nr1;
        else if (i->text1 == gloB->user_key_restart            ) key_restart            = i->nr1;
        else if (i->text1 == gloB->user_key_state_load         ) key_state_load         = i->nr1;
        else if (i->text1 == gloB->user_key_state_save         ) key_state_save         = i->nr1;
        else if (i->text1 == gloB->user_key_zoom               ) key_zoom               = i->nr1;
        else if (i->text1 == gloB->user_key_nuke               ) key_nuke               = i->nr1;
        else if (i->text1 == gloB->user_key_spec_tribe         ) key_spec_tribe         = i->nr1;
        else if (i->text1 == gloB->user_key_chat               ) key_chat               = i->nr1;
        else if (i->text1 == gloB->user_key_ga_exit            ) key_ga_exit            = i->nr1;

        else if (i->text1 == gloB->user_key_me_okay            ) key_me_okay            = i->nr1;
        else if (i->text1 == gloB->user_key_me_edit            ) key_me_edit            = i->nr1;
        else if (i->text1 == gloB->user_key_me_export          ) key_me_export          = i->nr1;
        else if (i->text1 == gloB->user_key_me_delete          ) key_me_delete          = i->nr1;
        else if (i->text1 == gloB->user_key_me_up_dir          ) key_me_up_dir          = i->nr1;
        else if (i->text1 == gloB->user_key_me_up_1            ) key_me_up_1            = i->nr1;
        else if (i->text1 == gloB->user_key_me_up_5            ) key_me_up_5            = i->nr1;
        else if (i->text1 == gloB->user_key_me_down_1          ) key_me_down_1          = i->nr1;
        else if (i->text1 == gloB->user_key_me_down_5          ) key_me_down_5          = i->nr1;
        else if (i->text1 == gloB->user_key_me_exit            ) key_me_exit            = i->nr1;
        else if (i->text1 == gloB->user_key_me_main_single     ) key_me_main_single     = i->nr1;
        else if (i->text1 == gloB->user_key_me_main_network    ) key_me_main_network    = i->nr1;
        else if (i->text1 == gloB->user_key_me_main_replay     ) key_me_main_replay     = i->nr1;
        else if (i->text1 == gloB->user_key_me_main_options    ) key_me_main_options    = i->nr1;

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
        else if (i->text1 == gloB->user_key_ed_exit            ) key_ed_exit            = i->nr1;

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
                         + Help::escape_utf8_with_ascii(gloB->user_name)
                         + gloB->ext_level;
    std::ofstream file(filename.c_str());

    file
     << IO::LineHash  (gloB->user_language,                language)
     << IO::LineHash  (gloB->user_option_group,            option_group)
     << std::endl

     << IO::LineHash  (gloB->user_mouse_speed,             mouse_speed)
     << IO::LineHash  (gloB->user_mouse_acceleration,      mouse_acceleration)
     << IO::LineHash  (gloB->user_scroll_speed_edge,       scroll_speed_edge)
     << IO::LineHash  (gloB->user_scroll_speed_click,      scroll_speed_click)
     << IO::LineHash  (gloB->user_scroll_edge,             scroll_edge)
     << IO::LineHash  (gloB->user_scroll_right,            scroll_right)
     << IO::LineHash  (gloB->user_scroll_middle,           scroll_middle)
     << IO::LineHash  (gloB->user_replay_cancel,           replay_cancel)
     << IO::LineHash  (gloB->user_replay_cancel_at,        replay_cancel_at)
     << IO::LineHash  (gloB->user_allow_builder_queuing,   allow_builder_queuing)
     << IO::LineHash  (gloB->user_avoid_builder_queuing,   avoid_builder_queuing)
     << IO::LineHash  (gloB->user_avoid_batter_to_exploder,avoid_batter_to_exploder)
     << IO::LineHash  (gloB->user_prioinv_middle,          prioinv_middle)
     << IO::LineHash  (gloB->user_prioinv_right,           prioinv_right)
     << std::endl

     << IO::LineHash  (gloB->user_screen_scaling,          screen_scaling)
     << IO::LineHash  (gloB->user_screen_border_colored,   screen_border_colored)
     << IO::LineHash  (gloB->user_screen_windowed,         screen_windowed)
     << IO::LineHash  (gloB->user_arrows_replay,           arrows_replay)
     << IO::LineHash  (gloB->user_arrows_network,          arrows_network)
     << IO::LineHash  (gloB->user_game_show_tooltips,      game_show_tooltips)
     << IO::LineHash  (gloB->user_game_show_skill_keys,    game_show_skill_keys)
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
     << IO::LineHash  (gloB->user_key_scroll,      key_scroll)
     << IO::LineHash  (gloB->user_key_priority,    key_priority)
     << IO::LineHash  (gloB->user_key_rate_minus,  key_rate_minus)
     << IO::LineHash  (gloB->user_key_rate_plus,   key_rate_plus)
     << IO::LineHash  (gloB->user_key_pause,       key_pause)
     << IO::LineHash  (gloB->user_key_speed_back_one,   key_speed_back_one)
     << IO::LineHash  (gloB->user_key_speed_back_many,  key_speed_back_many)
     << IO::LineHash  (gloB->user_key_speed_ahead_one,  key_speed_ahead_one)
     << IO::LineHash  (gloB->user_key_speed_ahead_many, key_speed_ahead_many)
     << IO::LineHash  (gloB->user_key_speed_fast,  key_speed_fast)
     << IO::LineHash  (gloB->user_key_speed_turbo, key_speed_turbo)
     << IO::LineHash  (gloB->user_key_restart,     key_restart)
     << IO::LineHash  (gloB->user_key_state_load,  key_state_load)
     << IO::LineHash  (gloB->user_key_state_save,  key_state_save)
     << IO::LineHash  (gloB->user_key_zoom,        key_zoom)
     << IO::LineHash  (gloB->user_key_nuke,        key_nuke)
     << IO::LineHash  (gloB->user_key_spec_tribe,  key_spec_tribe)
     << IO::LineHash  (gloB->user_key_chat,        key_chat)
     << IO::LineHash  (gloB->user_key_ga_exit,     key_ga_exit)
     << std::endl;

    // DEBUGGING: make a user option for it.
    file
     << "// Defining a custom order of the skills in the panel:"
     << std::endl
     << "// The game allows it, but the options menu doesn't support it yet."
     << std::endl
     << "// Edit the following line directly. Don't use EXPLODER, instead use"
     << std::endl
     << "// EXPLODER2 only. It will define the panel for both exploders."
     << std::endl
     << "// To reset: Delete the line, start and exit the game once."
     << std::endl
     << std::endl;

    std::ostringstream skill_sort_stream;
    for (size_t i = 0; i < skill_sort.size(); ++i) {
        if (i != 0) skill_sort_stream << " ";
        skill_sort_stream << ac_to_string(skill_sort[i]);
    }
    file
     << IO::LineDollar(gloB->user_sorted_skills, skill_sort_stream.str())
     << std::endl;

    for (size_t i = 0; i < key_skill.size(); ++i)
     if (key_skill[i] != 0) file << IO::LineHash(
     LixEn::ac_to_string(static_cast <LixEn::Ac> (i)), key_skill[i]);

    file
     << std::endl
     << IO::LineHash  (gloB->user_key_me_okay,          key_me_okay)
     << IO::LineHash  (gloB->user_key_me_edit,          key_me_edit)
     << IO::LineHash  (gloB->user_key_me_export,        key_me_export)
     << IO::LineHash  (gloB->user_key_me_delete,        key_me_delete)
     << IO::LineHash  (gloB->user_key_me_up_dir,        key_me_up_dir)
     << IO::LineHash  (gloB->user_key_me_up_1,          key_me_up_1)
     << IO::LineHash  (gloB->user_key_me_up_5,          key_me_up_5)
     << IO::LineHash  (gloB->user_key_me_down_1,        key_me_down_1)
     << IO::LineHash  (gloB->user_key_me_down_5,        key_me_down_5)
     << IO::LineHash  (gloB->user_key_me_exit,          key_me_exit)
     << IO::LineHash  (gloB->user_key_me_main_single,   key_me_main_single)
     << IO::LineHash  (gloB->user_key_me_main_network,  key_me_main_network)
     << IO::LineHash  (gloB->user_key_me_main_replay,   key_me_main_replay)
     << IO::LineHash  (gloB->user_key_me_main_options,  key_me_main_options)
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
     << IO::LineHash  (gloB->user_key_ed_add_hazard,  key_ed_add_hazard)
     << IO::LineHash  (gloB->user_key_ed_exit,        key_ed_exit);

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
