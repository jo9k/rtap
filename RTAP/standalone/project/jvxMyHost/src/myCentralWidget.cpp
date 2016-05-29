#include "myCentralWidget.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

void
myCentralWidget::init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* bwd, IjvxMainWindowControl* ctrl)
{
	this->setupUi(this);
	mainCentral::init_submodule(theHost, bwd, ctrl);
}


	void myCentralWidget::onANGLE()
	{
	jvxData ANGLE;
	QString str = lineEdit_ANGLE->text();
	ANGLE = str.toDouble();

	jvx_propertyReferenceTriple theTripleNode;
	jvxErrorType res = jvx_getReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	if(res == JVX_NO_ERROR)
	{
		if(jvx_isValidPropertyReferenceTriple(&theTripleNode))
		{
			jvx_set_property(theTripleNode.theProps, &ANGLE, 0, 1, JVX_DATAFORMAT_DATA, true, "ANGLE");
		}
		jvx_returnReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	}
	inform_update_window();
	}

	void myCentralWidget::onCFAC()
	{
	jvxData CFAC;
	QString str = lineEdit_CFAC->text();
	CFAC = str.toDouble();

	jvx_propertyReferenceTriple theTripleNode;
	jvxErrorType res = jvx_getReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	if(res == JVX_NO_ERROR)
	{
		if(jvx_isValidPropertyReferenceTriple(&theTripleNode))
		{
			jvx_set_property(theTripleNode.theProps, &CFAC, 0, 1, JVX_DATAFORMAT_DATA, true, "CFAC");
		}
		jvx_returnReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	}
	inform_update_window();	
	}

	void myCentralWidget::inform_update_window()
{
	jvxData ANGLE = 0.0;
	jvxData CFAC = 0.0;

	jvx_propertyReferenceTriple theTripleNode;
	jvxErrorType res = jvx_getReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	if(res == JVX_NO_ERROR)
	{
		if(jvx_isValidPropertyReferenceTriple(&theTripleNode))
		{
			jvx_get_property(theTripleNode.theProps, &ANGLE, 0, 1, JVX_DATAFORMAT_DATA, true, "ANGLE");
			jvx_get_property(theTripleNode.theProps, &CFAC, 0, 1, JVX_DATAFORMAT_DATA, true, "CFAC");
		}
		jvx_returnReferencePropertiesObject(theHostRef, &theTripleNode, JVX_COMPONENT_AUDIO_NODE);
	}

	lineEdit_ANGLE->setText(jvx_data2String(ANGLE, 4).c_str());
	lineEdit_CFAC->setText(jvx_data2String(CFAC, 4).c_str());
}


#ifdef JVX_PROJECT_NAMESPACE
}
#endif
