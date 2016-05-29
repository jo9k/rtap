#ifndef __CJVXTECHNOLOGY_H__
#define __CJVXTECHNOLOGY_H__

#include "jvx.h"

#include "templates/common/CjvxObject.h"
#include "templates/common/CjvxDeallocate.h"

class CjvxTechnology: public CjvxObject, public CjvxDeallocate
{
protected:

	typedef struct
	{
		IjvxDevice* hdlDev;
	} oneDeviceWrapper;
	
	struct
	{
		std::vector<oneDeviceWrapper> lstDevices;
	} _common_tech_set;
	
public:

	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	// Component class interface member functions
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	JVX_CALLINGCONVENTION CjvxTechnology(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
		CjvxObject(description, multipleObjects, module_name, descriptor, featureClass)
	{
	};

	virtual JVX_CALLINGCONVENTION ~CjvxTechnology(){};
	
	// virtual jvxErrorType JVX_CALLINGCONVENTION _activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION _number_devices(jvxSize* num)
	{
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			if(num)
			{
				*num = _common_tech_set.lstDevices.size();
			}		
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _description_device(jvxSize idx, jvxString** fldStr)
	{
		jvxErrorType res = JVX_ERROR_WRONG_STATE;
		jvxString* fldStrLoc = NULL;
		std::string nm;
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			if(idx < _common_tech_set.lstDevices.size())
			{
				_common_tech_set.lstDevices[idx].hdlDev->description(&fldStrLoc);
				if(fldStrLoc)
				{
					nm = fldStrLoc->bString;
				}
				_common_tech_set.lstDevices[idx].hdlDev->deallocate(fldStrLoc);
				HjvxObject_produceString(fldStr, nm);
				res = JVX_NO_ERROR;
			}		
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _descriptor_device(jvxSize idx, jvxString** fldStr)
	{
		jvxErrorType res = JVX_ERROR_WRONG_STATE;
		jvxString* fldStrLoc = NULL;
		std::string nm;
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			if(idx < _common_tech_set.lstDevices.size())
			{
				_common_tech_set.lstDevices[idx].hdlDev->descriptor(&fldStrLoc);
				if(fldStrLoc)
				{
					nm = fldStrLoc->bString;
				}
				_common_tech_set.lstDevices[idx].hdlDev->deallocate(fldStrLoc);
				HjvxObject_produceString(fldStr, nm);
				res = JVX_NO_ERROR;
			}		
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _module_name_device(jvxSize idx, jvxString** fldStr)
	{
		jvxErrorType res = JVX_ERROR_WRONG_STATE;
		jvxString* fldStrLoc = NULL;
		std::string nm;
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			if(idx < _common_tech_set.lstDevices.size())
			{
				_common_tech_set.lstDevices[idx].hdlDev->module_name(&fldStrLoc);
				if(fldStrLoc)
				{
					nm = fldStrLoc->bString;
				}
				_common_tech_set.lstDevices[idx].hdlDev->deallocate(fldStrLoc);
				HjvxObject_produceString(fldStr, nm);
				res = JVX_NO_ERROR;
			}		
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _request_device(jvxSize idx, IjvxDevice** hdl_onreturn)
	{
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			if(idx < _common_tech_set.lstDevices.size())
			{		
				if(hdl_onreturn)
				{
					*hdl_onreturn = _common_tech_set.lstDevices[idx].hdlDev;
				}
				//*hdl_onreturn = _common_tech_set.lstDevices[idx].hdl;
				return(JVX_NO_ERROR);
			}
			return(JVX_ERROR_ID_OUT_OF_BOUNDS);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _return_device(IjvxDevice* dev)
	{
		jvxSize i;
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{	
			jvxSize idx = _common_tech_set.lstDevices.size();
			for(i = 0; i < _common_tech_set.lstDevices.size(); i++)
			{
				if( dev == _common_tech_set.lstDevices[i].hdlDev)
				{
					idx = i;
					break;
				}
			}

			if(idx < _common_tech_set.lstDevices.size())
			{
				return(JVX_NO_ERROR);
			}
			return(JVX_ERROR_INVALID_ARGUMENT);
		}
		return(JVX_ERROR_WRONG_STATE);
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION _deactivate_cbxt()
	{
		jvxErrorType res = _deactivate();
		if(res == JVX_NO_ERROR)
		{
			// Do whatever is required
			_common_tech_set.lstDevices.clear();
		}
		return(res);
	};
};

#endif