#ifndef __HJVXQTCLASS_QT_H__
#define __HJVXQTCLASS_QT_H__

#include <QMetaType>

Q_DECLARE_METATYPE(jvxState)
Q_DECLARE_METATYPE(jvxBool)

#define JVX_QT_CLASS_FRAGMENTS_CONSTRUCTOR \
		qRegisterMetaType<jvxState>("jvxState"); \
		qRegisterMetaType<jvxBool>("jvxBool");



#define JVX_QT_CLASS_FRAGMENTS_UPDATE_WINDOW_SYNC 	\
	void updateWindow_sync(jvxState theState, jvxBool updateDelayed) \
	{ \
		if((_common_set_qt.idQtThread == JVX_GET_CURRENT_THREAD_ID()) && (!updateDelayed)) \
		{ \
			this->updateWindow_in_sync(theState, updateDelayed); \
		} \
		else \
		{ \
			emit emit_updateWindow_sync(theState, updateDelayed); \
		} \
	}; \

#define JVX_QT_CLASS_FRAGMENTS_UPDATE_WINDOW_CONNECT \
	connect(this, SIGNAL(emit_updateWindow_sync(jvxState, jvxBool)), this, SLOT(updateWindow_in_sync(jvxState, jvxBool)), Qt::QueuedConnection);

#endif
