/***************************************************************************************
****************************************************************************************
**
** Copyright (c) 2013 Clear Trail.
**
** File Name  :   opus_dll.h
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
#ifndef __OPUS_DLL_H__
#define __OPUS_DLL_H__ (1)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>

#define SUCCESS 0
#define FAILURE 1

#ifdef __cplusplus
extern "C" { 

#endif
__declspec(dllexport) void LoadCodec (void **ppVoid);
__declspec(dllexport) int setSamplingRate (void **pVoid, int Fs);
__declspec(dllexport) void UnLoadCodec  (void *pVoid);
__declspec(dllexport) unsigned char DecodeFrame (void *pVoid,
                                                 unsigned char *pszEncodedBuffer,
                                                 long dwEncodedBufferSize,
                                                 unsigned char **pszDecodedBuffer,
                                                 long *pdwDecodedBufferSize);

#ifdef __cplusplus
}
#endif

#endif // __OPUS_DLL_H__

/* EOF */
/* 
$Log$ 
*/

