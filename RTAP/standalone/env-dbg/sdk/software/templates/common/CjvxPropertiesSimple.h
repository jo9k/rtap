#ifndef __CJVXPROPERTIESSIMPLE_H__
#define __CJVXPROPERTIESSIMPLE_H__

#include "jvx.h"
#include "HjvxHostsAndProperties.h"
#include <map>
#include <iostream>
//#define CJVXPROPERTIESSIMPLIFY_VERBOSE_OUTPUT

class CjvxPropertiesSimple
{
 public:

        CjvxPropertiesSimple()
                {
                        // set pointers in triple-node to NULL
                        jvx_initPropertyReferenceTriple(&theTripleNode);

                        theHostRef = NULL;
                        tpRef = JVX_COMPONENT_UNKNOWN;
                }

        virtual jvxErrorType get_property_id_handle(std::string name, jvxSize *idx, IjvxProperties** theProp)
        {
                jvxBool isValid;
                jvxSize num2 = 0;
                jvxPropertyCategoryType category;
                jvxErrorType res;
                jvxDataFormat format2;
                jvxSize hdlIdx;

                res = refresh_properties();
                if(res != JVX_NO_ERROR)
                        {
                                reportErrorGet(name, res);
                                return res;
                        }


                // retrieve & check property description
                res = theTripleNode.theProps->description_property(propidmap[name],
                                                                   &category, NULL, NULL, &format2, &num2, NULL, NULL,
                                                                   &hdlIdx, NULL, NULL, NULL, NULL, &isValid);
                if(res == JVX_NO_ERROR)
                        {
                                if(idx)
                                        {
                                                *idx = hdlIdx;
                                        }
                                if(theProp)
                                        {
                                                *theProp = theTripleNode.theProps;
                                        }
                        }
                return(res);
        }

	virtual jvxErrorType get_property(std::string name, jvxInt16 *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return get_property_typesafe(name, val, JVX_DATAFORMAT_16BIT_LE, num, offset);
	}

	virtual jvxErrorType get_property(std::string name, jvxInt32 *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return get_property_typesafe(name, val, JVX_DATAFORMAT_32BIT_LE, num, offset);
	}

	virtual jvxErrorType get_property(std::string name, jvxSize *val)
	{
		jvxSelectionList tmp;
		jvxErrorType res;
                tmp.strList = NULL;
		res = get_property_typesafe(name, &tmp, JVX_DATAFORMAT_SELECTION_LIST, 1 ,0);
		if(res != JVX_NO_ERROR)
			return res;
		*val = 0;
		while(tmp.bitFieldSelected>>=1)
			(*val)++;
		return res;
	}

	virtual jvxErrorType get_property(std::string name, std::string &val)
	{
		jvxString *tmp;
		jvxErrorType res;
		res = get_property_typesafe(name, &tmp, JVX_DATAFORMAT_STRING, 1, 0);
		if(res != JVX_NO_ERROR)
			return res;
		val = tmp->bString;
		deallocate(tmp);
		return res;
	}

	virtual jvxErrorType get_property(std::string name, jvxData *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return get_property_typesafe(name, val, JVX_DATAFORMAT_DATA, num, offset);
	}

	virtual jvxErrorType set_property(std::string name, jvxInt16 *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return set_property_typesafe(name, val, JVX_DATAFORMAT_16BIT_LE, num, offset);
	}

	virtual jvxErrorType set_property(std::string name, jvxInt32 *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return set_property_typesafe(name, val, JVX_DATAFORMAT_32BIT_LE, num, offset);
	}

	virtual jvxErrorType set_property(std::string name, jvxSize *val)
	{
		jvxSelectionList tmp;

		tmp.bitFieldSelected = 1 << *val;
		return set_property_typesafe(name, &tmp, JVX_DATAFORMAT_SELECTION_LIST, 1 ,0);
	}

	virtual jvxErrorType set_property(std::string name, jvxString *val)
	{
		return set_property_typesafe(name, val, JVX_DATAFORMAT_STRING, 1, 0);
	}

	virtual jvxErrorType set_property(std::string name, jvxData *val, jvxSize num = 1, jvxSize offset = 0)
	{
		return set_property_typesafe(name, val, JVX_DATAFORMAT_DATA, num, offset);
	}


        jvxErrorType get_property_typesafe(std::string name,
                                           jvxHandle *val,
                                           jvxDataFormat format,
                                           jvxSize num = 1,
                                           jvxSize offset = 0,
                                           jvxBool contentOnly = true)
        {
                jvxBool isValid;
                jvxSize num2 = 0;
                jvxPropertyCategoryType category;
                jvxErrorType res;
                jvxDataFormat format2;
                jvxSize hdlIdx;
                std::map<std::string,int>::iterator it;

                res = refresh_properties();
                if(res != JVX_NO_ERROR)
                        {
                                reportErrorGet(name, res);
                                return res;
                        }

                it = propidmap.find(name);
                if(it == propidmap.end())
                        {
                                reportErrorGet(name, JVX_ERROR_INVALID_ARGUMENT, "unknown property "+name);
                                return JVX_ERROR_INVALID_ARGUMENT;
                        }

                // retrieve & check property description
                res = theTripleNode.theProps->description_property(propidmap[name],
                                                                   &category, NULL, NULL, &format2, &num2, NULL, NULL,
                                                                   &hdlIdx, NULL, NULL, NULL, NULL, &isValid);

                // num == -1 means get all (val must contain enough space)
                if(num == -1)
                        num = num2;

                if(res != JVX_NO_ERROR)
                        {
                                reportErrorGet(name, res);
                                return res;
                        }
                if(num2 < num)
                        {
                                reportErrorGet(name, JVX_ERROR_SIZE_MISMATCH,
                                               "requested "+ jvx_int2String(num) +
                                               ", but stored only "+ jvx_int2String(num2));
                                return JVX_ERROR_SIZE_MISMATCH;
                        }
                if(format != format2)
                        {
                                reportErrorGet(name, JVX_ERROR_INVALID_ARGUMENT,
                                               "requested format "+std::string(jvxDataFormat_str[format]) +
                                               ", but stored format is "+std::string(jvxDataFormat_str[format2]) );
                                return JVX_ERROR_INVALID_ARGUMENT;
                        }
                if(!isValid)
                        {
                                reportErrorGet(name, JVX_ERROR_WRONG_STATE);
                                return JVX_ERROR_WRONG_STATE;
                        }

                // retrieve the property
                res = theTripleNode.theProps->get_property(val, num,
                                                           format, hdlIdx,
                                                           category, offset, contentOnly, &isValid, NULL);
                reportErrorGet(name,res);
                return res;
        }

        jvxErrorType deallocate(jvxStringList* lst)
        {
                if(jvx_isValidPropertyReferenceTriple(&theTripleNode))
                        return theTripleNode.theObj->deallocate(lst);
                return JVX_ERROR_WRONG_STATE;
        }

        jvxErrorType deallocate(jvxString* str)
        {
                if(jvx_isValidPropertyReferenceTriple(&theTripleNode))
                        return theTripleNode.theObj->deallocate(str);
                return JVX_ERROR_WRONG_STATE;
        }

        jvxErrorType set_property_typesafe(std::string name,
                                           jvxHandle *val,
                                           jvxDataFormat format,
                                           jvxSize num = 1,
                                           jvxSize offset = 0,
                                           jvxBool contentOnly = true)
        {
                jvxErrorType res;
                jvxPropertyAccessType accessType;
                jvxBool isValid;
                jvxSize num2 = 0;
                jvxPropertyCategoryType category;
                jvxDataFormat format2;
                jvxSize hdlIdx;
                std::map<std::string,int>::iterator it;

                res = refresh_properties();
                if(res != JVX_NO_ERROR)
                        {
                                reportErrorSet(name, res);
                                return res;
                        }

                it = propidmap.find(name);
                if(it == propidmap.end())
                        {
                                reportErrorSet(name, JVX_ERROR_INVALID_ARGUMENT, "unknown property "+name);
                                return JVX_ERROR_INVALID_ARGUMENT;
                        }

                // retrieve & check property description
                res = theTripleNode.theProps->description_property(propidmap[name],
                                                                   &category, NULL, NULL, &format2, &num2, &accessType, NULL,
                                                                   &hdlIdx, NULL, NULL, NULL, NULL, &isValid);
                if(res != JVX_NO_ERROR)
                        {
                                reportErrorSet(name, res);
                                return res;
                        }
                if(num2 < num)
                        {
                                reportErrorSet(name, JVX_ERROR_SIZE_MISMATCH,
                                               "requested "+ jvx_int2String(num) +
                                               ", but stored only "+ jvx_int2String(num2));
                                return JVX_ERROR_SIZE_MISMATCH;
                        }
                if(format != format2)
                        {
                                reportErrorSet(name, JVX_ERROR_INVALID_ARGUMENT,
                                               "requested format "+std::string(jvxDataFormat_str[format]) +
                                               ", but stored format is "+std::string(jvxDataFormat_str[format2]) );
                                return JVX_ERROR_INVALID_ARGUMENT;
                        }
                if(!isValid)
                        {
                                reportErrorSet(name, JVX_ERROR_WRONG_STATE);
                                return JVX_ERROR_WRONG_STATE;
                        }
                if(accessType == JVX_PROPERTY_ACCESS_READ_ONLY)
                        {
                                reportErrorSet(name, JVX_ERROR_READ_ONLY);
                                return JVX_ERROR_READ_ONLY;
                        }

                // set the property
                res = theTripleNode.theProps->set_property(val, num,
                                                           format, hdlIdx,
                                                           category, offset, contentOnly);

                reportErrorSet(name, res);
                return res;
        }

	virtual jvxErrorType init_properties(IjvxHost *theHost, jvxComponentType tp)
	{
		jvxErrorType res;

		theHostRef = theHost;
		tpRef = tp;

		assert(theHost);
		res = jvx_getReferencePropertiesObject(theHostRef,
						       &theTripleNode,
						       tpRef);

		if(res != JVX_NO_ERROR)
			return res;

		savedPropRevision = -1;

		res = refresh_properties();

		return res;
	}

        virtual jvxErrorType terminate_properties()
        {
                jvxErrorType res;

                propidmap.clear();

                assert(theHostRef);
                res = jvx_returnReferencePropertiesObject(theHostRef,
                                                          &theTripleNode,
                                                          tpRef);

                // set pointers in triple-node to NULL
                jvx_initPropertyReferenceTriple(&theTripleNode);
                savedPropRevision = -1;
                return(res);
        }

 private:

        void reportErrorGet(std::string name, jvxErrorType err, std::string info="")
        {
                if(err != JVX_NO_ERROR)
                        {
                                std::cerr << "ERROR: failed to get property '"<<name<<"': " << jvxErrorType_str[err] << std::endl;
                                if(info != "")
                                        std::cerr << "       "<<info<< std::endl;
                        }
        }

        void reportErrorSet(std::string name, jvxErrorType err, std::string info="")
        {
                if(err != JVX_NO_ERROR)
                        {
                                std::cerr << "ERROR: failed to set property '"<<name<<"': " << jvxErrorType_str[err] << std::endl;
                                if(info != "")
                                        std::cerr << "       "<<info<< std::endl;
                        }
        }

	jvxErrorType refresh_properties()
	{
		jvxErrorType res;
		jvxBool isValid;
		jvxSize hdlIdx;
		jvxString* descror = NULL;
		std::string str;
		jvxSize num = 0;
		jvxSize i;
		jvxSize propRevision;
		jvxDataFormat format;
		jvxPropertyAccessType accessType;

		theTripleNode.theProps->get_revision(&propRevision);
		if(propRevision == savedPropRevision)
			return JVX_NO_ERROR; // no refresh needed

		savedPropRevision = propRevision;

		propidmap.clear();

#ifdef CJVXPROPERTIESSIMPLIFY_VERBOSE_OUTPUT
                std::cout << "COMPONENT " << jvxComponentType_txt(tpRef) << std::endl;
#endif

                theTripleNode.theProps->number_properties(&num);

		for(i = 0; i < num; i++)
			{
				res = theTripleNode.theProps->description_property(i, NULL, NULL, NULL, &format, NULL, &accessType, NULL,
										   &hdlIdx, NULL, NULL, NULL, &descror, &isValid);
				if(res != JVX_NO_ERROR)
					return res;

				str = descror->bString;
				theTripleNode.theObj->deallocate(descror);
				propidmap[str] = i;

#ifdef CJVXPROPERTIESSIMPLIFY_VERBOSE_OUTPUT
                                std::cout << "---> PROPERTY " <<  str;
                                if(accessType == JVX_PROPERTY_ACCESS_READ_ONLY)
					std::cout<< " (read only)";
				std::cout<< ", idx: " << i << ", hdlIdx: " << hdlIdx << ", format: " << format << std::endl;
#endif
                        }
		return JVX_NO_ERROR;
	}

        IjvxHost *theHostRef;
        jvxComponentType tpRef;

	jvx_propertyReferenceTriple theTripleNode;
	std::map<std::string,int> propidmap;
	jvxSize savedPropRevision;

};

#endif
