#include <iostream>
#include <stdint.h>

using namespace std;

int main() {
	uint64_t mt;
	int32_t samplerate;
	uint64_t res;
	// mt * 1000000 <= MAX_UINT64
	for (mt=0x000000F000000000; mt<=0x0000200000000000; mt++) {
		res = mt * 1000000;
		// check for overflow
		if (mt != 0 && res/mt != 1000000) {
			continue;
		}
		// bitwise shifted
		for (samplerate=0; samplerate<=65535; samplerate++) {  
			int mHeaderTimescale = 65536;
			int64_t media_time;
			media_time = (uint64_t)mt;
			uint64_t halfscale = mHeaderTimescale / 2;
			media_time = (media_time * 1000000 + halfscale)
				/ mHeaderTimescale;
			int64_t delay = (media_time  * samplerate + 500000) 
				/ 1000000;
			if (delay < 0 || delay > 0xFFFFFFFF)
				cout<<"delay: "<<delay<<", media_time: "<<mt
				<<", samplerate: "<<samplerate<<endl;
		}
	}
	return 0;
}
