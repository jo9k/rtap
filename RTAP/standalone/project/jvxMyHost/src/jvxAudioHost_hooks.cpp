#include "jvx.h"
#include "jvxAuNMyAlgorithm.h"

jvxErrorType jvx_LinkObjects(jvxInitObject_tp* funcInit, jvxTerminateObject_tp* funcTerm, char* description, jvxComponentType tp, jvxSize id)
{
	switch(tp)
	{
	case JVX_COMPONENT_AUDIO_NODE:
		if(id == 0)
		{
			std::string descr = "Audio Simple";
			jvxSize mL = JVX_MAXSTRING-1;
			*funcInit = jvxAuNMyAlgorithm_init;
			*funcTerm = jvxAuNMyAlgorithm_terminate;
			
			// description is a buffer of length JVX_MAXSTRING
			mL = JVX_MIN(descr.size(),mL);
			memcpy(description, descr.c_str(), mL);
			return(JVX_NO_ERROR);
		}
		break;
	default:
		break;
	}
	return(JVX_ERROR_ELEMENT_NOT_FOUND);
}

// =============================================================
// Configure ui extensions
// =============================================================
jvxHandle* configureAudio_UiExtension_init_device()
{
	return(NULL);
}

void configureAudio_UiExtension_terminate_device(jvxHandle* elm)
{
	assert(elm == NULL);
}

#include "myCentralWidget.h"

// =============================================================
// Main widget entry functions
// =============================================================
jvxHandle* mainWindow_UiExtension_init(QWidget* parent)
{
#ifdef JVX_PROJECT_NAMESPACE
	JVX_PROJECT_NAMESPACE::myCentralWidget* newElm = new JVX_PROJECT_NAMESPACE::myCentralWidget(parent);
#else
	myCentralWidget* newElm = new myCentralWidget(parent);
#endif
	return(reinterpret_cast<jvxHandle*>(static_cast<mainWindow_UiExtension*>(newElm)));
}

void mainWindow_UiExtension_terminate(jvxHandle* elm)
{
	delete(reinterpret_cast<mainWindow_UiExtension*>(elm));
}

// =============================================================
// Specify some runtime host configurations
// =============================================================
jvxErrorType configureAudioHost_features_request(jvxHandle* features)
{
	return(JVX_NO_ERROR);
}

jvxErrorType configureDevelopHost_features_request(jvxHandle* features)
{
	return(JVX_NO_ERROR);
}