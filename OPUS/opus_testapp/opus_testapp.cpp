#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include <Winsock2.h>

#define HEADER_SIZE 8
#define IN_BUFFER_SIZE 4096
#define OUT_BUFFER_SIZE 4096

typedef  unsigned char u8;
typedef void			(* pLoadFuncPtr)	 (void**);
typedef void			(* pUnLoadFuncPtr)(void*);
typedef unsigned char	(* pDecodeFuncPtr)(void*, u8*, long, unsigned char**, long*);

unsigned short ToUINT16(u8* byBuffer, int iStartIndex)
	{
		return (unsigned short)(
			(byBuffer[iStartIndex    ] <<  8) |
			(byBuffer[iStartIndex + 1]      ));
	};

void test_opus_codec()
{
	//char *pszPath = "D:\\MMF_StreamingServer_5D\\CLEARTRAILCPPUTILITIES\\DLL\\opus.dll";	
	char *pszPath = "C:\\Program Files\\ClearTrail\\MultimediaStreamingServer\\Codecs\\opus.dll";
	HANDLE lib = NULL;
	HANDLE hlib = NULL;

	int m_dwSrcLength = 0; 
	long m_dwDesLength = 0;


	lib = LoadLibraryEx(TEXT(pszPath),NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	pLoadFuncPtr LoadCodec = (pLoadFuncPtr)GetProcAddress((HMODULE)lib, "LoadCodec");
	LoadCodec(&hlib);

	char* file_name = "C:\\Users\\somesh.pathak\\Desktop\\Zello\\PCtoPCVoice\\2\\PC2PC_single.opus";
	char* d_file_name = "C:\\Users\\somesh.pathak\\Desktop\\Zello\\PCtoPCVoice\\2\\PC2PC_single.wav";
	FILE *fpr = NULL;
	FILE *fpw = NULL;
	fpr = fopen(file_name, "rb");
	fpw = fopen(d_file_name, "wb");

	int rt = 1;
	while (rt)
	{
		char pbyHeaderBuffer[HEADER_SIZE] = {0};
		unsigned char m_SrcBuffer[IN_BUFFER_SIZE] = {0};
		u8* m_pbySrcBuffer = m_SrcBuffer;
		unsigned char m_DesBuffer[OUT_BUFFER_SIZE] = {0};
		u8 *m_pbyDesBuffer = m_DesBuffer;
		rt = fread(pbyHeaderBuffer,1,HEADER_SIZE,fpr);

		short sTemp = ToUINT16((unsigned char*)pbyHeaderBuffer,0);
		short sPacketLength = ToUINT16((unsigned char*)&sTemp,0);

		m_dwSrcLength = sPacketLength;
		rt = fread(m_pbySrcBuffer, 1, m_dwSrcLength, fpr);

		pDecodeFuncPtr DecodeFrame = (pDecodeFuncPtr)GetProcAddress((HMODULE)lib, "DecodeFrame");
		int retCode = DecodeFrame(hlib, 
			m_pbySrcBuffer, 
			m_dwSrcLength, 
			&m_pbyDesBuffer, 
			&m_dwDesLength);		
		fwrite(m_pbyDesBuffer, 1, m_dwDesLength, fpw);
		fflush(fpw);
	}
	fclose(fpw);
}


int main()
{
	test_opus_codec();
};
