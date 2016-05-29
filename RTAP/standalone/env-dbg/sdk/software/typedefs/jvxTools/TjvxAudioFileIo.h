#ifndef __TJVXAUDIOFILE_H__
#define __TJVXAUDIOFILE_H__

//namespace IrtpAudioFile
//{
typedef enum
{
	JVX_FILETYPE_NONE = -1,

	//! Standard Windows WAV file
	JVX_FILETYPE_WAV = 0,

	//! Raw audio file
	JVX_FILETYPE_RAW = 1
} jvxEndpointDescription;

//! Type of behavior in case the file has been completely read
typedef enum
{
	JVX_FILECONTINUE_NONE = -1,
	//! Stop at the end
	JVX_FILECONTINUE_STOPEND = 0,

	//! Restart the file for infinite playback
	JVX_FILECONTINUE_LOOP = 1
} jvxFileContinueType;

//! Operation mode when using rtFileReader/Writer: Realtime or batch mode
typedef enum
{
	JVX_FILEOPERATION_NONE = -1,

	//! Used buffered input/output 
	JVX_FILEOPERATION_REALTIME = 0,

	//! Read samples directly from file in batch mode
	JVX_FILEOPERATION_BATCH = 1
} jvxFileOperationType;
//};

typedef struct
{
	jvxSize numberChannels;
	jvxInt32 samplerate;
	jvxBool littleEndian;
	jvxDataFormat fFormatFile;
	jvxSize lengthFile;

} jvxFileDescriptionEndpoint_open;

typedef struct
{
	jvxDataFormat fFormatAppl;
	jvxInt32 numSamplesFrameMax_moveBgrd;

} jvxFileDescriptionEndpoint_prepare;

#define JVX_NUMBER_AUDIO_FILE_TAGS 4

typedef enum
{
	JVX_AUDIO_FILE_TAG_BTFLD_ARTIST = 0x1,
	JVX_AUDIO_FILE_TAG_BTFLD_TITLE = 0x2,
	JVX_AUDIO_FILE_TAG_BTFLD_CREATION_DATA = 0x4,
	JVX_AUDIO_FILE_TAG_BTFLD_GENRE = 0x8
} jvxAudioFileTagType;

static const char* jvxAudioFileTagType_txt[4] =
{
	"IART",
	"INAM",
	"ICRD",
	"IGNR"
};

#endif