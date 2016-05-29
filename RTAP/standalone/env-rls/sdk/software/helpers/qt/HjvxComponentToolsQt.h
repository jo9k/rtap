#ifndef __HJVXCOMPONENENTTOOLSQT_H__
#define __HJVXCOMPONENENTTOOLSQT_H__

#include "helpers/HjvxComponentPreselection.h"

static void jvx_qt_listComponentsInComboBox(bool addressesStandardComponents, QComboBox* combo, IjvxHost* hostRef, jvxSize index)
{
	jvxSize i;
	IjvxObject* theObj = NULL;

	combo->clear();
	for(i = 0; i < JVX_COMPONENT_LIMIT; i++)
	{
		combo->addItem(jvxComponentType_txt(i).c_str());
	}

	for(i = 0; i < JVX_COMPONENT_LIMIT; i++)
	{
		QFont myCustomFont;

		// Ok, we know that this component has been selected
		theObj = NULL;
		jvx_getReferenceComponentType((jvxComponentType)i, theObj, addressesStandardComponents, hostRef);

		if(theObj)
		{
			myCustomFont.setBold(true);
			myCustomFont.setItalic(false);
			jvx_returnReferenceComponentType((jvxComponentType)i, theObj, addressesStandardComponents, hostRef);
		}
		else
		{
			myCustomFont.setBold(false);
			myCustomFont.setItalic(true);
		}
#ifdef JVX_OS_LINUX
		combo->setEditable(true);
#endif
		combo->setItemData(i, QVariant(myCustomFont), Qt::FontRole);
#ifdef JVX_OS_LINUX
		combo->setEditable(false);
#endif
	}
	combo->setCurrentIndex(index);
}

static void jvx_qt_listPropertiesInComboBox(jvxComponentType tp, jvxPropertyContext ctxtShow, jvxInt16 whatToShow, QComboBox* combo, IjvxHost* hostRef, jvxInt16& indexShow, jvxInt16* indexComponent, jvxRealtimeViewerFilterShow filter)
{
	jvxSize i;
	jvx_propertyReferenceTriple theTriple;
	jvxSize num = 0;
	jvxString* fldStr = NULL;

	jvxPropertyCategoryType cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
	jvxUInt64 allowStates = 0;
	jvxUInt64 allowThreads = 0;
	jvxDataFormat format = JVX_DATAFORMAT_NONE;
	jvxSize num_values = 0;
	jvxPropertyAccessType accessType = JVX_PROPERTY_ACCESS_READ_ONLY;
	jvxPropertyDecoderHintType decHtTp = JVX_PROPERTY_DECODER_NONE;
	jvxSize hdlIdx = 0;
	jvxPropertyContext ctxt = JVX_PROPERTY_CONTEXT_UNKNOWN;

	bool conditionForAdd = false;
	std::string entry;
	QFont myCustomFont;
	jvxBool showActiveOnly = false;

	jvx_initPropertyReferenceTriple(&theTriple);
	combo->clear();

	jvx_getReferencePropertiesObject(hostRef, &theTriple, tp);

	if(theTriple.theProps)
	{
		theTriple.theProps->number_properties(&num);
		if(num > 0)
		{
			for(i = 0; i < num; i++)
			{
				fldStr = NULL;
				entry = "UNKNOWN";
				if(whatToShow == 1)
				{
					theTriple.theProps->description_property(i, &cat, &allowStates, &allowThreads, &format, &num_values, &accessType, &decHtTp, &hdlIdx, &ctxt, NULL, NULL, &fldStr);
				}
				else if(whatToShow == 2)
				{
					theTriple.theProps->description_property(i, &cat, &allowStates, &allowThreads, &format, &num_values, &accessType, &decHtTp, &hdlIdx, &ctxt, &fldStr, NULL, NULL);
				}
				else
				{
					theTriple.theProps->description_property(i, &cat, &allowStates, &allowThreads, &format, &num_values, &accessType, &decHtTp, &hdlIdx, &ctxt, NULL, &fldStr, NULL);
				}

				if(fldStr)
				{
					entry = fldStr->bString;
					//entry = jvxPropertyContext_prefix(ctxt) + entry;
					theTriple.theObj->deallocate(fldStr);
				}

				conditionForAdd = jvx_applyPropertyFilter(format, num_values, decHtTp, filter);

				if(conditionForAdd)
				{
					if(ctxtShow != JVX_PROPERTY_CONTEXT_UNKNOWN)
					{
						showActiveOnly = true;
						if(ctxt != ctxtShow)
						{
							conditionForAdd = false;
						}
					}
				}
				if(conditionForAdd)
				{
					combo->addItem(entry.c_str());
					combo->setItemData(combo->count()-1, QVariant((int)i), Qt::UserRole);

					myCustomFont.setBold(true);
					myCustomFont.setItalic(false);
#ifdef JVX_OS_LINUX
					combo->setEditable(true);
#endif
					combo->setItemData(i, QVariant(myCustomFont), Qt::FontRole);
#ifdef JVX_OS_LINUX
					combo->setEditable(false);
#endif
				}
				else
				{
					if(!showActiveOnly)
					{
						combo->addItem(entry.c_str());
						combo->setItemData(combo->count()-1, QVariant((int)i), Qt::UserRole);

						myCustomFont.setBold(false);
						myCustomFont.setItalic(true);
#ifdef JVX_OS_LINUX
						combo->setEditable(true);
#endif
						combo->setItemData(i, QVariant(myCustomFont), Qt::FontRole);
#ifdef JVX_OS_LINUX
						combo->setEditable(false);
#endif
					}
				}
			}
			if(combo->count() >= 1)
			{
				if((indexShow < 0) || (indexShow >= combo->count()))
				{
					indexShow = 0;
				}
				combo->setCurrentIndex(indexShow);
				if(indexComponent)
				{
					QVariant var = combo->itemData(indexShow, Qt::UserRole);
					*indexComponent = var.toInt();
				}
			}
			else
			{
				indexShow = -1;
				if(indexComponent)
				{
					*indexComponent = -1;
				}
			}
		}
		else
		{
			indexShow = -1;
			if(indexComponent)
			{
				*indexComponent = -1;
			}
		}
		jvx_returnReferencePropertiesObject(hostRef, &theTriple, tp);
	}
	else
	{
		indexShow = -1;
		if(indexComponent)
		{
			*indexComponent = -1;
		}
	}
}

static void jvx_qt_propertiesIndexShowForIndexComponent(jvxComponentType tp, IjvxHost* hostRef, jvxInt16& indexShow, jvxInt16 indexComponent, jvxRealtimeViewerFilterShow filter)
{
	jvxSize i;
	jvx_propertyReferenceTriple theTriple;
	jvxSize num = 0;
	jvxString* fldStr = NULL;

	jvxPropertyCategoryType cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
	jvxUInt64 allowStates = 0;
	jvxUInt64 allowThreads = 0;
	jvxDataFormat format = JVX_DATAFORMAT_NONE;
	jvxSize num_values = 0;
	jvxPropertyAccessType accessType = JVX_PROPERTY_ACCESS_READ_ONLY;
	jvxPropertyDecoderHintType decHtTp = JVX_PROPERTY_DECODER_NONE;
	jvxSize hdlIdx = 0;
	jvxPropertyContext ctxt = JVX_PROPERTY_CONTEXT_UNKNOWN;

	bool conditionForAdd = false;
	std::string entry;
	int cnt = 0;
	indexShow = -1;

	jvx_initPropertyReferenceTriple(&theTriple);
	jvx_getReferencePropertiesObject(hostRef, &theTriple, tp);

	if(theTriple.theProps)
	{
		theTriple.theProps->number_properties(&num);
		if(num > 0)
		{
			for(i = 0; i < num; i++)
			{
				fldStr = NULL;
				theTriple.theProps->description_property(i, &cat, &allowStates, &allowThreads, &format, &num_values, &accessType, &decHtTp, &hdlIdx, &ctxt, NULL, NULL, NULL);

				/*
				if(fldStr)
				{
					entry = fldStr->bString;
					entry = jvxPropertyContext_prefix(ctxt) + entry;
					theTriple.theObj->deallocate(fldStr);
				}
				*/
				conditionForAdd = jvx_applyPropertyFilter(format, num_values, decHtTp, filter);

				if(conditionForAdd)
				{
					if(i == indexComponent)
					{
						indexShow = cnt;
					}
				}
				cnt++;
			}
		}
		jvx_returnReferencePropertiesObject(hostRef, &theTriple, tp);
	}
}

#endif
