#ifndef __HJVXOBJECTS_H__
#define __HJVXOBJECTS_H__

#include <string>
#include <vector>

#define HJVXOBJECTS_GETNAME(a,b,c) {jvxString* a; b->getName(&a); c = a->bString; b->deallocate(a);}
#define HJVXOBJECTS_GETDESCRIPTION(a,b,c) {jvxString* a; b->getDescription(&a); c = a->bString; b->deallocate(a);}
#define HJVXOBJECTS_GETERROR(a,b,c,d,e,f) {jvxError* a; b->getLastError(&a); c = a->errorType; d = a->errorCode; e = a->errorLevel; f = a->errorDescription; b->deallocate(a);}
#define HJVXOBJECTS_GETNAMECHANNELIN(a,b,c,d) {jvxString* a; b->getNameChannelInputDevice(c,&a); d = a->bString; b->deallocate(a);}
#define HJVXOBJECTS_GETNAMECHANNELOUT(a,b,c,d) {jvxString* a; b->getNameChannelOutputDevice(c,&a); d = a->bString; b->deallocate(a);}

static void
HjvxObject_deallocate(jvxString* tobeRemoved)
{
	if(tobeRemoved)
	{
		delete[](tobeRemoved->bString);
		delete(tobeRemoved);
	}
}

static void
HjvxObject_deallocate(jvxStringList* tobeRemoved)
{
	int i;
	if(tobeRemoved)
	{
		// remove each single string
		for(i = 0; i < (int)tobeRemoved->lStrings; i++)
		{
			delete[](tobeRemoved->bStrings[i].bString);
		}

		// Delete field for strings
		delete[](tobeRemoved->bStrings);

		// Delete container
		delete(tobeRemoved);
	}
}

static void
HjvxObject_deallocate(jvxInt32List * tobeRemoved)
{
	if(tobeRemoved)
	{
		delete[](tobeRemoved->bIntList);
		delete(tobeRemoved);
	}
}

static void
HjvxObject_deallocate(jvxDataList * tobeRemoved)
{
	if(tobeRemoved)
	{
		delete[](tobeRemoved->bDblList);
		delete(tobeRemoved);
	}
}

static void
HjvxObject_deallocate(jvxError* tobeRemoved)
{
	if(tobeRemoved)
	{
		HjvxObject_deallocate(tobeRemoved->errorDescription);
		delete(tobeRemoved);
	}
}

static void
HjvxObject_produceString(jvxString** str_argout, const std::string str_conv)
{
	if(str_argout)
	{
		*str_argout = new jvxString;
		(*str_argout)->lString = str_conv.length() + 1;
		(*str_argout)->bString = new char[(*str_argout)->lString];
		memset((*str_argout)->bString, 0, sizeof(char) * (*str_argout)->lString);
		memcpy((*str_argout)->bString, str_conv.c_str(), sizeof(char) * (*str_argout)->lString-1);
	}
}

static void
HjvxObject_produceStringList(jvxStringList** strl_argout, const std::vector<std::string> str_list)
{
	int i;
	if(strl_argout)
	{
		*strl_argout = new jvxStringList;
		(*strl_argout)->bStrings = NULL;
		(*strl_argout)->lStrings = str_list.size();
		if((*strl_argout)->lStrings)
		{
			(*strl_argout)->bStrings = new jvxString[(*strl_argout)->lStrings];
		}
		for(i = 0; i < (int)(*strl_argout)->lStrings; i++)
		{
			(*strl_argout)->bStrings[i].lString = str_list[i].length() + 1;
			(*strl_argout)->bStrings[i].bString = new char[(*strl_argout)->bStrings[i].lString];
			memset((*strl_argout)->bStrings[i].bString, 0, sizeof(char) * (*strl_argout)->bStrings[i].lString);
			memcpy((*strl_argout)->bStrings[i].bString, str_list[i].c_str(), sizeof(char) * (*strl_argout)->bStrings[i].lString-1);
		}
	}
}

static void
HjvxObject_produceDataList(jvxDataList** strl_argout, const std::vector<jvxData> dat_list)
{
	int i;
	if(strl_argout)
	{
		*strl_argout = new jvxDataList;
		(*strl_argout)->bDblList = NULL;
		(*strl_argout)->lDblList = dat_list.size();
		if((*strl_argout)->lDblList)
		{
			(*strl_argout)->bDblList = new jvxData[(*strl_argout)->lDblList];
		}
		for(i = 0; i < (int)(*strl_argout)->lDblList; i++)
		{
			(*strl_argout)->bDblList[i] = dat_list[i];
		}
	}
}

/*
static void
HjvxObject_produceSelectionList(jvxSelectionList** strl_argout, jvx_oneSelectionOption_cpp& opt_list)
{
	int i;
	if(strl_argout)
	{
		*strl_argout = new jvxSelectionList;
		(*strl_argout)->bStrings = NULL;
		(*strl_argout)->bitFieldSelected = 0;
		(*strl_argout)->bitFieldExclusive = 0;
		(*strl_argout)->lSelections = opt_list.entries.size();

		if((*strl_argout)->lSelections)
		{
			(*strl_argout)->bStrings = new jvxString[(*strl_argout)->lSelections];
		}
		for(i = 0; i < (int)(*strl_argout)->lSelections; i++)
		{
			(*strl_argout)->bStrings[i].lString = opt_list.entries[i].size() + 1;
			(*strl_argout)->bStrings[i].bString = new char[(*strl_argout)->bStrings[i].lString];
			memset((*strl_argout)->bStrings[i].bString, 0, sizeof(char) * (*strl_argout)->bStrings[i].lString);
			memcpy((*strl_argout)->bStrings[i].bString, opt_list.entries[i].c_str(), sizeof(char) * (*strl_argout)->bStrings[i].lString-1);
		}
		(*strl_argout)->bitFieldSelected = opt_list.selection;
		(*strl_argout)->bitFieldExclusive = opt_list.exclusive;
	}
}

static void
HjvxObject_deallocate(jvxSelectionList* strl_arg)
{
	jvxSize i;
	if(strl_arg)
	{
		for(i = 0; i < strl_arg->lSelections; i++)
		{
			delete[](strl_arg->bStrings[i].bString);
		}
		delete[](strl_arg->bStrings);
		delete(strl_arg);
	}
}
*/

static void
HjvxObject_produceError(jvxError** err_argout, jvxInt32 errorID,
							jvxInt32 errorLevel, const std::string& errorDescription)
{
	if(err_argout)
	{
		*err_argout = new jvxError;
		(*err_argout)->errorCode = errorID;
		(*err_argout)->errorLevel = errorLevel;

		(*err_argout)->errorDescription = new jvxString;
		HjvxObject_produceString(&((*err_argout)->errorDescription), errorDescription);
	}
}


#endif
