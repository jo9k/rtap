#ifndef __CONFIGUREAUDIOSPECIFIC_H__
#define __CONFIGUREAUDIOSPECIFIC_H__

#include "jvx.h"
#include "interfaces/qt-audio-host/configureAudio_UiExtension.h"
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

class configureAudio_specific_socket: public QObject, public configureAudio_UiExtension
{
private:
	IjvxHost* theHostRef;
	IjvxSubModuleInstall* theDelegateLink;

	struct
	{
		jvxInt32 id_sockhostname;
		jvxInt32 id_sockhostport;
		jvxInt32 id_sockconnstate;
		jvxInt32 id_sockcommand;

		jvxInt32 id_lostframes;
		jvxInt32 id_unsentframes;
		jvxInt32 id_remoteframes;
	} properties;

	QFrame* ptFrame;

	Q_OBJECT
public:
	configureAudio_specific_socket();
	~configureAudio_specific_socket();
	
	void init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* );
	void terminate_submodule();

	virtual void start_show(jvxComponentType tp, QFrame* placeHere);
	virtual void stop_show(QFrame* placeHere);

	void refreshPropertyIds();
	void refreshPropertyIds_run();
	void updateWindow();
	void updateWindow_periodic();

	void designFields(QFrame* placeHere, jvxComponentType tp);
	void cleanFields(QFrame* placeHere);

	void setBackgroundColorLabelConnectionState_exclusive(int id);
	void setTextAndActiveButton(std::string txt, jvxBool active, jvxInt32 idCompare);
	void setTextAndActiveLineEdit(std::string txt, jvxBool active, jvxInt32 idCompare);

public slots:

	void newTextDestHost();
	void newTextDestPort();
	void pushedButtonGo();
};

#endif