
/* █▀ ▀█▀ █▀▀ █▀█ █▀█ █▀▀ █▀█    █▀▄▀█ █▀█ ▀█▀ █▀█ █▀█    █▀▄▀█ █░█ █▀ █ █▀▀ */
/* ▄█ ░█░ ██▄ █▀▀ █▀▀ ██▄ █▀▄    █░▀░█ █▄█ ░█░ █▄█ █▀▄    █░▀░█ █▄█ ▄█ █ █▄▄ */

#include "midifile.h"
#define MIDI_LEN (midifile_len)
#define MIDI_ARR (midifile)

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <rom/ets_sys.h>
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "soc/gpio_reg.h"
#include "esp_log.h"

#include "midi.h"
#include "stepper.h"
#include "solenoid.h"

#define LOW  0
#define HIGH 1
#define TEMPO_BYTE_LENGTH 3
#define DEBUG_LED GPIO_NUM_2
#define ONE_SECOND 1000000

gptimer_handle_t parser_timer = NULL;



void app_main()
{
  Stepper_Init();
  Solenoid_Init();

  // Initialize Midi parser
  struct midi_parser * parser = malloc(sizeof(struct midi_parser));
  parser->state = MIDI_PARSER_INIT;
  parser->size  = MIDI_LEN;
  parser->in    = MIDI_ARR;
  enum midi_parser_status status;

  uint8_t  is_first_event = true;
  uint16_t mticks_til_next_event = 0;
  uint32_t usec_per_qtr   = 0; // modified in set_tempo meta event
  uint16_t usec_per_mtick = 0; // calculated from usec_per_qtr and time division
  midi_midi_event previous_event;

  ets_delay_us(ONE_SECOND);

  while (1)
  {
    status = midi_parse(parser);
    switch (status) {
      case MIDI_PARSER_TRACK_MIDI:
        // Initialize the saved event if needed
        if (is_first_event) {
          previous_event = parser->midi;
          is_first_event = false;
          break;
        }

        // Handle the stored last event
        if (previous_event.status == MIDI_STATUS_NOTE_ON) {
          Stepper_NoteOn(previous_event);
        } else if (previous_event.status == MIDI_STATUS_NOTE_OFF) {
          Stepper_NoteOff(previous_event.param1);
        }

        // Set delay time til next event
        mticks_til_next_event = parser->vtime;

        // Store current event
        previous_event = parser->midi;
        break;

      case MIDI_PARSER_TRACK_META:
        if (parser->meta.type == MIDI_META_SET_TEMPO) {
          // Read next 3 bytes to get tempo (usec per qtr note)
          usec_per_qtr = 0;
          for (int i = 0; i < TEMPO_BYTE_LENGTH; i++) {
            usec_per_qtr <<= 8;
            usec_per_qtr += parser->meta.bytes[i];
          }
          if (usec_per_qtr == 0) break;
          usec_per_mtick = usec_per_qtr / parser->header.time_division;
        }
        break;

      case MIDI_PARSER_EOB:
        Stepper_AllOff();
        return;
        break;

      default: break;
    }

    uint32_t usec_delay = mticks_til_next_event * usec_per_mtick;
    ets_delay_us(usec_delay);
  }
}
