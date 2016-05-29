#ifndef _CONFIGUREAUDIOHOST_FEATURES_H__
#define _CONFIGUREAUDIOHOST_FEATURES_H__

struct configureAudioHost_features
{
	jvxBool specifyDefaultSequenceOnlyIfThereIsNone; // Default: true
	jvxBool loadSystemAudioTechnologyComponents; // Default: true
	jvxBool loadPortAudioTechnologyComponents; // Default: true
	jvxInt32 timeout_viewer_ms;
};
	
#endif