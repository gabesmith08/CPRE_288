#include "open_interface.h"
#include "music.h"

/// Load three songs onto the iRobot
/**
 * Before calling this method, be sure to initialize the open interface by calling:
 *
 *   oi_t* sensor = oi_alloc();
 *   oi_init(sensor); 
 *
 */


void load_songs(int i) {

	// Notes: oi_load_song takes four arguments
	// arg1 - an integer from 0 to 16 identifying this song
	// arg2 - an integer that indicates the number of notes in the song (if greater than 16, it will consume the next song index's storage space)
	// arg3 - an array of integers representing the midi note values (example: 60 = C, 61 = C sharp)
	// arg4 - an array of integers representing the duration of each note (in 1/64ths of a second)
	// 
	// For a note sheet, see page 12 of the iRobot Creat Open Interface datasheet

    if(i == 0){
        unsigned char rickrollNumNotes = 14;
        unsigned char rickrollNotes[14] = { 71, 66, 71, 78, 71, 78, 81, 80, 76, 71, 0, 71, 73, 74 };
        unsigned char rickrollDuration[14] = { 12, 6, 6, 36, 12, 12, 12, 12, 12, 48, 12, 6, 6, 60 };
        oi_loadSong(RICK_ROLL, rickrollNumNotes, rickrollNotes, rickrollDuration);
        oi_play_song(RICK_ROLL);
    }

    else if(i == 1){
        unsigned char spacenumnotes = 14;
        unsigned char spacenotes[14] = { 71, 74, 78, 76, 71, 66, 71, 78, 71, 78, 81, 80, 76, 71};
        unsigned char spaceduration[14] = {12, 12, 12, 96, 12, 6, 6, 36, 12, 12, 12, 12, 12, 48};
        oi_loadSong(Part2, spacenumnotes, spacenotes, spaceduration);
        oi_play_song(Part2);
    }

    else if(i == 2){
            unsigned char part3numnotes = 4;
            unsigned char part3notes[4] = { 0, 71, 73, 74};
            unsigned char part3duration[14] = {12, 6, 6, 96};
            oi_loadSong(Part3, part3numnotes, part3notes, part3duration);
            oi_play_song(Part3);
   }
//	static uint8_t mario1NumNotes = 49;
//	static uint8_t mario1Notes[49]    = {48, 60, 45, 57, 46, 58,  0, 48, 60, 45, 57, 46, 58,  0, 41, 53, 38, 50, 39, 51,  0, 41, 53, 38, 50, 39, 51,  0, 51, 50, 49, 48, 51, 50, 44, 43, 49, 48, 54, 53, 52, 58, 57, 56, 51, 47, 46, 45, 44 };
//	static uint8_t mario1Duration[49] = {12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 48,  8,  8,  8, 24, 24, 24, 24, 24, 24,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16 };
//	oi_loadSong(MARIO_UNDERWORLD, mario1NumNotes, mario1Notes, mario1Duration);
//
//	static uint8_t mario2NumNotes = 65;
//	static uint8_t mario2Notes[65]    = {50, 52, 54, 55, 57, 58, 59, 59, 59, 59, 59, 55, 64, 63, 64, 0, 55, 57, 59, 60, 62, 64, 63, 65, 64,  0, 55, 62, 61, 62,  0, 55, 57, 59, 60, 61, 62, 55, 65, 64,  0, 55, 67, 67, 67, 67, 69,  0, 67, 65, 65, 65, 65, 67,  0, 65, 64, 57, 59, 65, 64, 64, 64, 59, 60 };
//	static uint8_t mario2Duration[65] = {18, 18, 18, 18, 18, 18,  9,  9, 18, 18, 45,  9, 54, 54, 54, 9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54,  9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54, 18, 18,  9,  9, 54, 54, 54, 18, 18,  9,  9, 54, 18, 18, 18,  9,  9, 27,  9, 54 };
//	oi_loadSong(MARIO_UNDERWATER, mario2NumNotes, mario2Notes, mario2Duration);
}
