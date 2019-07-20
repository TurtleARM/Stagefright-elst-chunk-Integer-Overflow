#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/FileSource.h>
#include <media/stagefright/MetaData.h>
#include <string.h>
#include <cstdint>

using namespace android;

int main(int argc, char **argv) {
	if (argc != 3) return -1;
	// Input file to be tested
    sp<FileSource> fileSource = new FileSource(argv[2]);
    // MediaExtractor client
    sp<IMediaExtractor> extractor = 
		MediaExtractor::Create(fileSource, argv[1]);
    int64_t durationUs;
    // Get video MetaData
    sp<MetaData> meta = extractor->getMetaData(); 
	int32_t delay;
	if (meta->findInt32(kKeyEncoderDelay, &delay)) {
		fprintf(stderr, "Could not find delay.\n");
		return -1;
	}
	fprintf(stdout, "Found delay: %d\n", delay); // Should be negative!
	if (!meta->findInt64(kKeyDuration, &durationUs)) {
		fprintf(stderr, "Could not find duration.\n");
		//return -1;
	}
	fprintf(stdout, "Duration: %lld\n", (long long)durationUs);
    if (extractor == NULL) {
      fprintf(stderr, "Could not create extractor\n");
      return -1;
    } 
    size_t numTracks = extractor->countTracks();
    size_t i;
    // Attempt to identify each track by its MetaData
    for (i = 0; i < numTracks; ++i) {
      meta = extractor->getTrackMetaData(
        i, MediaExtractor::kIncludeExtensiveMetaData);
      const char *mime;
      meta->findCString(kKeyMIMEType, &mime);
      // We only support videos in our test suite
      if (strncasecmp(mime, "video/", 6)) {
        break;
      }
      meta = NULL;
    }
    // No tracks found
    if (meta == NULL) return -1;
    sp<IMediaSource> mediaSource;
    for (size_t i = 0; i < numTracks; ++i) {
      mediaSource = extractor->getTrack(i);
    }
    return 0;
}
