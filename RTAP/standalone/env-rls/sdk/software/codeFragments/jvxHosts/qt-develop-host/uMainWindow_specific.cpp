#include "uMainWindow.h"
#include "uMainWindow_widgets.h"

#include "jvxQtDevelopHostHookupEntries.h"
jvxErrorType
uMainWindow::bootup_specific()
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;
		
	// Set firwst and afterwards request some additional features from host
	theHostFeatures.showExpertUi_seq = true; // Default: true
	theHostFeatures.showExpertUi_props = true; // Default: true
	theHostFeatures.showExpertUi_plots = true; // Default: true
	theHostFeatures.showExpertUi_messages = true; // Default: true
	theHostFeatures.timeout_viewer_ms = 500; // Default: 500


	configureDevelopHost_features_request(reinterpret_cast<jvxHandle*>(&theHostFeatures));
	/*
	* Add the configuration lines here
	*/
	IjvxConfigurationLine* theConfigLines = NULL;

	res = this->involvedComponents.theHost.hHost->request_hidden_interface(JVX_INTERFACE_CONFIGURATIONLINE, reinterpret_cast<jvxHandle**>(&theConfigLines));
	if((res == JVX_NO_ERROR) && theConfigLines)
	{
		theConfigLines->add_line(JVX_COMPONENT_AUDIO_DEVICE, "Use the audio parameters from the audio device in the audio node and the camera");
		theConfigLines->add_slave_line(0, JVX_COMPONENT_AUDIO_NODE);

		for(i = 0; i < JVX_NUM_CONFIGURATIONLINE_PROPERTIES_AUDIO; i++)
		{
			jvxSize id = 0;
			if(jvx_findPropertyDescriptor(jvxProperties_configLine_audio[i], &id, NULL, NULL, NULL))
			{
				theConfigLines->add_configuration_property_line(0, id);
			}

		}

		this->involvedComponents.theHost.hHost->return_hidden_interface(JVX_INTERFACE_CONFIGURATIONLINE, reinterpret_cast<jvxHandle*>(theConfigLines));
	}


	/*
	* Add the types to be handled by host
	*/
	IjvxHostTypeHandler* theTypeHandler = NULL;
	res = this->involvedComponents.theHost.hHost->request_hidden_interface(JVX_INTERFACE_HOSTTYPEHANDLER, reinterpret_cast<jvxHandle**>(&theTypeHandler));
	if((res == JVX_NO_ERROR) && theTypeHandler)
	{
		jvxComponentType tp[2];

		tp[0] = JVX_COMPONENT_AUDIO_TECHNOLOGY;
		tp[1] = JVX_COMPONENT_AUDIO_DEVICE;
		res = theTypeHandler->add_type_host(tp, 2, "Jvx Audio Technologies", "audio", JVX_COMPONENT_TYPE_TECHNOLOGY);
		assert(res == JVX_NO_ERROR);

		tp[0] = JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY;
		tp[1] = JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE;
		res = theTypeHandler->add_type_host(tp, 2, "Jvx External Controller Technologies", "ex_control", JVX_COMPONENT_TYPE_TECHNOLOGY);
		assert(res == JVX_NO_ERROR);

		tp[0] = JVX_COMPONENT_AUDIO_NODE;
		tp[1] = JVX_COMPONENT_UNKNOWN;
		res = theTypeHandler->add_type_host(tp, 1, "Jvx Audio Node", "audio_node", JVX_COMPONENT_TYPE_NODE);
		assert(res == JVX_NO_ERROR);

		/*			theTypeHandler->add_type_host(JVX_COMPONENT_SCANNER_TECHNOLOGY, "", "camera", JVX_COMPONENT_TYPE_TECHNOLOGY);
		theTypeHandler->add_type_host(JVX_COMPONENT_SCANNER_TECHNOLOGY, "", "lamp", JVX_COMPONENT_TYPE_TECHNOLOGY);
		theTypeHandler->add_type_host(JVX_COMPONENT_SCANNER_TECHNOLOGY, "", "spectrum_processor", JVX_COMPONENT_TYPE_TECHNOLOGY);

		theTypeHandler->add_type_host(JVX_COMPONENT_SCANNER_TECHNOLOGY, "", "", JVX_COMPONENT_TYPE_ENDPOINT);
		*/
		this->involvedComponents.theHost.hHost->return_hidden_interface(JVX_INTERFACE_HOSTTYPEHANDLER, reinterpret_cast<jvxHandle*>(theTypeHandler));
	}

	// Reference set before..
	//this->involvedComponents.theHost.hHost->add_external_interface(reinterpret_cast<jvxHandle*>(static_cast<IjvxQtWidgetHost*>(this)), JVX_INTERFACE_QT_WIDGET_HOST);

	// Do not allow that host components are loaded via DLL
	this->involvedComponents.theHost.hHost->add_component_load_blacklist(JVX_COMPONENT_HOST);

		if(theHostFeatures.showExpertUi_seq)
	{
		subWidgets.sequences.theWidgetD->show();
	}
	else
	{
		subWidgets.sequences.theWidgetD->hide();
	}
	if(theHostFeatures.showExpertUi_props)
	{
		subWidgets.realtimeViewer.props.theWidgetD->show();
	}
	else
	{
		subWidgets.realtimeViewer.props.theWidgetD->hide();
	}
	if(theHostFeatures.showExpertUi_plots)
	{
		subWidgets.realtimeViewer.plots.theWidgetD->show();
	}
	else
	{
		subWidgets.realtimeViewer.plots.theWidgetD->hide();
	}
	if(theHostFeatures.showExpertUi_messages)
	{
		subWidgets.messages.theWidgetD->show();
	}
	else
	{
		subWidgets.messages.theWidgetD->hide();
	}

	systemParams.rtv_period_central_ms = theHostFeatures.timeout_viewer_ms; 
	return(res);
}

jvxErrorType
uMainWindow::postbootup_specific()
{
	if(this->subWidgets.sequences.theWidget)
	{
		this->subWidgets.sequences.theWidget->addDefaultSequence(true);
	}
	return(JVX_NO_ERROR);
}

void
uMainWindow::bootDelayed_specific()
{
	jvxBool wantsToAdjust = false;
	subWidgets.main.theWidget->inform_bootup_complete(&wantsToAdjust);
	if(wantsToAdjust)
	{
		this->adjustSize();
	}
}
// ==========================================================================================

void
uMainWindow::updateWindow_components()
{
	jvxState stat = JVX_STATE_NONE;
	int idItalic = -1;

	menuUpdate_oneComponentType(menuAvailable_audiotechnologies,
		menuStatus_audiotechnology, menuProperties_audiotechnology,
		menuCommands_audiotechnology, menuConditions_audiotechnology,
		JVX_COMPONENT_AUDIO_TECHNOLOGY);

	menuUpdate_oneComponentType(menuAvailable_audiodevices,
		menuStatus_audiodevice, menuProperties_audiodevice,
		menuCommands_audiodevice, menuConditions_audiodevice,
		JVX_COMPONENT_AUDIO_DEVICE);

	menuUpdate_oneComponentType(menuAvailable_excontrollertechnologies,
		menuStatus_excontrollertechnology, menuProperties_excontrollertechnology,
		menuCommands_excontrollertechnology, menuConditions_excontrollertechnology,
		JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY);

	menuUpdate_oneComponentType(menuAvailable_excontrollerdevices,
		menuStatus_excontrollerdevice, menuProperties_excontrollerdevice,
		menuCommands_excontrollerdevice, menuConditions_excontrollerdevice,
		JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE);

	menuUpdate_oneComponentType(menuAvailable_audionodes,
		menuStatus_audionode, menuProperties_audionode,
		menuCommands_audionode, menuConditions_audionode,
		JVX_COMPONENT_AUDIO_NODE);
}

jvxErrorType
uMainWindow::shutdown_specific()
{
	// This interface is handled separately.
	// this->involvedComponents.theHost.hHost->remove_external_interface(reinterpret_cast<jvxHandle*>(static_cast<IjvxQtWidgetHost*>(this)), JVX_INTERFACE_QT_WIDGET_HOST);
	return(JVX_NO_ERROR);
}

jvxErrorType
uMainWindow::connect_specific()
{

	actionInformation_exconttechnology->setProperty("task", QVariant(70));
	actionInformation_exconttechnology->setProperty("identification", QVariant((int)JVX_COMPONENT_AUDIO_NODE));
	actionInformation_exconttechnology->setProperty("subid", QVariant(-1));
	connect(actionInformation_exconttechnology, SIGNAL(triggered()),this, SLOT(action_int_selection_slot()));

	actionInformation_excontdevice->setProperty("task", QVariant(70));
	actionInformation_excontdevice->setProperty("identification", QVariant((int)JVX_COMPONENT_AUDIO_NODE));
	actionInformation_excontdevice->setProperty("subid", QVariant(-1));
	connect(actionInformation_excontdevice, SIGNAL(triggered()),this, SLOT(action_int_selection_slot()));


	actionInformation_audiotechnology->setProperty("task", QVariant(70));
	actionInformation_audiotechnology->setProperty("identification", QVariant((int)JVX_COMPONENT_AUDIO_NODE));
	actionInformation_audiotechnology->setProperty("subid", QVariant(-1));
	connect(actionInformation_audiotechnology, SIGNAL(triggered()),this, SLOT(action_int_selection_slot()));

	actionInformation_audiodevice->setProperty("task", QVariant(70));
	actionInformation_audiodevice->setProperty("identification", QVariant((int)JVX_COMPONENT_AUDIO_NODE));
	actionInformation_audiodevice->setProperty("subid", QVariant(-1));
	connect(actionInformation_audiodevice, SIGNAL(triggered()),this, SLOT(action_int_selection_slot()));

	actionInformation_audionode->setProperty("task", QVariant(70));
	actionInformation_audionode->setProperty("identification", QVariant((int)JVX_COMPONENT_AUDIO_NODE));
	actionInformation_audionode->setProperty("subid", QVariant(-1));
	connect(actionInformation_audionode, SIGNAL(triggered()),this, SLOT(action_int_selection_slot()));


	return(JVX_NO_ERROR);
}
