#include "configureAudio_specific_socket.h"
#include "helpers/HjvxHostsAndProperties.h"
#include "typedefs/TjvxSubModuleTypeIds.h"
#include "jvxAudioHost_common.h"
#include "helpers/qt/HjvxQtHelpers.h"

#include <QGridLayout>

#define JVX_LOCAL_SHOW_HOSTNAME 0
#define JVX_LOCAL_SHOW_HOSTPORT 1
#define JVX_LOCAL_SHOW_CONNECT_DISCONNECT 2
#define JVX_LOCAL_SHOW_LABEL_LOST_FRAMES 3
#define JVX_LOCAL_SHOW_TEXTEDIT_LOST_FRAMES 4
#define JVX_LOCAL_SHOW_LABEL_UNSENT_FRAMES 5
#define JVX_LOCAL_SHOW_TEXTEDIT_UNSENT_FRAMES 6

#define JVX_LOCAL_SHOW_CONNECTSTATE_OFF 16

#define JVX_NUMBER_STATES_CONNECTION 6

// =============================================================
// =============================================================
// =============================================================

configureAudio_specific_socket::configureAudio_specific_socket()
{
	theHostRef = NULL;
	theDelegateLink = NULL;
	ptFrame = NULL;

	properties.id_sockcommand = -1;
	properties.id_sockconnstate = -1;
	properties.id_sockhostname = -1;
	properties.id_sockhostport = -1;

}

configureAudio_specific_socket::~configureAudio_specific_socket()
{
}

void
configureAudio_specific_socket::init_submodule(IjvxHost* hostRef, IjvxSubModuleInstall* bwd)
{
	theDelegateLink = bwd;
	theHostRef = hostRef;
	theDelegateLink->set_link(reinterpret_cast<jvxHandle*>(static_cast<configureAudio_UiExtension*>(this)), JVX_CONFIGURE_AUDIO_EXTENSION_UI);
}

void
configureAudio_specific_socket::terminate_submodule()
{
	if(theDelegateLink)
	{
		theDelegateLink->release_link(reinterpret_cast<jvxHandle*>(static_cast<configureAudio_UiExtension*>(this)), JVX_CONFIGURE_AUDIO_EXTENSION_UI);
	}
}

void
configureAudio_specific_socket::refreshPropertyIds()
{
	jvxState stat = JVX_STATE_NONE;
	jvx_propertyReferenceTriple theDevProps;

	jvxSize num;
	jvxBool foundit;
	jvxBool isValid;
	jvxSize hdlIdx;

	properties.id_sockhostname = -1;
	properties.id_sockhostport = -1;
	properties.id_sockconnstate = -1;
	properties.id_sockcommand = -1;

	if(theHostRef)
	{
		theHostRef->state(&stat);
		if(stat == JVX_STATE_ACTIVE)
		{
			theHostRef->state_component(JVX_COMPONENT_AUDIO_TECHNOLOGY, &stat);
			if(stat == JVX_STATE_ACTIVE)
			{
				theHostRef->state_component(JVX_COMPONENT_AUDIO_DEVICE, &stat);
				if(stat >= JVX_STATE_ACTIVE)
				{
					jvx_initPropertyReferenceTriple(&theDevProps);
					jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					if(jvx_isValidPropertyReferenceTriple(&theDevProps))
					{
						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_SOCKET_HOSTNAME", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_sockhostname = hdlIdx;
						}
						else
						{
							assert(0);
						}

						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_SOCKET_HOSTPORT", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_sockhostport = hdlIdx;
						}
						else
						{
							assert(0);
						}

						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_SOCKET_CONN_STATE", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_sockconnstate = hdlIdx;
						}
						else
						{
							assert(0);
						}

						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_SOCKET_COMMAND", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_sockcommand = hdlIdx;
						}
						else
						{
							assert(0);
						}
					}
				}
			}
		}
	}
}

void
configureAudio_specific_socket::refreshPropertyIds_run()
{
	jvxState stat = JVX_STATE_NONE;
	jvx_propertyReferenceTriple theDevProps;

	jvxSize num;
	jvxBool foundit;
	jvxBool isValid;
	jvxSize hdlIdx;

	properties.id_lostframes = -1;
	properties.id_unsentframes = -1;
	properties.id_remoteframes = -1;

	if(theHostRef)
	{
		theHostRef->state(&stat);
		if(stat == JVX_STATE_ACTIVE)
		{
			theHostRef->state_component(JVX_COMPONENT_AUDIO_TECHNOLOGY, &stat);
			if(stat == JVX_STATE_ACTIVE)
			{
				theHostRef->state_component(JVX_COMPONENT_AUDIO_DEVICE, &stat);
				if(stat >= JVX_STATE_ACTIVE)
				{
					jvx_initPropertyReferenceTriple(&theDevProps);
					jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					if(jvx_isValidPropertyReferenceTriple(&theDevProps))
					{
						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_NET_AUDEV_LOSTFRAMES", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_lostframes = hdlIdx;
						}
						else
						{
							assert(0);
						}
						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_NET_AUDEV_UNSENTFRAMES", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_unsentframes = hdlIdx;
						}
						else
						{
							assert(0);
						}

						num = 0;
						foundit = false;
						hdlIdx = 0;
						isValid = false;

						FIND_PROPERTY_ID(theDevProps, hdlIdx, "JVX_GENW/JVX_NET_AUDEV_LOSTFRAMES_REMOTE", isValid, foundit);
						if(foundit && isValid)
						{
							properties.id_remoteframes = hdlIdx;
						}
						else
						{
							assert(0);
						}

						jvx_returnReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					}
				}
			}
		}
	}
}

void
configureAudio_specific_socket::updateWindow()
{
	jvxState stat = JVX_STATE_NONE;
	jvx_propertyReferenceTriple theDevProps;
	jvxErrorType res = JVX_NO_ERROR;
	jvxBitField selection = 0;
	jvxBool isValid = false;
	jvxSelectionList selLst;
	std::string hostname;
	jvxInt32 hostport;
	selLst.strList = NULL;
	jvxString* fldStr = NULL;
	jvxBool isActive = false;

	setBackgroundColorLabelConnectionState_exclusive(-1);
	setTextAndActiveButton("--", false, JVX_LOCAL_SHOW_CONNECT_DISCONNECT);
	if(theHostRef)
	{
		theHostRef->state(&stat);
		if(stat == JVX_STATE_ACTIVE)
		{
			theHostRef->state_component(JVX_COMPONENT_AUDIO_TECHNOLOGY, &stat);
			if(stat == JVX_STATE_ACTIVE)
			{
				theHostRef->state_component(JVX_COMPONENT_AUDIO_DEVICE, &stat);
				if(stat >= JVX_STATE_ACTIVE)
				{
					if(stat > JVX_STATE_ACTIVE)
					{
						isActive = true;
					}
					jvx_initPropertyReferenceTriple(&theDevProps);
					jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					if(jvx_isValidPropertyReferenceTriple(&theDevProps))
					{
						if(properties.id_sockhostname >= 0)
						{
							res = theDevProps.theProps->get_property(&fldStr, 1, JVX_DATAFORMAT_STRING, properties.id_sockhostname, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
							assert(res == JVX_NO_ERROR);
							assert(fldStr);
							hostname = fldStr->bString;
							theDevProps.theObj->deallocate(fldStr);
						}

						if(properties.id_sockhostport >= 0)
						{
							res = theDevProps.theProps->get_property(&hostport, 1, JVX_DATAFORMAT_32BIT_LE, properties.id_sockhostport, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
							assert(res == JVX_NO_ERROR);
						}

						if(properties.id_sockconnstate >= 0)
						{
							res = theDevProps.theProps->get_property(&selLst, 1, JVX_DATAFORMAT_SELECTION_LIST, properties.id_sockconnstate, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
							assert(res == JVX_NO_ERROR);
							if(isActive)
							{
								setTextAndActiveButton("Disconnect", false, JVX_LOCAL_SHOW_CONNECT_DISCONNECT);
							}
							else
							{
								setTextAndActiveButton("Disconnect", true, JVX_LOCAL_SHOW_CONNECT_DISCONNECT);
							}
							setTextAndActiveLineEdit(hostname.c_str(), false, JVX_LOCAL_SHOW_HOSTNAME);
							setTextAndActiveLineEdit((jvx_int2String(hostport)).c_str(), false, JVX_LOCAL_SHOW_HOSTPORT);

							setTextAndActiveLineEdit("--", true, JVX_LOCAL_SHOW_TEXTEDIT_LOST_FRAMES);
							setTextAndActiveLineEdit("--", true, JVX_LOCAL_SHOW_TEXTEDIT_UNSENT_FRAMES);

							switch(selLst.bitFieldSelected)
							{
							case 0x1: // "NONE":
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+0);
								setTextAndActiveButton("Connect", true, JVX_LOCAL_SHOW_CONNECT_DISCONNECT);
								setTextAndActiveLineEdit(hostname.c_str(), true, JVX_LOCAL_SHOW_HOSTNAME);
								setTextAndActiveLineEdit((jvx_int2String(hostport)).c_str(), true, JVX_LOCAL_SHOW_HOSTPORT);
								break;
							case 0x2: //"SOCKET_TYPE_SET"
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+1);
								break;
							case 0x4: //"SOCKET_CLIENT_STARTED"
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+2);
								break;
							case 0x8: //, "SOCKET_CLIENT_CONNECTED",
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+3);
								break;
							case 0x10: // "SOCKET_CLIENT_HSK_COMPLETE",
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+4);
								break;
							case 0x20: // "SOCKET_CLIENT_SHUTDOWN"
								setBackgroundColorLabelConnectionState_exclusive(JVX_LOCAL_SHOW_CONNECTSTATE_OFF+5);
								break;
							}
						}
						jvx_returnReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					}
				}
			}
		}
	}

/*	properties.id_socktypes = -1;
	properties.id_sockifaces = -1;
	properties.id_sockhostname = -1;
	properties.id_sockhostport = -1;
	*/
}

void
configureAudio_specific_socket::updateWindow_periodic()
{
	jvx_propertyReferenceTriple theDevProps;
	jvxErrorType res = JVX_NO_ERROR;
	jvxBitField selection = 0;
	jvxBool isValid = false;
	jvxInt32 valI32;
	std::string txtShow;

	jvx_initPropertyReferenceTriple(&theDevProps);
	jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
	if(jvx_isValidPropertyReferenceTriple(&theDevProps))
	{
		if(properties.id_lostframes >= 0)
		{
			res = theDevProps.theProps->get_property(&valI32, 1, JVX_DATAFORMAT_32BIT_LE, properties.id_lostframes, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
			assert(res == JVX_NO_ERROR);
			txtShow += jvx_int2String(valI32);

		}
		else
		{
			txtShow += "--";
		}

		txtShow += "/";

		if(properties.id_unsentframes >= 0)
		{
			res = theDevProps.theProps->get_property(&valI32, 1, JVX_DATAFORMAT_32BIT_LE, properties.id_unsentframes, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
			assert(res == JVX_NO_ERROR);
			txtShow += jvx_int2String(valI32);
		}
		else
		{
			txtShow += "--";
		}

		txtShow += "/";

		if(properties.id_remoteframes >= 0)
		{
			res = theDevProps.theProps->get_property(&valI32, 1, JVX_DATAFORMAT_32BIT_LE, properties.id_remoteframes, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
			assert(res == JVX_NO_ERROR);
			txtShow += jvx_int2String(valI32);
		}
		else
		{
			txtShow += "--";
		}

		setTextAndActiveLineEdit(txtShow.c_str(), true, JVX_LOCAL_SHOW_TEXTEDIT_LOST_FRAMES);

		jvx_returnReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
	}
}

void
configureAudio_specific_socket::start_show(jvxComponentType tp, QFrame* placeHere)
{
	designFields(placeHere, tp);
	placeHere->show();
}

void
configureAudio_specific_socket::stop_show(QFrame* placeHere)
{
	placeHere->hide();
	cleanFields(placeHere);
}

void
configureAudio_specific_socket::designFields(QFrame* placeHere, jvxComponentType tp)
{
	jvxSize i;
	QGridLayout* theLayout;
	QLabel* theLabel_selectSocketType;
	QLineEdit* theTextEdit_specifyDestHost;
	QLineEdit* theTextEdit_specifyDestPort;
	QPushButton* theButton_go;
	QLabel* theLabel_connectionState;
	
	QLabel* theLabel_performance;
	QLabel* theLabel_lostFrames;
	QLineEdit* theTextEdit_lostFrames;
	
	QLabel* theLabel_unsentFrames;
	QLineEdit* theTextEdit_unsentFrames;

	QFont font;
	QPalette pal;
	QBrush brush_gray(QColor(220, 220, 220, 255));
	QBrush brush_white(QColor(255, 255, 255, 255));
	QBrush brush_black(QColor(0, 0, 0, 255));

	theLayout = new QGridLayout;

	theLabel_selectSocketType = new QLabel(placeHere);
	theLabel_selectSocketType->setText("Specify Connection");
	theLabel_selectSocketType->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
    font = theLabel_selectSocketType->font();
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theLabel_selectSocketType->setFont(font);
	theLayout->addWidget(theLabel_selectSocketType, 0, 0, 1, 1);
	//theLabel_selectSocketType->hide();

	theTextEdit_specifyDestHost = new QLineEdit(placeHere);
	theTextEdit_specifyDestHost->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
	theTextEdit_specifyDestHost->setProperty("JVX_WHAT_TO_SHOW", qVariantFromValue((int)JVX_LOCAL_SHOW_HOSTNAME));
	font = theTextEdit_specifyDestHost->font();
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theTextEdit_specifyDestHost->setFont(font);
	theLayout->addWidget(theTextEdit_specifyDestHost , 0, 1, 1, 1);
	connect(theTextEdit_specifyDestHost, SIGNAL(editingFinished()), this, SLOT(newTextDestHost()));

	theTextEdit_specifyDestPort = new QLineEdit(placeHere);
	theTextEdit_specifyDestPort->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
	theTextEdit_specifyDestPort->setProperty("JVX_WHAT_TO_SHOW", qVariantFromValue((int)JVX_LOCAL_SHOW_HOSTPORT));
	font = theTextEdit_specifyDestPort->font();
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theTextEdit_specifyDestPort->setFont(font);
	theLayout->addWidget(theTextEdit_specifyDestPort , 0, 2, 1, 1);
	connect(theTextEdit_specifyDestPort, SIGNAL(editingFinished()), this, SLOT(newTextDestPort()));

	theButton_go = new QPushButton(placeHere);
	theButton_go->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
	theButton_go->setProperty("JVX_WHAT_TO_SHOW", qVariantFromValue((int)JVX_LOCAL_SHOW_CONNECT_DISCONNECT));
	theButton_go->setAutoDefault(false);
	theButton_go->setDefault(false);
	font = theButton_go->font();
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theButton_go->setFont(font);
	theButton_go->setText("Start Connection");
	theLayout->addWidget(theButton_go , 0, 3, 1, 1);
	connect(theButton_go, SIGNAL(clicked()), this, SLOT(pushedButtonGo()));

	for(i = 0; i < JVX_NUMBER_STATES_CONNECTION; i++)
	{
		theLabel_connectionState = new QLabel(placeHere);
		theLabel_connectionState->setMinimumSize(QSize(15, 15));
		theLabel_connectionState->setMaximumSize(QSize(15, 15));
		theLabel_connectionState->setFrameShape(QFrame::StyledPanel);
		theLabel_connectionState->setFrameShadow(QFrame::Sunken);
		theLabel_connectionState->setProperty("JVX_WHAT_TO_SHOW", qVariantFromValue((int)JVX_LOCAL_SHOW_CONNECTSTATE_OFF+i));
		font = theLabel_connectionState->font();
		font.setPointSize(10);
		font.setBold(false);
		font.setItalic(true);
		theLabel_connectionState->setFont(font);
		setBackgroundLabelColor(Qt::white, theLabel_connectionState);

		theLayout->addWidget(theLabel_connectionState, 0, 4 + i, 1, 1);
	}
	theLayout->setColumnStretch(0, 1);
	theLayout->setColumnStretch(1, 1);
	theLayout->setColumnStretch(2, 1);
	theLayout->setColumnStretch(3, 1);
	for(i = 0; i < JVX_NUMBER_STATES_CONNECTION; i++)
	{
		theLayout->setColumnStretch(4+i, 1);
	}

	theLabel_performance = new QLabel(placeHere);
	theLabel_performance->setText("Runtime Performancve");
	theLabel_performance->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
    font = theLabel_performance->font();
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theLabel_performance->setFont(font);
	theLayout->addWidget(theLabel_performance, 1, 0, 1, 1);

	theLabel_lostFrames	= new QLabel(placeHere);
	theLabel_lostFrames->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
	theLabel_lostFrames->setText("Lost/Unsent/Remote Frames");
	font = theLabel_lostFrames->font();
	font.setPointSize(10);
	font.setBold(false);
	font.setItalic(true);
	theLabel_lostFrames->setFont(font);
	pal.setBrush(QPalette::Active, QPalette::WindowText, brush_black);
	pal.setBrush(QPalette::Inactive, QPalette::WindowText, brush_black);
	theLabel_lostFrames->setPalette(pal);

	theLayout->addWidget(theLabel_lostFrames, 1, 1, 1, 1);

	theTextEdit_lostFrames = new QLineEdit(placeHere);
	theTextEdit_lostFrames->setProperty("JVX_COMPONENT_TYPE", qVariantFromValue((int)tp));
	theTextEdit_lostFrames->setProperty("JVX_WHAT_TO_SHOW", qVariantFromValue((int)JVX_LOCAL_SHOW_TEXTEDIT_LOST_FRAMES));
	font = theTextEdit_lostFrames->font();
	theTextEdit_lostFrames->setReadOnly(true);
	font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    theTextEdit_lostFrames->setFont(font);
	theLayout->addWidget(theTextEdit_lostFrames , 1, 2, 1, 1);

	placeHere->setLayout(theLayout);
	ptFrame = placeHere;

	pal.setBrush(QPalette::Active, QPalette::Window, brush_gray);
	placeHere->setPalette(pal);
}

void
configureAudio_specific_socket::cleanFields(QFrame* placeHere)
{
	QLayout* theLayout = placeHere->layout();
	QLayoutItem* it = NULL;

	if(theLayout)
	{
		while((it = theLayout->takeAt(0)))
		{
			if(it)
			{
				QWidget* theW = it->widget(); // <- this is the frame
				disconnect(theW);
				delete(theW);
			}
		}
	}
	delete theLayout;
}

void
configureAudio_specific_socket::newTextDestHost()
{
	jvxErrorType res = JVX_NO_ERROR;
	jvx_propertyReferenceTriple theDevProps;
	QObject* obj = sender();
	 QLineEdit* theLe = static_cast<QLineEdit*>(obj);
	 std::string txt = theLe->text().toLatin1().data();
	 jvxString fldStr;
	 fldStr.bString = (char*)txt.c_str();
	 fldStr.lString = txt.size();
	 jvx_initPropertyReferenceTriple(&theDevProps);
	 jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
	 if(jvx_isValidPropertyReferenceTriple(&theDevProps))
	 {
		 res = theDevProps.theProps->set_property(&fldStr, 1, JVX_DATAFORMAT_STRING,  properties.id_sockhostname, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true);
	 }
	 this->updateWindow();
}

void
configureAudio_specific_socket::newTextDestPort()
{
	jvxErrorType res = JVX_NO_ERROR;
	jvx_propertyReferenceTriple theDevProps;
	QObject* obj = sender();
	QLineEdit* theLe = static_cast<QLineEdit*>(obj);
	std::string txt = theLe->text().toLatin1().data();
	jvxString fldStr;
	jvxInt32 port = theLe->text().toInt();
	jvx_initPropertyReferenceTriple(&theDevProps);
	jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
	if(jvx_isValidPropertyReferenceTriple(&theDevProps))
	{
		res = theDevProps.theProps->set_property(&port, 1, JVX_DATAFORMAT_32BIT_LE,  properties.id_sockhostport, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true);
	}
	this->updateWindow();
}

void
configureAudio_specific_socket::pushedButtonGo()
{
	jvxErrorType res = JVX_NO_ERROR;
	jvx_propertyReferenceTriple theDevProps;
	QObject* obj = sender();
	std::string command;
	jvxString fldStr;
	jvxSelectionList selLst;
	selLst.strList = NULL;
	jvxBool isValid;
	jvxState stat = JVX_STATE_NONE;
 	if(theHostRef)
	{
		theHostRef->state(&stat);
		if(stat == JVX_STATE_ACTIVE)
		{
			theHostRef->state_component(JVX_COMPONENT_AUDIO_TECHNOLOGY, &stat);
			if(stat == JVX_STATE_ACTIVE)
			{
				theHostRef->state_component(JVX_COMPONENT_AUDIO_DEVICE, &stat);
				if(stat >= JVX_STATE_ACTIVE)
				{
					jvx_initPropertyReferenceTriple(&theDevProps);
					jvx_getReferencePropertiesObject(theHostRef, &theDevProps, JVX_COMPONENT_AUDIO_DEVICE);
					if(jvx_isValidPropertyReferenceTriple(&theDevProps))
					{
						if(properties.id_sockconnstate >= 0)
						{
							res = theDevProps.theProps->get_property(&selLst, 1, JVX_DATAFORMAT_SELECTION_LIST, properties.id_sockconnstate, JVX_PROPERTY_CATEGORY_UNSPECIFIC, 0, true, &isValid, NULL);
							assert(res == JVX_NO_ERROR);
							switch(selLst.bitFieldSelected)
							{
							case 0x1:
								command = "connect();";
								break;
							default:
								command = "disconnect();";
								break;
							}
							fldStr.bString = (char*)command.c_str();
							fldStr.lString = command.size();
							res = theDevProps.theProps->set_property(&fldStr, 1, JVX_DATAFORMAT_STRING,  properties.id_sockcommand, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, true);
						}
					}
				}
			}
		}
	}
	this->updateWindow();
}

void
configureAudio_specific_socket::setTextAndActiveButton(std::string txt, jvxBool active, jvxInt32 idCompare)
{
	jvxSize i;
	QLayout* theLayout = NULL;
	QLayoutItem* it = NULL;

	if(ptFrame)
	{
		theLayout = ptFrame->layout();
		if(theLayout)
		{
			for(i = 0; i < theLayout->count(); i++)
			{
				it = theLayout->itemAt(i);
				QWidget* theW = it->widget(); // <- this is the frame
				QPushButton* theButton = NULL;
				theButton = static_cast<QPushButton*>(theW);
				if(theButton)
				{
					QVariant qq = theButton->property("JVX_WHAT_TO_SHOW");
					if(qq.isValid())
					{
						int id = qq.toInt();
						if(id == idCompare)
						{
							theButton->setText(txt.c_str());
							theButton->setEnabled(active);
						}
					}
				}
			}
		}
	}
}

void
configureAudio_specific_socket::setBackgroundColorLabelConnectionState_exclusive(int idCompare)
{
	jvxSize i;
	QLayout* theLayout = NULL;
	QLayoutItem* it = NULL;

	if(ptFrame)
	{
		theLayout = ptFrame->layout();
		if(theLayout)
		{
			for(i = 0; i < theLayout->count(); i++)
			{
				it = theLayout->itemAt(i);
				QWidget* theW = it->widget(); // <- this is the frame
				QLabel* theLabel = NULL;
				theLabel = static_cast<QLabel*>(theW);
				if(theLabel)
				{
					QVariant qq = theLabel->property("JVX_WHAT_TO_SHOW");
					if(qq.isValid())
					{
						int id = qq.toInt();
						if(id == idCompare)
						{
							setBackgroundLabelColor(Qt::white, theLabel);
						}
						else
						{
							setBackgroundLabelColor(Qt::gray, theLabel);
						}
					}
				}
			}
		}
	}
}

void
configureAudio_specific_socket::setTextAndActiveLineEdit(std::string txt, jvxBool activate, int idCompare)
{
	jvxSize i;
	QLayout* theLayout = NULL;
	QLayoutItem* it = NULL;

	if(ptFrame)
	{
		theLayout = ptFrame->layout();
		if(theLayout)
		{
			for(i = 0; i < theLayout->count(); i++)
			{
				it = theLayout->itemAt(i);
				QWidget* theW = it->widget(); // <- this is the frame
				QLineEdit* theLineEdit = NULL;
				theLineEdit = static_cast<QLineEdit*>(theW);
				if(theLineEdit)
				{
					QVariant qq = theLineEdit->property("JVX_WHAT_TO_SHOW");
					if(qq.isValid())
					{
						int id = qq.toInt();
						if(id == idCompare)
						{
							theLineEdit->setText(txt.c_str());
							theLineEdit->setEnabled(activate);
						}
					}
				}
			}
		}
	}
}
