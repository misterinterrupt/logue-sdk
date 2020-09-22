#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

int main(int argc, char** argv)
{
    if( argc == 2 ) {
        printf("Processing %s\n", argv[1]);
    } else {
        printf("First argument is the filename.\n");
        return 0;
    }
    size_t len = strlen(argv[1]);
    if(len > 32) {
        printf("Filename is longer than 32 characters! :(\n");
        return 0;
    }

    char * wavname = malloc(len-1);
    strcpy(wavname, argv[1]);

    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalPCMFrameCount;
    short* pSampleData = drwav_open_file_and_read_pcm_frames_s16(wavname, &channels, &sampleRate, &totalPCMFrameCount, NULL);
    if (pSampleData == NULL) {
        // Error opening and reading WAV file.
    }
    
    FILE * pFile;
    
    char * cfilename = malloc(len-1);
    cfilename = strncat(cfilename, wavname, len-4);
    cfilename = strncat(cfilename, ".h", 2);

    pFile = fopen(cfilename,"w");

    fprintf (pFile, "float wave[%llu] = {\n  ", totalPCMFrameCount);
    for (int i=0; i<totalPCMFrameCount; i++) {
        fprintf (pFile, "%hd", pSampleData[i]);
        if(i != totalPCMFrameCount-1) {
            fprintf (pFile, ", ");
        }
        if((i+1) % 8 == 0) {
            fprintf (pFile, "\n  ");
        }
    }
    fprintf (pFile, "};\n\n");
    fclose(pFile);
    drwav_free(pSampleData, NULL);
    return 0;
}