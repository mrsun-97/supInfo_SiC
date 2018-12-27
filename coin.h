#pragma once		//∑¿÷π∂‡¥Œ‘ÿ»Î
#ifdef DLL_EXPORTS
#define DLL_IO  _declspec(dllexport)
#else
#define DLL_IO  _declspec(dllimport)
#endif

/*def coin(stamp,channel,value,delay,bin):
	delay(stamp,channel,value,delay)
	coin = [0,0,0,0,0,0]
	°§°§°§
*/

extern "C" DLL_IO int* ffcoin(int* stamp, int* channel, int value, int delay[8], int bin);