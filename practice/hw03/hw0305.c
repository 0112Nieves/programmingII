#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
const char music[][50] = {"Acoustic Grand Piano",
                               "Bright Acoustic Piano",
                               "Electric Grand Piano",
                               "Honky-tonk Piano",
                               "Electric Piano 1 (Rhodes Piano)",
                               "Electric Piano 2 (Chorused Pian",
                               "Harpsichord",
                               "Clavinet",
                               "Celesta",
                               "Glockenspiel",
                               "Music box",
                               "Vibraphone",
                               "Marimba",
                               "Xylophone",
                               "Tubular Bells",
                               "Dulcimer (Santur)",
                               "Drawbar Organ (Hammond)",
                               "Percussive Organ",
                               "Rock Organ",
                               "Church Organ",
                               "Reed Organ",
                               "Accordion (French)",
                               "Harmonica",
                               "Tango Accordion (Band neon)",
                               "Acoustic Guitar (nylon)",
                               "Acoustic Guitar (steel)",
                               "Electric Guitar (jazz)",
                               "Electric Guitar (clean)",
                               "Electric Guitar (muted)",
                               "Overdriven Guitar",
                               "Distortion Guitar",
                               "Guitar Harmonics",
                               "Acoustic Bass",
                               "Electric Bass (fingered)",
                               "Electric Bass (picked)",
                               "Fretless Bass",
                               "Slap Bass 1",
                               "Slap Bass 2",
                               "Synth Bass 1",
                               "Synth Bass 2",
                               "Violin",
                               "Viola",
                               "Cello",
                               "Contrabass",
                               "Tremolo Strings",
                               "Pizzicato Strings",
                               "Orchestral Harp",
                               "Timpani",
                               "String Ensemble 1 (strings)",
                               "String Ensemble 2 (slow strings)",
                               "Synth Strings 1",
                               "Synth Strings 2",
                               "Choir Aahs",
                               "Voice Oohs",
                               "Synth Voice",
                               "Orchestra Hit",
                               "Trumpet",
                               "Trombone",
                               "Tuba",
                               "Muted Trumpet",
                               "French Horn",
                               "Brass Section",
                               "Synth Brass 1",
                               "Synth Brass 2",
                               "Soprano Sax",
                               "Alto Sax",
                               "Tenor Sax",
                               "Baritone Sax",
                               "Oboe",
                               "English Horn",
                               "Bassoon",
                               "Clarinet",
                               "Piccolo",
                               "Flute",
                               "Recorder",
                               "Pan Flute",
                               "Blown Bottle",
                               "Shakuhachi",
                               "Whistle",
                               "Ocarina",
                               "Lead 1 (square wave)",
                               "Lead 2 (sawtooth wave)",
                               "Lead 3 (calliope)",
                               "Lead 4 (chiffer)",
                               "Lead 5 (charang)",
                               "Lead 6 (voice solo)",
                               "Lead 7 (fifths)",
                               "Lead 8 (bass + lead)",
                               "Pad 1 (new age Fantasia)",
                               "Pad 2 (warm)",
                               "Pad 3 (polysynth)",
                               "Pad 4 (choir space voice)",
                               "Pad 5 (bowed glass)",
                               "Pad 6 (metallic pro)",
                               "Pad 7 (halo)",
                               "Pad 8 (sweep)",
                               "FX 1 (rain)",
                               "FX 2 (soundtrack)",
                               "FX 3 (crystal)",
                               "FX 4 (atmosphere)",
                               "FX 5 (brightness)",
                               "FX 6 (goblins)",
                               "FX 7 (echoes, drops)",
                               "FX 8 (sci-fi, star theme)",
                               "Sitar",
                               "Banjo",
                               "Shamisen",
                               "Koto",
                               "Kalimba",
                               "Bag pipe",
                               "Fiddle",
                               "Shana",
                               "Tinkle Bell",
                               "Agogo",
                               "Steel Drums",
                               "Woodblock",
                               "Taiko Drum",
                               "Melodic Tom",
                               "Synth Drum",
                               "Reverse Cymbal",
                               "Guitar Fret Noise",
                               "Breath Noise",
                               "Seashore",
                               "Bird Tweet",
                               "Telephone Ring",
                               "Helicopter",
                               "Applause",
                               "Gunshot"};
typedef struct _headchunk{
    char MThd[4];
    uint32_t headerLength;
    uint16_t format;
    uint16_t trackCount;
    uint16_t ticks;
} __attribute__((packed)) headChunk;
typedef struct _trackchunk{
    char MTrk[4];
    uint32_t trackLength;
    uint16_t trackID;
    uint16_t instrument;
} __attribute__((packed)) trackChunk;
typedef struct _node{
    uint8_t pitch;
    double beat;
    uint8_t value;
    uint8_t instrument;
} __attribute__((packed)) node;
void swap_endian(void *data, size_t size) {
    uint8_t *bytes = (uint8_t *)data;
    for (size_t i = 0; i < size / 2; ++i) {
        uint8_t temp = bytes[i];
        bytes[i] = bytes[size - 1 - i];
        bytes[size - 1 - i] = temp;
    }
}
node parseMusic(const char* str){
    node a;
    char c;
    while((c = fgetc(str)) != NULL){
        if()
    }
    return a;
}
int main()
{
    printf("Welcome to Cool Sound Infinity Expoter!\n");
    printf("Please enter input and output file name.\n");
    printf("Input file name: ");
    char *Iname = calloc(30, sizeof(char));
    char *Oname = calloc(30, sizeof(char));
    fgets(Iname, 30, stdin);
    Iname[strlen(Iname)-1] = '\0';
    printf("Output file name: ");
    fgets(Oname, 30, stdin);
    Oname[strlen(Oname)-1] = '\0';
    FILE *in = fopen(Iname, "r");
    if(in == NULL){
        printf("Error open file input.\n");
        return 0;
    }
    FILE *out = fopen(Oname, "w+");
    if(out == NULL){
        printf("Error open file output.\n");
        return 0;
    }
    headChunk midiHeader;
    trackChunk trackHeader;
    midiHeader.MThd[0] = 0x4D;
    midiHeader.MThd[1] = 0x54;
    midiHeader.MThd[2] = 0x68;
    midiHeader.MThd[3] = 0x64;
    midiHeader.headerLength = 0x06000000;
    midiHeader.ticks = 0xE001;
    trackHeader.MTrk[0] = 0x4D;
    trackHeader.MTrk[1] = 0x54;
    trackHeader.MTrk[2] = 0x72;
    trackHeader.MTrk[3] = 0x6B;
    node *list = NULL;
    char *str = calloc(256, sizeof(char));
    int cformat = 0, cnt = 0;
    while(fgets(str, 256, in) != NULL){
        if(strstr(str, "[") != NULL){
            cformat++;
            trackHeader.trackID = 0x00C0 + (cformat-1);
            swap_endian(&trackHeader.trackID, sizeof(trackHeader.trackID));
            for(uint16_t i = 0; i < 128; i++){
                if(strstr(str, music[i]) != NULL)
                    trackHeader.instrument = ((i+1) & 0xFFFF);
            }
        }
        else{  
            char *token;
            token = strtok(str, "1234567");
            while(token != NULL){
                node *now; now->instrument = trackHeader.instrument;
                *now = parseMusic(token);
                list = realloc(list, (cnt+1)*sizeof(node));
                list[cnt] = *now;
                cnt++;
                token = strtok(NULL, "1234567");
            }
        }
    }
    if(cformat == 1){
        midiHeader.format = 0x0000;
        midiHeader.trackCount = 0x0001;
        swap_endian(&midiHeader.trackCount, sizeof(midiHeader.trackCount));
    }
    else{
        midiHeader.format = 0x0001;
        swap_endian(&midiHeader.format, sizeof(midiHeader.format));
        midiHeader.trackCount = cformat;
        swap_endian(&midiHeader.trackCount, sizeof(midiHeader.trackCount));
    }
    fwrite(&midiHeader, sizeof(midiHeader), 1, out);
    fwrite(&trackHeader, sizeof(trackHeader), 1, out);
    free(Iname); free(Oname); free(str);
    fclose(in); fclose(out);
    return 0;
}