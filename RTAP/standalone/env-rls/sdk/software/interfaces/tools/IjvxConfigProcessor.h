
#ifndef __IJVXCONFIGPROCESSOR_H__
#define __IJVXCONFIGPROCESSOR_H__

JVX_INTERFACE IjvxConfigProcessor: public IjvxObject
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxConfigProcessor(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION parseFile(const char* fName) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION parseTextField(const char* txt, const char* fName, int lineno) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getParseError(jvxError** err) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getConfigurationHandle(jvxConfigData** data) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getNameCurrentEntry(jvxConfigData* data, jvxString** name) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getTypeCurrentEntry(jvxConfigData* data, jvxConfigSectionTypes* type) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getSectionIsEmpty(jvxConfigData* data, jvxBool* isit) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getNumberSubsectionsCurrentSection(jvxConfigData* data, jvxSize* num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getNumberEntriesCurrentSection(jvxConfigData* data, jvxSize* num) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceEntryCurrentSection_id(jvxConfigData* data, jvxConfigData** dataOut, jvxSize idx) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceSubsectionCurrentSection_id(jvxConfigData* data, jvxConfigData** dataOut, jvxSize idx) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceSubsectionCurrentSection_name(jvxConfigData* data, jvxConfigData** dataOut, const char* nameSection) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceEntryCurrentSection_name(jvxConfigData* dataIn, jvxConfigData** dataOut, const char* nameSection) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION getAssignmentString_allocate(jvxConfigData* data, jvxString** value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getReferencePathType(jvxConfigData* data, jvxBool* isAbsolute) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceKey_allocate(jvxConfigData* data, jvxString** key) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION getReferenceNumberPathTokens(jvxConfigData* data, jvxSize* num) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION getReferencePathToken_id_allocate(jvxConfigData* data, jvxString** token, jvxSize idx) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getAssignmentValue(jvxConfigData* data, jvxData* value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getNumberValueLists(jvxConfigData* data, jvxSize* value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getValueList_id_allocate(jvxConfigData* data, jvxDataList** lst, jvxSize idx) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getNumberStrings(jvxConfigData* data, jvxSize* value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getString_id_allocate(jvxConfigData* data, jvxString** lst, jvxSize idx) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createEmptySectionList(jvxConfigList** lst) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION addSectionToSectionList(jvxConfigList* lst, jvxConfigData* data) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION attachSectionListToSection(jvxConfigData* toAddTo, jvxConfigList* tobeAdded) = 0;

	// Build some stuff
	virtual jvxErrorType JVX_CALLINGCONVENTION createAssignmentString(jvxConfigData** data, const char* name, const char* assignmentString) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createAssignmentStringList(jvxConfigData** data, const char* name) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createAssignmentValue(jvxConfigData** data, const char* name, jvxData value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createAssignmentValueList(jvxConfigData** data, const char* name) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createComment(jvxConfigData** data, const char* comment) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION createEmptySection(jvxConfigData** data, const char* name) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION addAssignmentValueToList(jvxConfigData* data, int idxArray, jvxData value) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION addAssignmentStringToList(jvxConfigData* data, const char* entry) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION addSubsectionToSection(jvxConfigData* toAddTo, jvxConfigData* tobeAdded) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION getOriginSection(jvxConfigData* dataIn, jvxString** fName, jvxInt32* lineno) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION printConfiguration(jvxConfigData* print, jvxString** str, bool compactForm = false) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION removeHandle(jvxConfigData* toBeRemoved) = 0;

	//virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxError* err) = 0;

	//virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxString* err) = 0;
};

#endif
