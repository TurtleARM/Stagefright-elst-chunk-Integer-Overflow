# Stagefright 'elst' chunk Integer Overflow

Providing specific combinations of media_time and samplerate MetaData leads to an Integer Overflow
when calculating the value for kKeyEncoderDelay.
`MPEG4Extractor::parseChunk(off64_t *offset, int depth)`

```
./mystagefright video/mp4 '/videos/1337' 

entering parseChunk 0/0
entering parseChunk 20/0
entering parseChunk 28/1
TimeScale: 65536
mvhd duration: 552
dst: 0xf619c028, size: 8
entering parseChunk 68/1
dst: 0xf619a6c0, size: 25
entering parseChunk 76/2
*** coding='enca' 1 channels, size 31, rate 9481
dst: 0xf61abb0c, size: 4
dst: 0xf61abb2c, size: 4
entering parseChunk 112/2
mdhd version: 0
mdhd timescale: 83886080
mdhd duration: 1048576
dst: 0xf619c030, size: 8
dst: 0xf61c217c, size: 4
entering parseChunk 142/2
elst version: 1
elst entry_count: 1
media_time: 1030792151040                   <-- both duration and media_time have been set
samplerate: 9481
dst: 0xf61c217c, size: 4
dst: 0xf61c218c, size: 4
entering parseChunk 174/0
dst: 0xf619a680, size: 25
Found delay: -143831936                     <-- delay wrapped to a negative value
Duration: 8422
MPEG4Extractor::countTracks: 0 tracks
```
