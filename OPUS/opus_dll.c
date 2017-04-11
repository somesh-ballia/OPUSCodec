/***************************************************************************************
****************************************************************************************
**
** Copyright (c) 2013 Clear Trail.
**
** File Name  :   opus_dll.c
**
** Module Name:   Voice Codecs
**
** $Author: jinuga.preetham $
**
** $RCSfile$
**
** $Revision$
**
** Change History
** -------------------------------------------------------------------------------------
** Author      Date    Version     Description                 SearchTag   Reviewed By
** -------------------------------------------------------------------------------------
**
****************************************************************************************
***************************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "opus.h"
#include "debug.h"
#include "opus_types.h"
#include "opus.h"
#include "opus_dll.h"

#define MAX_PACKET 1500

/* Function to change the sampling rate of already created decoder */
int setSamplingRate (void **pVoid, int Fs)
{
    struct OpusDecoder *dec = NULL;
    int num_channels = 1;
    int ret = FAILURE;

    if ((Fs != 8000) && (Fs != 12000) && (Fs != 16000) && (Fs != 24000) && (Fs != 48000))
    {
        fprintf(stderr, "Supported sampling rates: 8000/12000/16000/24000/48000.\n");
        return OPUS_BAD_ARG;
    }

    if (NULL == *pVoid)
    {
        fprintf(stderr, "NULL decoder state \n");
        return OPUS_BAD_ARG;
    }

    dec = (struct OpusDecoder *)*pVoid;
    opus_decoder_ctl(dec, OPUS_RESET_STATE);
    ret = opus_decoder_init(dec, Fs, num_channels);


    //printf("Fs is reset to %d \n",((struct OpusDecoder *)dec).Fs);
    if(ret!=OPUS_OK)
        return ret;

    return SUCCESS;
}

OpusDecoder *OPUS_init_decoder()
{
    int err;
    OpusDecoder *dec = NULL;
    opus_int32  Fs   = 8000;    // default
    opus_int32  num_channels = 1;

    if ((Fs != 8000) && (Fs != 12000) && (Fs != 16000) && (Fs != 24000) && (Fs != 48000))
    {
        fprintf(stderr, "Supported sampling rates: 8000/12000/16000/24000/48000.\n");
        return NULL;
    }

    dec = opus_decoder_create(Fs, num_channels, &err);
    if (err != OPUS_OK)
    {
        fprintf(stderr, "Cannot create decoder: %s\n", opus_strerror(err));
        return NULL;
    }
    return dec;
}

void LoadCodec(void **ppVoid)
{
   *ppVoid = (void *)OPUS_init_decoder();
   //setSamplingRate (ppVoid, 48000);
}

void UnLoadCodec(void *pVoid)
{
    OpusDecoder *dec = (OpusDecoder *)pVoid;
    opus_decoder_destroy(dec);
    return;
}

unsigned char DecodeFrame(void* pVoid, 
                          unsigned char* pszEncodedBuffer,
                          long dwEncodedBufferSize,
                          unsigned char** pszDecodedBuffer,
                          long *pdwDecodedBufferSize)
{
    unsigned char rc   = FAILURE;
    OpusDecoder *dec   = NULL;
    int max_frame_size = (960 * 6);
    int output_samples = 0;
    int lost           = 0;
    DWORD dwOpSize     = *pdwDecodedBufferSize;

    opus_int16 out[960 * 6 * 2]; //max_frame_size*channels*sizeof(short)

    if ((NULL == pVoid) || (NULL == pszEncodedBuffer) ||\
        (NULL == (*pszDecodedBuffer)) || (0 >= dwEncodedBufferSize))
    {
        return rc;
    }

    dec = (OpusDecoder *)pVoid;

    //printf("OPUS sampling rate = %d \n", (OpusDecoder *)dec->Fs);

    output_samples = max_frame_size;
    output_samples = opus_decode(dec, 
                                 lost ? NULL : pszEncodedBuffer,
                                 dwEncodedBufferSize, 
                                 out, 
                                 output_samples, 
                                 0);

    memcpy(*pszDecodedBuffer, out, (output_samples * sizeof(opus_int16)));
    *pdwDecodedBufferSize = (output_samples * sizeof(opus_int16));

    return SUCCESS;
}


/* EOF */
/* 
$Log$ 
*/

