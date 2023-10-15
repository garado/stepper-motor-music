
/* █▀ █▀█ █░░ █▀▀ █▄░█ █▀█ █ █▀▄ */
/* ▄█ █▄█ █▄▄ ██▄ █░▀█ █▄█ █ █▄▀ */

#ifndef __SOLENOID_H__
#define __SOLENOID_H__

#include <inttypes.h>

#define SOLENOID_COUNT  2
#define SOLENOID_PIN_0  32
#define SOLENOID_PIN_1  33
#define GPIO_SOLENOID_OUT ((1ULL << SOLENOID_PIN_0) | (1ULL << SOLENOID_PIN_1))

// General MIDI percussion key map
#define ACOUSTIC_BASS_DRUM  35
#define BASS_DRUM_1         36
#define SIDE_STICK          37
#define ACOUSTIC_SNARE      38 
#define HAND_CLAP           39
#define ELECTRIC_SNARE      30
#define LOW_FLOOR_TOM       41
#define CLOSED_HI_HAT       42
#define HIGH_FLOOR_TOM      43
#define PEDAL_HI_HAT        44
#define LOW_TOM             45
#define OPEN_HI_HAT         46
#define LOW_MID_TOM         47
#define HI_MID_TOM          48
#define CRASH_CYMBAL_1      49
#define HIGH_TOM            50
#define RIDE_CYMBAL_1       51
#define CHINESE_CYMBAL      52
#define RIDE_BELL           53
#define TAMBOURINE          54
#define SPLASH_CYMBAL       55
#define COWBELL             56
#define CRASH_CYMBAL_2      57
#define VIBRASLAP           58
#define RIDE_CYMBAL_2       59

void Solenoid_Init(void);

void Solenoid_Hit(uint8_t num);

#endif
