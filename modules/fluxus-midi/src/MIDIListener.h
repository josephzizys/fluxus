/*
 Copyright (C) 2008 Gabor Papp

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MIDI_LISTENER__
#define __MIDI_LISTENER__

#include <pthread.h>
#include <vector>
#include <deque>

#include "RtMidi.h"

using namespace std;

#define MAX_CHAN   16
#define MAX_CNTRL (MAX_CHAN*128)

class MIDINote
{
	public:
			MIDINote() { on_off = channel = note = velocity; }
			MIDINote(int on_off, int channel, int note, int velocity);

			int on_off; /**< MIDI_NOTE_ON or MIDI_NOTE_OFF */
			int channel; /**< MIDI channel */
			int note; /**< MIDI note */
			int velocity; /**< velocity of MIDI note */
};

class MIDIEvent
{
	public:
			MIDIEvent() { channel = controller = value; }
			MIDIEvent(int channel, int controller, int value);
			int channel;	/**< MIDI channel (>=0) **/
			int controller;	/*<< MIDI controller number **/
			int value; /**< the actual controller value **/
};

class MIDIListener
{
	public:
			MIDIListener(int port = -1);
			~MIDIListener();

			vector<string> info(void);
			/** callback receiving MIDI data */
			void callback(double deltatime, vector< unsigned char > *message);
			void open(int port); /**< open a MIDI input connection */
			void close(void); /**< close MIDI port */

			int get_cc(int channel, int cntrl_number);
			float get_ccn(int channel, int cntrl_number);
			int get_program(int channel);

			string get_last_event(void);

			MIDINote *get_note(void);
			MIDIEvent *get_cc_event(void);

			enum {
				MIDI_NOTE_OFF = 0x08,
				MIDI_NOTE_ON = 0x09,
				MIDI_CONTROLLER = 0x0b,
				MIDI_PROGRAM_CHANGE = 0x0c,
				MIDI_PITCH_BEND = 0x0e
			};
	private:
			void init_midi(void);
			void add_note(int on_off, int ch, int note, int velocity);
			void add_event(int channel, int controller, int value);

			pthread_mutex_t mutex;

			RtMidiIn *midiin; /**< handler of realtime MIDI input */
			vector<string> port_names; /**< names of MIDI ports */

			string last_event; /**< last midi event as string */

			/** array holding the current state of all, 16*128 controllers */
			unsigned char *cntrl_values;

			/** array holding program number of 16 channels **/
			unsigned char *pgm_values;

			deque<MIDINote *> midi_notes;
			deque<MIDIEvent *> midi_events;
};

#endif

