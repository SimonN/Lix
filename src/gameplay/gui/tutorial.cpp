#include "chat.h"

#include "../../other/globals.h"
#include "../../other/user.h"
#include "../../other/help.h" // scancode_to_string
#include "../../lix/lix_enum.h"

static std::string ctrl_keyword_to_key_string(const std::string& t)
{
    int key = 0;

    // ugly code copied from ../../other/user.cpp
    if      (t == gloB->user_key_force_left ) key = useR->key_force_left;
    else if (t == gloB->user_key_force_right) key = useR->key_force_right;
    else if (t == gloB->user_key_scroll     ) key = useR->key_scroll;
    else if (t == gloB->user_key_priority   ) key = useR->key_priority;
    else if (t == gloB->user_key_rate_minus ) key = useR->key_rate_minus;
    else if (t == gloB->user_key_rate_plus  ) key = useR->key_rate_plus;
    else if (t == gloB->user_key_pause      ) key = useR->key_pause;
    else if (t == gloB->user_key_speed_slow ) key = useR->key_speed_slow;
    else if (t == gloB->user_key_speed_fast ) key = useR->key_speed_fast;
    else if (t == gloB->user_key_speed_turbo) key = useR->key_speed_turbo;
    else if (t == gloB->user_key_restart    ) key = useR->key_restart;
    else if (t == gloB->user_key_state_load ) key = useR->key_state_load;
    else if (t == gloB->user_key_state_save ) key = useR->key_state_save;
    else if (t == gloB->user_key_zoom       ) key = useR->key_zoom;
    else if (t == gloB->user_key_nuke       ) key = useR->key_nuke;
    else if (t == gloB->user_key_spec_tribe ) key = useR->key_spec_tribe;
    else if (t == gloB->user_key_chat       ) key = useR->key_chat;
    else if (t == gloB->user_key_ga_exit    ) key = useR->key_ga_exit;

    else {
        LixEn::Ac ac = LixEn::string_to_ac(t);
        if (ac != LixEn::AC_MAX) key = useR->key_skill[ac];
    }
    std::string ret;
    if (key != 0) {
        ret += '[';
        ret += Help::scancode_to_string(key);
        ret += ']';
    }
    else {
        ret += '<';
        ret += t;
        ret += '>';
    }
    return ret;
}


static std::string hint_replace_keywords_except_newlines(const std::string& hi)
{
    std::string ret;
    std::string::const_iterator itr = hi.begin();
    while (itr != hi.end()) {
        // '<' triggers a special command
        if (*itr == '<') {
            std::string esc_word;
            std::string::const_iterator esc = itr;
            // ignore the '<' and start reading chars into the esc_word
            ++esc;
            while (esc != hi.end() && *esc != '>') esc_word += *esc++;
            // we have read up either until the end of the string, or until
            // the delimiting '>', in which case we might do substitution
            if (esc != hi.end()) {
                // we don't linebreak here, that will be done later with the
                // string that is returned from this function
                if (esc_word == "br") ret += "<br>";
                else ret += ctrl_keyword_to_key_string(esc_word);
                itr = esc + 1;
            }
            // if we have read until end of string, don't do anything
            // with the escaped string, just print '<' normally
            else {
                ret += '<';
                ++itr;
            }
        }
        // a normal character that is just displayed as-is
        else {
            ret += *itr;
            ++itr;
        }
    }
    return ret;
}



void GameplayChat::set_hint(const std::string& hi)
{
    set_draw_required();
    hints.clear();
    if (hi.empty()) return;

    const std::string replaced = hint_replace_keywords_except_newlines(hi);

    typedef std::vector <std::string> VecStr;
    VecStr lines;
    Console::break_lines(lines, replaced, font_med, LEMSCR_X - 6);

    for (VecStr::iterator itr = lines.begin(); itr != lines.end(); ++itr) {
        Api::Label lab(3, y_hint_first + y_hint_plus * hints.size());
        hints.push_back(lab);
        hints.back().set_text(*itr);
        hints.back().set_undraw_color(color[COL_PINK]);
    }
}

