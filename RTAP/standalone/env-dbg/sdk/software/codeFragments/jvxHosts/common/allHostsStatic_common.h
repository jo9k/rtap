#ifndef __ALLHOSTSSTATIC_COMMON_H__
#define __ALLHOSTSSTATIC_COMMON_H__

typedef struct
{
	IjvxObject* theStaticObject;
	jvxInitObject_tp funcInit;
	jvxTerminateObject_tp funcTerm;
} oneAddedStaticComponent;

#define LOAD_ONE_MODULE_LIB_SIMPLE(FUNC, DESRIPTION, HOST_REF)		\
	addMe = NULL;							\
	res = FUNC(&addMe);						\
	assert(res == JVX_NO_ERROR);					\
	assert(addMe);							\
	res = HOST_REF->add_external_component(addMe, "static host component"); \
	assert(res == JVX_NO_ERROR);

#define LOAD_ONE_MODULE_LIB_FULL(FUNCI, FUNCT, DESRIPTION, LST_COMP, HOST_REF) \
	comp.funcInit = FUNCI;						\
	comp.funcTerm = FUNCT;						\
	comp.theStaticObject = NULL;					\
	res = FUNCI(&comp.theStaticObject);				\
	assert(res == JVX_NO_ERROR);					\
	assert(comp.theStaticObject);					\
	res = HOST_REF->add_external_component(comp.theStaticObject, DESRIPTION, true, comp.funcInit, comp.funcTerm); \
	assert(res == JVX_NO_ERROR);					\
	LST_COMP.push_back(comp);

typedef struct
{
	jvxSequencerQueueType seqType;
	jvxSequencerElementType tpElm;
	jvxComponentType tpComp;
	std::string descr;
	std::string label_step;

	jvxSize funcId;
	jvxInt64 timeout;
	std::string label_cond1;
	std::string label_cond2;
} oneSequencerStep;

// ##################################################################################
#define FIND_PROPERTY_ID(theT, idx, TOKEN, isV, found)			\
	{								\
		jvxSize i;						\
		jvxSize num = 0;					\
		jvxString* descror = NULL;				\
		std::string str;					\
		theT.theProps->number_properties(&num);			\
		for(i = 0; i < num; i++)				\
			{						\
				theT.theProps->description_property(i, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &idx, NULL, NULL, NULL, &descror, &isV); \
				str = descror->bString;			\
				theT.theObj->deallocate(descror);	\
				if(str == TOKEN)			\
					{				\
						found = true;		\
						break;			\
					}				\
			}						\
	}

#define FIND_PROPERTY_ID_SZ(theT, idx, hdlSz, TOKEN, isV, found)	\
	{								\
		jvxSize i;						\
		jvxSize num = 0;					\
		jvxString* descror = NULL;				\
		std::string str;					\
		theT.theProps->number_properties(&num);			\
		for(i = 0; i < num; i++)				\
			{						\
				theT.theProps->description_property(i, NULL, NULL, NULL, NULL, hdlSz, NULL, NULL, &idx, NULL, NULL, NULL, &descror, &isV); \
				str = descror->bString;			\
				theT.theObj->deallocate(descror);	\
				if(str == TOKEN)			\
					{				\
						found = true;		\
						break;			\
					}				\
			}						\
	}

#define FIND_PROPERTY(theT, TOKEN, isV, found)				\
	{								\
		jvxSize i;						\
		jvxSize num = 0;					\
		jvxString* descror = NULL;				\
		std::string str;					\
		theT.theProps->number_properties(&num);			\
		for(i = 0; i < num; i++)				\
			{						\
				theT.theProps->description_property(i, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &hdlIdx, NULL, NULL, NULL, &descror, &isV); \
				str = descror->bString;			\
				theT.theObj->deallocate(descror);	\
				if(str == TOKEN)			\
					{				\
						found = true;		\
						break;			\
					}				\
			}						\
	}

#endif
