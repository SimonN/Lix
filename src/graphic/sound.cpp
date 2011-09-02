/*
 * graphic/sound.cpp
 *
 */

#include "sound.h"

#include "../other/globals.h"
#include "../other/user.h"

Sound::Sample::Sample()
:
    filename     (),
    sample       (0),
    unique       (true),
    loud         (false),
    quiet        (false),
    last_was_loud(false)
{
}



Sound::Sample::Sample(const std::string& f)
:
    filename     (f),
    sample       (0),
    unique       (true),
    loud         (false),
    quiet        (false),
    last_was_loud(false)
{
    sample = load_sample(filename.c_str());
}



Sound::Sample::Sample(const Sample& s)
:
    filename     (s.filename),
    sample       (0),
    unique       (s.unique),
    loud         (s.loud),
    quiet        (s.quiet),
    last_was_loud(s.last_was_loud)
{
    sample = load_sample(filename.c_str());
}



Sound::Sample::~Sample()
{
    if (sample) destroy_sample(sample);
}



Sound::Sample& Sound::Sample::operator = (const Sample& s)
{
    if (&s == this) return *this;
    filename      = s.filename;
    unique        = s.unique;
    loud          = s.loud;
    quiet         = s.quiet;
    last_was_loud = s.last_was_loud;

    if (sample) destroy_sample(sample);
    sample = load_sample(filename.c_str());
    return *this;
}



void Sound::Sample::draw()
{
    // Erste Rechnung: 255 ist volle Lautstaerke, 0 - 20 ist unsere Skala.
    // Mittlere 127: In der Mitte des Bildschirms
    // Mittlere 1000: Normale Schnelligkeit
    // Hintere 0: Kein Loop
    if (sample) {
        if (unique && (loud || (quiet && !last_was_loud))) stop_sample(sample);

        if (loud) {
            last_was_loud = true;
            play_sample(sample, useR->sound_volume
             * 256 / 20, 127, 1000, 0);
        }
        else if (quiet) {
            last_was_loud = false;
            play_sample(sample, useR->sound_volume
             *  64 / 20, 127, 1000, 0);
        }
        loud  = false;
        quiet = false;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



Sound* Sound::singleton = 0;

Sound::Sound()
:
    sample(MAX)
{
    #ifdef ALLEGRO_WINDOWS
        // Letzter Parameter 0 macht nix, ist nur notwendige Syntax
        install_sound(DIGI_AUTODETECT, MIDI_NONE, 0);
    #else
        // ALSA emuliert OSS, mit ALSA selbst bzw. AUTODETECT gab's Probleme
        install_sound   (DIGI_ALSA, MIDI_NONE, 0)
        && install_sound(DIGI_OSS,  MIDI_NONE, 0);
    #endif
    // Globale Lautstaerke ist immer dies, die Option veraendert nur die
    // jeweils gespielte Lautstaerke.
    set_volume(225, 0);

    const std::string dds = gloB->dir_data_sound.get_rootful();

    sample[SAVE]        = Sample(dds + "save.wav");

    sample[PANEL]       = Sample(dds + "panel.wav");
    sample[ASSIGN]      = Sample(dds + "assign.wav");
    sample[CLOCK]       = Sample(dds + "clock.wav");

    sample[LETS_GO]     = Sample(dds + "lets_go.wav");
    sample[HATCH_OPEN]  = Sample(dds + "hatch.wav");
    sample[HATCH_CLOSE] = Sample(dds + "hatch.wav");
    sample[OBLIVION]    = Sample(dds + "oblivion.wav");
    sample[FIRE]        = Sample(dds + "fire.wav");
    sample[WATER]       = Sample(dds + "water.wav");
    sample[GOAL]        = Sample(dds + "goal.wav");
    sample[GOAL_BAD]    = Sample(dds + "goal_bad.wav");
    sample[YIPPIE]      = Sample(dds + "yippie.wav");
    sample[OVERTIME]    = Sample(dds + "overtime.wav");

    sample[OUCH]        = Sample(dds + "ouch.wav");
    sample[SPLAT]       = Sample(dds + "splat.wav");
    sample[OHNO]        = Sample(dds + "ohno.wav");
    sample[POP]         = Sample(dds + "pop.wav");
    sample[BRICK]       = Sample(dds + "brick.wav");
    sample[ROPER]       = Sample(dds + "roper.wav");
    sample[STEEL]       = Sample(dds + "steel.wav");
    sample[CLIMBER]     = Sample(dds + "climber.wav");
    sample[BATTER_MISS] = Sample(dds + "bat_miss.wav");
    sample[BATTER_HIT]  = Sample(dds + "bat_hit.wav");

    sample[AWARD_1]     = Sample(dds + "award_1.wav");
    sample[AWARD_2]     = Sample(dds + "award_2.wav");
    sample[AWARD_3]     = Sample(dds + "award_3.wav");
    sample[AWARD_4]     = Sample(dds + "award_4.wav");
}



Sound::~Sound()
{
    sample.clear();
    remove_sound();
}



void Sound::initialize()
{
    if (singleton) return;
    singleton = new Sound();
}



void Sound::deinitialize()
{
    if (!singleton) return;
    delete singleton;
    singleton = 0;
}



void Sound::play_loud(const Id& id)
{
    if (!singleton) return;
    if (id < MAX) singleton->sample[id].set_loud();
}



void Sound::play_quiet(const Id& id)
{
    if (!singleton) return;
    if (id < MAX) singleton->sample[id].set_quiet();
}



void Sound::play_loud_if(const Id& id, const bool loud)
{
    if (!singleton || id >= MAX) return;
    if (loud) singleton->sample[id].set_loud();
    else      singleton->sample[id].set_quiet();
}



void Sound::draw()
{
    if (!singleton) return;
    for (std::vector <Sample> ::iterator
     itr = singleton->sample.begin(); itr != singleton->sample.end(); ++itr)
     itr->draw();
}
