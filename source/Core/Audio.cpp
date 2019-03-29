/*
* ΔLaunch
* Copyright (C) 2018  Reisyukaku
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Audio.hpp"

void Audio::Init() {
	/*static const AudioRendererConfig arConfig =
    {
        .output_rate     = AudioRendererOutputRate_48kHz,
        .num_voices      = 24,
        .num_effects     = 0,
        .num_sinks       = 1,
        .num_mix_objs    = 1,
        .num_mix_buffers = 2,
    };

    AudioDriver drv;
    Result res;
    res = audrenInitialize(&arConfig);
    bool initedDriver = false;
    bool initedAudren = R_SUCCEEDED(res);*/
}

void Audio::Load(std::string file, bool loop) {
	/*FILE *fp = fopen(file.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	size_t fsize = ftell(fp);
	rewind(fp);
	size_t mempool_size = (fsize + 0xFFF) &~ 0xFFF;
    void* mempool_ptr = memalign(0x1000, mempool_size);
	fread(mempool_ptr, fsize, 1, fp);
	fclose(fp);
	
    armDCacheFlush(mempool_ptr, mempool_size);

    AudioDriverWaveBuf wavebuf = {0};
    wavebuf.data_raw = mempool_ptr;
    wavebuf.size = fsize;
    wavebuf.start_sample_offset = 0;
    wavebuf.end_sample_offset = fsize/2;
    wavebuf.is_looping = loop;*/
}

void Audio::Exit() {
	//
}