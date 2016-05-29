#ifndef _JVXMISCHELPERS_H__
#define _JVXMISCHELPERS_H__

#define NUM_DIGITS_RELATION 5

#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

// ============================================================================

// Macro for SIGN
#ifndef JVX_SIGN
#define JVX_SIGN(a) (a > 0 ? 1:-1)
#endif

#ifndef JVX_ABS
#define JVX_ABS(a) (a > 0 ? a:-a)
#endif

// ============================================================================

// ==================================================================
// State
// ==================================================================
#define JVXSTATE_NUM 6

static std::string jvxState_txt(jvxState stat)
{
	switch(stat )
	{
	case JVX_STATE_NONE:
		return("none");
	case JVX_STATE_INIT:
		return("init");
	case JVX_STATE_SELECTED:
		return("selected");
	case JVX_STATE_ACTIVE:
		return("active");
	case JVX_STATE_PREPARED:
		return("prepared");
	case JVX_STATE_PROCESSING:
		return("processing");
	case JVX_STATE_COMPLETE:
		return("complete");
	default:
		assert(0);
	}
	return("UNKNOWN");
} ;

inline jvxInt16 jvx_stateToIndex(jvxState theState)
{
	switch(theState)
	{
	case JVX_STATE_NONE:
		return(0);
	case JVX_STATE_INIT:
		return(1);
	case JVX_STATE_SELECTED:
		return(2);
	case JVX_STATE_ACTIVE:
		return(3);
	case JVX_STATE_PREPARED:
		return(4);
	case JVX_STATE_PROCESSING:
		return(5);
	case JVX_STATE_COMPLETE:
		return(6);
	case JVX_STATE_DONTCARE:
		return(-2);
	}
	return(-1);
}

inline jvxState jvx_idxToState(jvxSize id)
{
	switch(id)
	{
	case 0:
		return(JVX_STATE_NONE);
	case 1:
		return(JVX_STATE_INIT);
	case 2:
		return(JVX_STATE_SELECTED);
	case 3:
		return(JVX_STATE_ACTIVE);
	case 4:
		return(JVX_STATE_PREPARED);
	case 5:
		return(JVX_STATE_PROCESSING);
	case 6:
		return(JVX_STATE_COMPLETE);
	default:
		assert(0);
	}
	return(JVX_STATE_DONTCARE);
}

static std::string jvxPropertyAccessType_str[JVX_PROPERTY_ACCESS_LIMIT] =
{
	"full read + write",
	"read + write"
	"read",
	"write"
};

static const char* jvxStateSwitch_str[JVX_STATE_SWITCH_LIMIT] = 
{
	"none",
	"initialize",
	"select",
	"activate",
	"prepare",
	"start",
	"stop",
	"postprocess",
	"deactivate",
	"unselect",
	"terminate"
};

// ==================================================================
// Component Types
// ==================================================================

static std::string jvxComponentType_str[JVX_COMPONENT_LIMIT] =
{
	"unknown",
#include "helpers/HpjvxTypes_components.h"
	"audio technology",
	"audio device",
	"audio node",
	"application controller technology",
	"application controller device",
	"host",
#include "helpers/HpjvxTypes_tools.h"
	"data logger",
	"data log reader",
	"rs232",
	"config processor",
	"external call",
	"thread controller",
	"encryption/decryption",
	"web server"
};

inline std::string jvxComponentType_txt(jvxSize id)
{
	assert(id < JVX_COMPONENT_LIMIT);
	return(jvxComponentType_str[id]);
}

static std::string jvxComponentTypeClass_str[JVX_COMPONENT_TYPE_LIMIT] =
{
	"none",
	"technologies",
	"nodes"
};

inline std::string jvxComponentTypeClass_txt(jvxSize id)
{
	assert(id < JVX_COMPONENT_TYPE_LIMIT);
	return(jvxComponentTypeClass_str[id]);
}

// ==================================================================
// Component Types
// ==================================================================

static std::string jvxInterfaceType_str[JVX_INTERFACE_LIMIT] =
{
	"unknown",
	"data processor",
	"data processor selector",
#include "helpers/HpjvxTypes_interfaces.h"
	"configuration",
	"tools host",
	"report message",
	"properties",
	"sequencer control",
	"sequencer",
	"configuration line",
	"host type handler",
	"qt widget host",
	"qt widget ui",
	"binary fast data control"
};


inline std::string jvxInterfaceType_txt(jvxSize id)
{
	assert(id < JVX_INTERFACE_LIMIT);
	return(jvxComponentType_str[id]);
}

// ==================================================================
// Error types
// ==================================================================

static std::string jvxErrorType_str[JVX_ERROR_LIMIT] =
{
	"JVX_NO_ERROR",
#include "helpers/HpjvxTypes_errorcodes_txt.h"
	"JVX_ERROR_UNKNOWN",
	"JVX_ERROR_INVALID_ARGUMENT",
	"JVX_ERROR_WRONG_STATE",
	"JVX_ERROR_ID_OUT_OF_BOUNDS",
	"JVX_ERROR_UNSUPPORTED",
	"JVX_ERROR_ELEMENT_NOT_FOUND",
	"JVX_ERROR_DUPLICATE_ENTRY",
	"JVX_ERROR_OPEN_FILE_FAILED",
	"JVX_ERROR_UNEXPECTED",
	"JVX_ERROR_NOT_READY",
	"JVX_ERROR_COMPONENT_BUSY",
	"JVX_ERROR_BUFFER_OVERFLOW",
	"JVX_ERROR_BUFFER_UNDERRUN",
	"JVX_ERROR_WRONG_STATE_SUBMODULE",
	"JVX_ERROR_MESSAGE_QUEUE_FULL",
	"JVX_ERROR_PARSE_ERROR",
	"JVX_ERROR_WRONG_SECTION_TYPE",
	"JVX_ERROR_INTERNAL",
	"JVX_ERROR_EMPTY_LIST",
	"JVX_ERROR_CALL_SUB_COMPONENT_FAILED",
	"JVX_ERROR_THREADING_MISMATCH",
	"JVX_ERROR_SYSTEMCALL_FAILED",
	"JVX_ERROR_SIZE_MISMATCH",
	"JVX_ERROR_READONLY",
	"JVX_ERROR_WRITEONLY",
	"JVX_ERROR_INVALID_SETTING",
	"JVX_ERROR_TIMEOUT",
	"JVX_ERROR_ABORT",
	"JVX_ERROR_POSTPONE",
	"JVX_ERROR_END_OF_FILE",
	"JVX_ERROR_STANDARD_EXCEPTION",
	"JVX_ERROR_SEQUENCER_TOOK_CONTROL",
	"JVX_ERROR_THREAD_ONCE_MORE",
	"JVX_ERROR_PROCESS_COMPLETE",
	"JVX_ERROR_DSP",
	"JVX_ERROR_ALREADY_IN_USE"
	//"JVX_ERROR_LIMIT"
} ;

static std::string jvxErrorType_str_descr[JVX_ERROR_LIMIT] =
{
	"JVX_NO_ERROR",
#include "helpers/HpjvxTypes_errorcodes_descr.h"
	"JVX_ERROR_UNKNOWN",
	"JVX_ERROR_INVALID_ARGUMENT",
	"JVX_ERROR_WRONG_STATE",
	"JVX_ERROR_ID_OUT_OF_BOUNDS",
	"JVX_ERROR_UNSUPPORTED",
	"JVX_ERROR_ELEMENT_NOT_FOUND",
	"JVX_ERROR_DUPLICATE_ENTRY",
	"JVX_ERROR_OPEN_FILE_FAILED",
	"JVX_ERROR_UNEXPECTED",
	"JVX_ERROR_NOT_READY",
	"JVX_ERROR_COMPONENT_BUSY",
	"JVX_ERROR_BUFFER_OVERFLOW",
	"JVX_ERROR_BUFFER_UNDERRUN",
	"JVX_ERROR_WRONG_STATE_SUBMODULE",
	"JVX_ERROR_MESSAGE_QUEUE_FULL",
	"JVX_ERROR_PARSE_ERROR",
	"JVX_ERROR_WRONG_SECTION_TYPE",
	"JVX_ERROR_INTERNAL",
	"JVX_ERROR_EMPTY_LIST",
	"JVX_ERROR_CALL_SUB_COMPONENT_FAILED",
	"JVX_ERROR_THREADING_MISMATCH",
	"JVX_ERROR_SYSTEMCALL_FAILED",
	"JVX_ERROR_SIZE_MISMATCH",
	"JVX_ERROR_READONLY",
	"JVX_ERROR_INVALID_SETTING"
	"JVX_ERROR_TIMEOUT",
	"JVX_ERROR_ABORT",
	"JVX_ERROR_POSTPONE",
	"JVX_ERROR_END_OF_FILE",
	"JVX_ERROR_STANDARD_EXCEPTION",
	"JVX_ERROR_SEQUENCER_TOOK_CONTROL",
	"JVX_ERROR_THREAD_ONCE_MORE",
	"JVX_ERROR_PROCESS_COMPLETE",
	"JVX_ERROR_DSP",
	"JVX_ERROR_ALREADY_IN_USE"
	//"JVX_ERROR_LIMIT"
} ;

inline std::string jvxErrorType_txt(jvxSize id)
{
	assert(id < (int)JVX_ERROR_LIMIT);
	return(jvxErrorType_str[id]);
}

inline std::string jvxErrorType_descr(jvxSize id)
{
	assert(id < (int)JVX_ERROR_LIMIT);
	return(jvxErrorType_str_descr[id]);
}

// ==================================================================
// Category
// ==================================================================
#define JVXCATEGORYTYPE_NUM 3
static std::string jvxPropertyCategoryType_str[JVXCATEGORYTYPE_NUM] =
{
	"JVX_PROPERTY_CATEGORY_UNKNOWN",
	"JVX_PROPERTY_CATEGORY_PREDEFINED",
	"JVX_PROPERTY_CATEGORY_UNSPECIFIC"
};

inline std::string jvxPropertyCategoryType_txt(jvxSize id)
{
	assert(id < JVXCATEGORYTYPE_NUM);
	return(jvxPropertyCategoryType_str[id]);
}

static std::string jvxPropertyCategoryType_descr[JVXCATEGORYTYPE_NUM] =
{
	"unknown",
	"predefined",
	"unspecific"
};

// ==================================================================
// Property context
// ==================================================================
static std::string jvxPropertyContext_str[JVX_PROPERTY_CONTEXT_LIMIT] =
{
	"JVX_PROPERTY_CONTEXT_UNKNOWN",
	"JVX_PROPERTY_CONTEXT_PARAMETER",
	"JVX_PROPERTY_CONTEXT_INFO",
	"JVX_PROPERTY_CONTEXT_RESULT",
	"JVX_PROPERTY_CONTEXT_COMMAND",
	"JVX_PROPERTY_CONTEXT_VIEWBUFFER"
};

inline std::string jvxPropertyContext_txt(jvxSize id)
{
	assert(id < JVX_PROPERTY_CONTEXT_LIMIT);
	return(jvxPropertyContext_str[id]);
}

static std::string jvxPropertyContext_pre[JVX_PROPERTY_CONTEXT_LIMIT] =
{
	"",
	"Param: ",
	"Info: ",
	"Result: ",
	"Command: ",
	"ViewBuffer: "
};

inline std::string jvxPropertyContext_prefix(jvxSize id)
{
	assert(id < JVX_PROPERTY_CONTEXT_LIMIT);
	return(jvxPropertyContext_pre[id]);
}

// ==================================================================
// Property decoder hint type
// ==================================================================

static std::string jvxDecoderHintType_str[JVX_PROPERTY_DECODER_LIMIT] =
{
	"none",
	"file name (open)",
	"directory (select)",
	"file name (save)",
	"ip address",
	"plot array",
	"bit field",
	"single selection",
	"multi selection",
	"format (index)",
	"progress bar",
	"input file list",
	"output file list",
	"multi selection change order",
	"multi channel circ buffer",
	"command"
};

inline std::string jvxDecoderHintType_txt(jvxSize id)
{
	assert(id < JVX_PROPERTY_DECODER_LIMIT);
	return(jvxDecoderHintType_str[id]);
}

// ==================================================================
// ==================================================================
#define JVXSEQUENCEDRSTATUS_TYPE_NUM 8
static std::string  jvxSequencerStatus_str[JVXSEQUENCEDRSTATUS_TYPE_NUM] =
{
	"none",
	"startup",
	"in operation",
	"waiting",
	"wants to shutdown",
	"shutdown",
	"error",
	"shutdown complete"
};

inline std::string jvxSequencerStatus_txt(jvxSize id)
{
	assert(id < JVXSEQUENCEDRSTATUS_TYPE_NUM);
	return(jvxSequencerStatus_str[id]);
}

// ==================================================================
// ==================================================================

static std::string jvxSequencerQueueType_str[JVX_SEQUENCER_QUEUE_TYPE_LIMIT] =
{
	"queue type none",
	"queue_type_run",
	"queue_type_leave"
};


// ==================================================================
// ==================================================================

#define JVXPRIORITY_TYPE_NUM 5

static std::string jvxReportPriority_str[JVXPRIORITY_TYPE_NUM] =
{
	"",
	"<error>",
	"<warning>",
	"<success>",
	"<info>"
};

inline std::string jvxReportPriority_txt(jvxSize id)
{
	assert(id < JVXPRIORITY_TYPE_NUM);
	return(jvxReportPriority_str[id]);
}

// ==================================================================
// ==================================================================

/**
 * Function to copy a string type variable in a fixed length character buffer.
 * The field will definitely be terminated by a NULL character.
 *///=======================================================================
static bool
jvx_fillCharStr(char* ptrField, size_t lField, const std::string& str)
{
	size_t lStr = str.size();
	if(ptrField)
	{
		memset(ptrField, 0, lField);
		lStr = (lStr <= (lField-1)? lStr:(lField-1));
		memcpy(ptrField, str.c_str(), lStr);
	}
	return(true);
}

/**
 * Function to convert an integer value into a string.
 *///=======================================================================
static std::string
jvx_int2String(int val)
{
	char str[JVX_MAXSTRING];
#if _MSC_VER >= 1600
	sprintf_s(str, JVX_MAXSTRING, "%d", val);
#else
	sprintf(str, "%d", val);
#endif
	return(str);
}

/**
 * Function to convert an integer value into a string.
 *///=======================================================================
static std::string
jvx_intx2String(int val)
{
	char str[JVX_MAXSTRING];
#if _MSC_VER >= 1600
	sprintf_s(str, JVX_MAXSTRING, "%x", val);
#else
	sprintf(str, "%x", val);
#endif
	return(str);
}
/**
 * Function to convert an integer value into a string.
 *///=======================================================================
static std::string
jvx_size2String(jvxSize val)
{
	char str[JVX_MAXSTRING];
	if(sizeof(jvxSize) == 8)
	{
#if _MSC_VER >= 1600
		sprintf_s(str, JVX_MAXSTRING, JVX_PRINTF_CAST_SIZE, val);
#else
		sprintf(str, JVX_PRINTF_CAST_INT64, (jvxInt64)val);
#endif
	}
	else
	{
#if _MSC_VER >= 1600
		sprintf_s(str, JVX_MAXSTRING, "%lu", val);
#else
		sprintf(str, "%lu", val);
#endif
	}
	return(str);
}

// #ifdef RTPMODE64BIT: 64 Bit int always available!!
/**
 * Function to convert an integer value into a string.
 *///=======================================================================
static std::string
jvx_int642String(jvxInt64 val)
{
	char str[JVX_MAXSTRING];
#if _MSC_VER >= 1600
	sprintf_s(str, JVX_MAXSTRING, JVX_PRINTF_CAST_INT64, val);
#else
	sprintf(str, JVX_PRINTF_CAST_INT64, val);
#endif
	return(str);
}
//#endif


/**
 * Function to convert a jvxData value into a string.
 *///=======================================================================
static std::string
jvx_data2String(jvxData convert)
{
	char str[JVX_MAXSTRING];

	if(JVX_ISINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "inf");
	}
	else if(JVX_ISNEGINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "-inf");
	}
	else
	{
#if _MSC_VER >= 1600
		sprintf_s(str, JVX_MAXSTRING, "%f", convert);
#else
		sprintf(str, "%f", convert);
#endif
	}
	return(str);
}

/**
 * Function to convert a jvxData value into a string.
 *///=======================================================================
#define JVX_DATA_2_STRING_MAX_VALUE 1e10
#define JVX_DATA_2_STRING_MIN_VALUE -1e10

static std::string
jvx_data2String(jvxData convert, int digits)
{
	char str[JVX_MAXSTRING];

	if(JVX_ISINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "inf");
	}
	if(JVX_ISNEGINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "-inf");
	}
	else
	{
		if(convert > JVX_DATA_2_STRING_MAX_VALUE)
		{
			jvx_fillCharStr(str, JVX_MAXSTRING, "+lim");
		}
		else
		{
			if(convert < JVX_DATA_2_STRING_MIN_VALUE)
			{
				jvx_fillCharStr(str, JVX_MAXSTRING, "-lim");
			}
			else
			{
				std::string formStr;
				if(digits >= 0)
				{
					formStr = "%." + jvx_int2String(digits) + "f";
				}
				else
				{
					int numDigits = JVX_DATA2INT32(JVX_DBL_2_DATA( floor(log10(JVX_ABS(convert)+1e-10))));
					numDigits = -numDigits + NUM_DIGITS_RELATION;
					numDigits = JVX_MAX(numDigits, 0);
					numDigits = JVX_MIN(numDigits, -digits);

					formStr = "%." + jvx_int2String(numDigits) + "f";
				}

#if _MSC_VER >= 1600
				sprintf_s(str, JVX_MAXSTRING, formStr.c_str(), convert);
#else
				sprintf(str, formStr.c_str(), convert);
#endif
			}
		}
	}
	return(str);
}


/**
 * Function to convert a jvxData value into a string.
 *///=======================================================================
static std::string
jvx_data2String_highPrecision(jvxData convert, int digits)
{
	char str[JVX_MAXSTRING];
 	if(JVX_ISINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "inf");
	}
 	if(JVX_ISNEGINF(convert))
	{
		jvx_fillCharStr(str, JVX_MAXSTRING, "-inf");
	}
	else
	{
		std::string formStr = "%." + jvx_int2String(digits) + "e";
#if _MSC_VER >= 1600
		sprintf_s(str, JVX_MAXSTRING, formStr.c_str(), convert);
#else
		sprintf(str, formStr.c_str(), convert);
#endif
	}
	return(str);
}

static jvxInt32
jvx_bitfieldSelection2Id(jvxBitField sel, jvxSize numSel)
{
	jvxSize i;
	jvxInt32 retVal = -1;
	for(i = 0; i < numSel; i++)
	{
		if(sel & ((jvxBitField)1<<i))
		{
			retVal = (jvxInt32)i;
			break;
		}
	}
	return(retVal);
}

static jvxInt32
jvx_bitfieldSelectionMax(jvxBitField sel)
{
    jvxSize i;
    jvxInt32 retVal = -1;
    for(i = 0; i < sizeof(jvxBitField)*8; i++)
    {
        if(sel & ((jvxBitField)1<<i))
        {
            retVal = (jvxInt32)i;
        }
    }
    return(retVal+1); // One more than the highest position
}

/**
 *
 *///=======================================================================
static std::string
jvx_replaceDirectorySeparators_toWindows(std::string path, char replaceChar_from, char replaceChar_to)
{
	unsigned int i;
	std::string tokenReturn;
	for(i = 0; i < path.size(); i++)
	{
		char c = path[i];
		if(c == replaceChar_from)
		{
			tokenReturn += replaceChar_to;
		}
		else
		{
			tokenReturn += c;
		}
	}
	return(tokenReturn);
}

static std::string
jvx_replaceDirectorySeparators_toWindows(std::string path, char replaceChar_from, const char* replaceChar_to)
{
	unsigned int i;
	std::string tokenReturn;
	for(i = 0; i < path.size(); i++)
	{
		char c = path[i];
		if(c == replaceChar_from)
		{
			tokenReturn += replaceChar_to;
		}
		else
		{
			tokenReturn += c;
		}
	}
	return(tokenReturn);
}

static std::string
jvx_replaceSpecialCharactersBeforeWrite(std::string text, std::string characterReplace)
{
/* 	int state = 0; */
	bool cont = true;
	std::string tokenReturn;
	int posEnd = (int)text.size();
/* 	int posStart = 0; */
	size_t posQ = std::string::npos;
	while(cont)
	{
		// Find the last "
		posQ = text.rfind(characterReplace);
		if(posQ != std::string::npos)
		{
			// Take the end of the token up to last " and copy to output
			tokenReturn = text.substr(posQ, posEnd-posQ) + tokenReturn;
			tokenReturn = "\\" + tokenReturn;
			text = text.substr(0, posQ);
		}// if(posQ != std::npos)
		else
		{
			tokenReturn = text + tokenReturn;
			cont = false;
		}
	}
	return(tokenReturn);
}



/**
 * Function to convert a jvxData value into a string.
 *///=======================================================================
static std::string
jvx_absoluteToRelativePath(std::string path, bool isFile)
{
	std::list<std::string>::iterator elm;
	std::list<std::string>::iterator elmc;
	std::list<std::string>::reverse_iterator relm;

	// Scan for empty path
	if(path.size() == 0)
	{
		return(path);
	}

	// Path is already relative
	if(path.size() > 1)
	{
		if(path[0] == '.')
		{
			// path is already relativ
			return(path);
		}
	}

	char str[JVX_MAXSTRING+1];
	JVX_GETCURRENTDIRECTORY(str, JVX_MAXSTRING);

	std::string wd = str;
	std::string p = path;
	std::string tmp;
#ifdef JVX_OS_WINDOWS
	std::string driveletter;
	std::string driveletterwd;
#endif
	std::list<std::string> directories;
	std::list<std::string> directorieswd;
	std::string fileName;

	size_t pos = std::string::npos;

	if(path.size() == 0)
	{
		return("." JVX_SEPARATOR_DIR);
	}

#if defined(JVX_OS_LINUX) || defined(JVX_OS_MACOSX)

	// Check for linux absolute path (leading /)
	if(path.substr(0,1) != JVX_SEPARATOR_DIR)
	{
		// path is already a relative path..
		return(path);
	}

#endif
	while(1)
	{
		pos = p.find(JVX_SEPARATOR_DIR);
		if(pos != std::string::npos)
		{
			tmp = p.substr(0, pos);
			if(!tmp.empty())
			{
				directories.push_back(tmp);
			}
			if(pos+1 < (size_t)p.size())
			{
				p = p.substr(pos+1, p.size());
			}
			else
			{
				break;
			}
		}
		else
		{
			if(!p.empty())
			{
				directories.push_back(p);
			}
			break;
		}
	}

	pos = std::string::npos;
	while(1)
	{
		pos = wd.find(JVX_SEPARATOR_DIR);
		if(pos != std::string::npos)
		{
			tmp = wd.substr(0, pos);
			if(!tmp.empty())
			{
				directorieswd.push_back(tmp);
			}
			if(pos+1 < (size_t)wd.size())
			{
				wd = wd.substr(pos+1, wd.size());
			}
			else
			{
				break;
			}
		}
		else
		{
			if(!wd.empty())
			{
				directorieswd.push_back(wd);
			}
			break;
		}
	}


#ifdef JVX_OS_WINDOWS
	elm = directories.begin();
	if(elm != directories.end())
	{
		driveletter = *elm;

		// Small correction: There might be upper and lower case mixed somehow.
		// Solution: Convert letters to lowercase in all cases for the driveletter
		std::transform(driveletter.begin(), driveletter.end(), driveletter.begin(), ::tolower);
		directories.pop_front();
	}
	else
	{
		return(path);
	}
	elm = directorieswd.begin();
	if(elm != directorieswd.end())
	{
		driveletterwd = *elm;

		// Small correction: There might be upper and lower case mixed somehow
		// Solution: Convert letters to lowercase in all cases for the driveletter
		std::transform(driveletterwd.begin(), driveletterwd.end(), driveletterwd.begin(), ::tolower);
		directorieswd.pop_front();
	}
	else
	{
		return(path);
	}
	if(driveletter != driveletterwd)
	{
		// No relative path possible
		return(path);
	}
#endif

	if(isFile)
	{
		relm = directories.rbegin();
		if(relm != directories.rend())
		{
			fileName = *relm;
		}
		directories.pop_back();
	}

	int cnt = 0;
	elm = directories.begin();
	elmc = directorieswd.begin();
	while((elm != directories.end()) && (elmc != directorieswd.end()))
	{
		if(*elm == *elmc)
		{
			cnt++;
		}
		else
		{
			break;
		}
		elm++;
		elmc++;
	}
	std::string relativePath;
	while(elmc != directorieswd.end())
	{
		if(!relativePath.empty())
			relativePath += JVX_SEPARATOR_DIR;
		relativePath += "..";
		elmc++;
	}
	while(elm != directories.end())
	{
		if(!relativePath.empty())
			relativePath += JVX_SEPARATOR_DIR;
		relativePath += *elm;
		elm++;
	}
	if(isFile)
	{
		if(!relativePath.empty())
			relativePath += JVX_SEPARATOR_DIR;
		relativePath += fileName;
	}
	return(relativePath);
}

static std::string
jvx_replaceCharacter(std::string path, char replace, char by)
{
	int i;
	for(i = 0; i < (int)path.size(); i++)
	{
		if(path[i] == replace)
		{
			path[i] = by;
		}
	}
	return(path);
}

static std::string
jvx_replaceStrInStr(std::string strText, const std::string& strLookFor, const std::string& strReplace)
{
	for (std::string::size_type i = 0; (i = strText.find(strLookFor, i)) != std::string::npos;)
	{
		strText.replace(i, strLookFor.length(), strReplace);
		i += strReplace.length() - strLookFor.length() + 1;
	}
	return(strText);
}

/**
 * Given the buffersize, the processing format and the number of channels, this function computes the size
 * in bytes
 *///==========================================================
static size_t
jvx_numberBytesField(int buffersize, jvxDataFormat format, int chans)
{
	size_t szElm = 0;
	switch(format)
	{
	case JVX_DATAFORMAT_DATA:
		szElm = sizeof(jvxData);
		break;
	case JVX_DATAFORMAT_32BIT_LE:
	case JVX_DATAFORMAT_24BIT_LE:
		szElm = sizeof(jvxInt32);
		break;
	case JVX_DATAFORMAT_16BIT_LE:
		szElm = sizeof(jvxInt16);
		break;
	case JVX_DATAFORMAT_64BIT_LE:
		szElm = sizeof(jvxInt64);
		break;
	case JVX_DATAFORMAT_8BIT:
		szElm = sizeof(jvxInt8);
		break;
	case JVX_DATAFORMAT_NONE:
	case JVX_DATAFORMAT_STRING:
	case JVX_DATAFORMAT_VOID:
          /* BG: unhandled, is this correct? */
		  /* HK: yes, this function is not used for those datatypes */
          break;
    default:
          break;
	}
	szElm *= buffersize * chans;
	return(szElm);
}

#ifdef JVX_OS_WINDOWS

#define L_BUF_CHAR 32767

#ifdef UNICODE
#error UNICODE DEFINED
#else
static void
jvx_addPathEnvironment_onlywindows(std::vector<std::string>& additionalPaths)
{
	int i;

	// Only possible in Windows: Set the path for the location of the dynamic link libraries
	// dynamically. Add the rtpComponents subdirectory
	char* tmpC = new char[L_BUF_CHAR]; // <- given according to Win32 API
	std::string envStr;
	std::string addPath = "";

	for(i = 0; i < (int)additionalPaths.size(); i++)
	{
		if(i != 0)
			addPath += ";";
		addPath += additionalPaths[i];
	}

	if(JVX_GETENVIRONMENTVARIABLE(JVX_DYN_LIB_PATH_VARIABLE, tmpC, L_BUF_CHAR) != 0) //GetEnvironmentVariable("path", tmpC, L_BUF_CHAR)
	{
		envStr = tmpC;
		if(JVX_GETCURRENTDIRECTORY(tmpC, L_BUF_CHAR) != 0)
		{
			envStr = JVX_PASTE_ENV_VARIABLE_ATT(tmpC, envStr, JVX_COMPONENT_DIR);//(std::string)tmpC + "\\" + COMPONENT_DIR + ";" + envStr;
			if(!addPath.empty())
			{
				envStr = JVX_PASTE_ENV_VARIABLE_ADD_PATH_FRONT(envStr, addPath);//(std::string)tmpC + "\\" + COMPONENT_DIR + ";" + envStr;
			}
			JVX_SETENVIRONMENTVARIABLE(JVX_DYN_LIB_PATH_VARIABLE, envStr.c_str());//SetEnvironmentVariable("path", envStr.c_str());
		}
	}
	else
	{
		if(JVX_GETCURRENTDIRECTORY(tmpC, L_BUF_CHAR) != 0)
		{
			envStr = JVX_PASTE_ENV_VARIABLE_NEW(tmpC, JVX_COMPONENT_DIR);//(std::string)tmpC + "\\" + COMPONENT_DIR + ";" + envStr;
			if(!addPath.empty())
			{
				envStr = JVX_PASTE_ENV_VARIABLE_ADD_PATH_FRONT(envStr, addPath);//(std::string)tmpC + "\\" + COMPONENT_DIR + ";" + envStr;
			}
			JVX_SETENVIRONMENTVARIABLE(JVX_DYN_LIB_PATH_VARIABLE, envStr.c_str());//SetEnvironmentVariable("path", envStr.c_str());
		}
	}
	if(JVX_GETENVIRONMENTVARIABLE(JVX_DYN_LIB_PATH_VARIABLE, tmpC, L_BUF_CHAR) != 0) //GetEnvironmentVariable("path", tmpC, L_BUF_CHAR)
	{
		envStr = tmpC;
		//std::cout << "ENV: " << JVX_DYN_LIB_PATH_VARIABLE << "=" << envStr << std::endl;
	}
	delete[](tmpC);
}
#endif
#endif

// ============================================================================

#ifndef JVX_SAFE_NEW_OBJ
#define JVX_SAFE_NEW_OBJ(a, b) a = new b;
#endif

#ifndef JVX_SAFE_NEW_FLD
#define JVX_SAFE_NEW_FLD(a,b,c) a = new b[c]
#endif

#ifndef JVX_SAFE_NEW_FLD_CVRT
#define JVX_SAFE_NEW_FLD_CVRT(a,b,c, d) a = (d)new b[c]
#endif

#ifndef JVX_SAFE_MALLOC_OBJ
#define JVX_SAFE_MALLOC_OBJ(a, b) a = (b*)malloc(sizeof(b));
#endif

#ifndef JVX_SAFE_MALLOC_FLD
#define JVX_SAFE_MALLOC_FLD(a,b,c) a = (b*) malloc(sizeof(b) * c);
#endif

#ifndef JVX_SAFE_DELETE_OBJ
#define JVX_SAFE_DELETE_OBJ(a) if(a) { delete(a); a= NULL; }
#endif

#ifndef JVX_SAFE_DELETE_FLD
#define JVX_SAFE_DELETE_FLD(a, b) if(a) { delete[]((b*)a); a= NULL; }
#endif

#ifndef JVX_SAFE_FREE_OBJ
#define JVX_SAFE_FREE_OBJ(a) if(a) { free(a); a= NULL; }
#endif

#ifndef JVX_SAFE_FREE_FLD
#define JVX_SAFE_FREE_FLD(a) if(a) { free(a); a= NULL; }
#endif

#ifndef JVX_SAFE_DELETE_FLD_CVRT
#define JVX_SAFE_DELETE_FLD_CVRT(a,b) if(a) { delete[]((b)a); a = NULL; }
#endif

#ifndef JVX_SAFE_FREE_FLD_PRECONVERT
#define JVX_SAFE_FREE_FLD_CVRT(a,b) if(a) { free((b)a); a = NULL; }
#endif

// ============================================================================

#ifdef __cplusplus
template<typename T> void jvx_allocateField_cpp(jvxHandle*& ptr, jvxSize buffersize)
{
	JVX_SAFE_NEW_FLD(ptr, T, buffersize);
	memset(ptr, 0, sizeof(T) *buffersize);
}

template<typename T> void jvx_deallocateField_cpp(jvxHandle*& ptr)
{
	JVX_SAFE_DELETE_FLD(ptr, T);
}
#endif


#ifndef JVX_DATA_2_DATA
#define JVX_DATA_2_DATA(in) in
#endif
#ifndef JVX_DATA_2_FLOAT
#define JVX_DATA_2_FLOAT(in) ((jvxFloat)in)
#endif
#ifndef JVX_DATA_2_INT32
#define JVX_DATA_2_INT32(in) JVX_DATA2INT32(in * JVX_MAX_INT_32)
#endif
#ifndef JVX_DATA_2_INT24
#define JVX_DATA_2_INT24(in) JVX_DATA2INT32(in * JVX_MAX_INT_24)
#endif
#ifndef JVX_DATA_2_INT16
#define JVX_DATA_2_INT16(in) JVX_DATA2INT16(in * JVX_MAX_INT_16)
#endif
#ifndef JVX_DATA_2_INT8
#define JVX_DATA_2_INT8(in) JVX_DATA2INT8(in * JVX_MAX_INT_8)
#endif
#ifndef JVX_DATA_2_INT64
#define JVX_DATA_2_INT64(in) JVX_DATA2INT64(in * JVX_MAX_INT_64)
#endif

#ifndef JVX_FLOAT_2_DATA
#define JVX_FLOAT_2_DATA(in) ((jvxData)in)
#endif
#ifndef JVX_INT32_2_DATA
#define JVX_INT32_2_DATA(in) ((jvxData)in * JVX_MAX_INT_32_DIV)
#endif
#ifndef JVX_INT24_2_DATA
#define JVX_INT24_2_DATA(in) ((jvxData)in * JVX_MAX_INT_24_DIV)
#endif
#ifndef JVX_INT16_2_DATA
#define JVX_INT16_2_DATA(in) ((jvxData)in * JVX_MAX_INT_16_DIV)
#endif
#ifndef JVX_INT64_2_DATA
#define JVX_INT64_2_DATA(in) ((jvxData)in * JVX_MAX_INT_64_DIV)
#endif
#ifndef JVX_INT8_2_DATA
#define JVX_INT8_2_DATA(in) ((jvxData)in * JVX_MAX_INT_8_DIV)
#endif

/*
#ifndef JVX_FLOAT_2_FLOAT
#define JVX_FLOAT_2_FLOAT(in) in
#endif
#ifndef JVX_FLOAT_2_INT32
#define JVX_FLOAT_2_INT32(in) JVX_FLOAT2INT32(in * JVX_MAX_INT_32)
#endif
#ifndef JVX_FLOAT_2_INT24
#define JVX_FLOAT_2_INT24(in) JVX_FLOAT2INT32(in * JVX_MAX_INT_24)
#endif
#ifndef JVX_FLOAT_2_INT16
#define JVX_FLOAT_2_INT16(in) JVX_FLOAT2INT16(in * JVX_MAX_INT_16)
#endif
#ifndef JVX_FLOAT_2_INT8
#define JVX_FLOAT_2_INT8(in) JVX_FLOAT2INT8(in * JVX_MAX_INT_8)
#endif
#ifndef JVX_FLOAT_2_INT64
#define JVX_FLOAT_2_INT64(in) JVX_FLOAT2INT64(in * JVX_MAX_INT_64)
#endif

#ifndef JVX_INT32_2_FLOAT
#define JVX_INT32_2_FLOAT(in) ((jvxData)in * JVX_MAX_INT_32_DIV)
#endif
#ifndef JVX_INT24_2_FLOAT
#define JVX_INT24_2_FLOAT(in) ((jvxData)in * JVX_MAX_INT_24_DIV)
#endif
#ifndef JVX_INT16_2_FLOAT
#define JVX_INT16_2_FLOAT(in) ((jvxData)in * JVX_MAX_INT_16_DIV)
#endif
#ifndef JVX_INT64_2_FLOAT
#define JVX_INT64_2_FLOAT(in) ((jvxData)in * JVX_MAX_INT_64_DIV)
#endif
#ifndef JVX_INT8_2_FLOAT
#define JVX_INT8_2_FLOAT(in) ((jvxData)in * JVX_MAX_INT_8_DIV)
#endif
*/

static jvxErrorType
jvx_writeContentToFile(std::string fName, std::string& writeContentStr, IjvxCrypt* theCryptor = NULL)
{
	FILE* fPtr = NULL;
	size_t val = 0;
	jvxByte* ptrContent = (jvxByte*)writeContentStr.c_str();
	jvxSize numWrite = writeContentStr.size();
	jvxErrorType res = JVX_NO_ERROR;

	fopen_s(&fPtr, fName.c_str(),"wb");
	if(!fPtr)
	{
		return(JVX_ERROR_OPEN_FILE_FAILED);
	}

	if(theCryptor)
	{
		// Replace the byte field involvng encryption
		res = theCryptor->encrypt(ptrContent, numWrite, &ptrContent, &numWrite);
		if(res != JVX_NO_ERROR)
		{
			return(JVX_ERROR_CALL_SUB_COMPONENT_FAILED);
		}
		assert(ptrContent);
	}

	//writeContent = writeContent + "\n";
	val =	fwrite(ptrContent, sizeof(char),numWrite , fPtr);

	if(theCryptor)
	{
		theCryptor->release(ptrContent);
		ptrContent = NULL;
	}

	fclose(fPtr);

	if(val != numWrite)
	{
		return(JVX_ERROR_UNEXPECTED);
	}
	return(JVX_NO_ERROR);
}

static jvxErrorType
jvx_readContentFromFile(std::string fName, std::string& storeContent, IjvxCrypt* theCryptor = NULL)
{
	char c;
	FILE* fPtr = NULL;
	jvxByte* fldOnReturn = NULL;
	jvxSize numElms = 0;
	jvxErrorType res = JVX_NO_ERROR;
	int cnt = 0;

#if _MSC_VER >= 1600
	fopen_s(&fPtr, fName.c_str(),"rb");
#else
	fPtr = fopen(fName.c_str(),"rb");
#endif

	if(!fPtr)
	{
		return(JVX_ERROR_OPEN_FILE_FAILED);
	}
	while(1)
	{
		size_t val = 0;
		val =	fread(&c, sizeof(char), 1, fPtr);
		if(val == 1)
		{
			storeContent += c;
		}
		else
		{
			break;
		}
		cnt ++;
	}
	fclose(fPtr);

	if(theCryptor)
	{
		res = theCryptor->decrypt((jvxByte*)storeContent.c_str(), storeContent.size(), &fldOnReturn, &numElms);
		if(res != JVX_NO_ERROR)
		{
			return(JVX_ERROR_CALL_SUB_COMPONENT_FAILED);
		}
		storeContent = std::string(fldOnReturn, numElms);
		theCryptor->release(fldOnReturn);
	}

	return(JVX_NO_ERROR);
}

#ifdef JVX_OS_WINDOWS

/**
 * Function to convert a standard string into a LPWSTR. This is required for the IPC since MS
 * tends to use their own specific non-standard conform strings.
 *///==========================================================================
static wchar_t*
jvx_stdString2LPWSTR_alloc(std::string txt)
{
	int i;
	wchar_t* newField = new wchar_t[txt.size()+1];
	memset(newField, 0, sizeof(wchar_t) * (txt.size()+1));
	for(i = 0; i < (int)txt.size(); i++)
	{
		newField[i] = (wchar_t)txt[i];
	}
	return(newField);
}

/**
 * Function to convert a standard string into a LPWSTR. This is required for the IPC since MS
 * tends to use their own specific non-standard conform strings.
 *///==========================================================================
static void
jvx_stdString2LPWSTR_delete(wchar_t* ptr)
{
	delete[](ptr);
}
#endif


/**
* Convert all letter to uppercases letters
*///=======================================================================
static std::string jvx_toUpper(const std::string& txt)
{
	char buf[2];
	std::string txtOut;
	memset(buf, 0, 2);
	for(int i = 0; i < (int)txt.size(); i++)
	{
		buf[0] = (char)toupper(txt.c_str()[i]);
		txtOut += buf;
	}
	return(txtOut);
}

/**
 * Check a 64 bit bitfield for allowed states
 *///=====================================================================
static std::string jvx_validStates2String(jvxUInt64 states)
{
	int i;
	std::string out;
	for(i = 1; i < JVXSTATE_NUM; i++)
	{
		jvxState stat = jvx_idxToState(i);
		if(states & stat)
		{
			if(out.empty())
			{
				out += jvxState_txt(stat);
			}
			else
			{
				out += ", " + jvxState_txt(stat);
			}
		}
	}
	return(out);
}

template<typename T> static bool jvx_insertListBeforePosition(std::vector<T>& lst, T& newItem, int position)
{
	int i;
	if((position >= 0) && (position < (int)lst.size()))
	{
		std::vector<T> newList;
		for(i = 0; i < position; i++)
		{
			newList.push_back(lst[i]);
		}
		newList.push_back(newItem);
		for(i = position; i < (int)lst.size(); i++)
		{
			newList.push_back(lst[i]);
		}
		lst = newList;
		return(true);
	}
	return(false);
}

template<typename T> static bool jvx_insertListAfterPosition(std::vector<T>& lst, T& newItem, int position)
{
	int i;
	if((position >= 0) && (position < (int)lst.size()))
	{
		std::vector<T> newList;
		for(i = 0; i <= position; i++)
		{
			newList.push_back(lst[i]);
		}
		newList.push_back(newItem);
		for(i = position+1; i < (int)lst.size(); i++)
		{
			newList.push_back(lst[i]);
		}
		lst = newList;
		return(true);
	}
	return(false);
}

template<typename T> static jvxErrorType jvx_removeItemFromPosition(std::vector<T>& lst, int position)
{
	int i;
	if((position >= 0) && (position < (int)lst.size()))
	{
		std::vector<T> newList;
		for(i = 0; i < position; i++)
		{
			newList.push_back(lst[i]);
		}
		for(i = position+1; i < (int)lst.size(); i++)
		{
			newList.push_back(lst[i]);
		}
		lst = newList;
		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_ID_OUT_OF_BOUNDS);
}

template<typename T, typename T_sel>
static typename std::vector<T>::iterator jvx_findItemSelectorInList(std::vector<T>& lst, T_sel& selection)
{
	typename std::vector<T>::iterator elm = lst.begin();
	for(; elm != lst.end(); elm++)
	{
		if(elm->selector == selection)
		{
			break;
		}
	}
	return(elm);
}

template<typename T, typename T_sel>
static typename std::vector<T>::iterator jvx_findItemSelectorInList(std::vector<T>& lst, T_sel& selection, jvxSize idxSelector)
{
    typename std::vector<T>::iterator elm = lst.begin();
	for(; elm != lst.end(); elm++)
	{
		if(elm->selector[idxSelector] == selection)
		{
			break;
		}
	}
	return(elm);
}

template<typename T> static typename std::vector<T>::iterator jvx_findItemIdInList(std::vector<T>& lst, jvxSize& idx)
{
	typename std::vector<T>::iterator elm = lst.begin();
	jvxSize cnt = 0;
	for(; elm != lst.end(); elm++, cnt++)
	{
		if(cnt == idx)
		{
			break;
		}
	}
	return(elm);
}

static bool jvxFileExists(const char *filePath)
{
	JVX_DIR_FIND_DATA findFileData;
	JVX_HANDLE_DIR hFind;

	//Check if file can be found.
	hFind = JVX_FINDFIRSTFILE_S(filePath, findFileData);
	if(hFind == JVX_INVALID_HANDLE_VALUE)
	{
		return false;
	}
	JVX_FINDCLOSE(hFind);
	return true;
}

inline static void jvx_switchEndianess_int32(jvxInt32* value)
{
	jvxSize i;
	jvxInt32 retVal = 0;
	jvxInt32 tmp;
	for(i = 0; i < 4; i++)
	{
		retVal = retVal << 8;
		tmp = *value >> i*8;
		retVal |= (tmp & 0xFF);
	}
	*value = retVal;
}

inline static void jvx_switchEndianess_int16(jvxInt16* value)
{
	jvxSize i;
	jvxInt16 retVal = 0;
	for(i = 0; i < 2; i++)
	{
		retVal = retVal << 8;
		retVal |= (*value >> i*8) & 0xFF;
	}
	*value = retVal;
}

static jvxErrorType
jvx_parseCommandIntoToken(std::string command, std::string& f_expr, std::vector<std::string>& args)
{
	size_t i;
	jvxErrorType res = JVX_ERROR_PARSE_ERROR;
	std::string remainItems;
	std::string tok_taken;

	args.clear();
	f_expr.clear();
	size_t pos_openBraces = command.find("(");
	size_t pos_closeBraces = command.rfind(")");
	if(pos_openBraces == std::string::npos)
	{
		return(res);
	}
	if(pos_closeBraces == std::string::npos)
	{
		return(res);
	}
	f_expr = command.substr(0,pos_openBraces);
	remainItems = command.substr(pos_openBraces+1, pos_closeBraces - pos_openBraces - 1);
	res = JVX_NO_ERROR;

	tok_taken.clear();
	int state = 0;
	char oneChar;
	for(i = 0; i < remainItems.size(); i++)
	{
		oneChar = remainItems[i];
		switch(state)
		{
		case 0:
			switch(oneChar)
			{
			case ',':
				//tok_taken += oneChar;
				args.push_back(tok_taken);
				tok_taken.clear();
				break;
			case '"':
				state = 1;
				break;
			default:
				tok_taken += oneChar;
				break;
			}
			break;
		case 1:
			switch(oneChar)
			{
			case '"':
				state = 3;
				break;
			/*case '\\':
				state = 2;
				break;*/
			default:
				tok_taken += oneChar;
				break;
			}
			break;
		case 2:
			state = 1;
			switch(oneChar)
			{
			case 't':
				tok_taken += '\t';
				break;
			case 'r':
				tok_taken += '\r';
				break;
			case 'n':
				tok_taken += '\n';
				break;
			default:
				tok_taken += '\\';
				tok_taken += oneChar;
				break;
			}
			break;
		case 3:
			switch(oneChar)
			{
			case ' ':
			case '\t':
				break;
			case ',':
				args.push_back(tok_taken);
				tok_taken.clear();
				state = 0;
				break;
			default:
				res = JVX_ERROR_PARSE_ERROR;
			}
			break;
		}
		if(res != JVX_NO_ERROR)
		{
			break;
		}
	}
	//if(!tok_taken.empty())
	//{
		args.push_back(tok_taken);
		tok_taken.clear();
	//}
	return(res);
}

static jvxErrorType 
jvx_stringToMacAddress(std::string str, jvxUInt8* mac)
{
	int i;
	std::string token;
	std::string remain = str;
	jvxErrorType res = JVX_NO_ERROR;
	size_t pos;
	for(i = 0; i < 5; i++)
	{
		pos = remain.find(':');
		if(pos == 2)
		{
			token = remain.substr(0,2);
			remain = remain.substr(3, std::string::npos); // get rid of ":"
			int number = strtol(token.c_str(), NULL, 16);
			assert(number <= 255);
			mac[i] = (jvxUInt8) number;
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
			break;
		}
	}
	if(res == JVX_NO_ERROR)
	{
		int number = strtol(remain.c_str(), NULL, 16);
		assert(number <= 255);
		mac[i] = (jvxUInt8) number;
	}
	return(res);
}

static std::string 
jvx_macAddressToString(jvxUInt8* mac)
{
	int i;

	std::string out;
	for(i = 0; i < 5; i++)
	{
		out += jvx_intx2String(mac[i]);
		out += ":";
	}
	out += jvx_intx2String(mac[i]);
	return(out);
}

static std::string jvx_shortenStringName(jvxSize numLetters, std::string input)
{
	std::string ret = input;
	if(input.size() > numLetters)
	{
		ret = input.substr(0, numLetters/4) + "..." + input.substr(input.size()-3*numLetters/4, 3*numLetters/4);
	}
	return ret;
}

static jvxErrorType jvx_parseCommandLineOneToken(std::string in, std::vector<std::string>& out, char sep = ' ')
{
	std::string oneToken = "";
	jvxSize pos = std::string::npos;
	do
	{
		pos = in.find(sep);
		if(pos != std::string::npos)
		{
			oneToken = in.substr(0, pos);
			in = in.substr(pos+1, std::string::npos);
			out.push_back(oneToken);
		}
	}
	while(pos != std::string::npos);
	if(!in.empty())
	{
		out.push_back(in);
	}
	return(JVX_NO_ERROR);
}

static std::string
jvx_charReplace(std::string input, char* replaceChar_from,  const char** replaceChar_to, jvxSize numReplace)
{
	unsigned int i,j;
	std::string tokenReturn;
	jvxBool found = false;
	for(i = 0; i < input.size(); i++)
	{
		char c = input[i];
		found = false;
		for(j = 0; j < numReplace; j++)
		{
			if(c == replaceChar_from[j])
			{
				found = true;
				break;
			}
		}

		if(found)
		{
			tokenReturn += replaceChar_to[j];
		}
		else
		{
			tokenReturn += c;
		}
	}
	return(tokenReturn);
}

static 
jvxErrorType jvx_parseValueString_size(std::string input, jvxSize* numberValues)
{
	std::string token;
	if(numberValues)
	{
		*numberValues = 0;
		if(input.size() > 0)
		{
			// Remove leading and ending []
			if((input[0] == '[') && (input[input.size()-1] == ']'))
			{
				input = input.substr(1,input.size()-2);
			}

			while(1)
			{
				size_t pp = std::string::npos;
				pp = input.find(input, ',');
				if(pp == std::string::npos)
				{
					break;
				}
				else
				{
					token = input.substr(0, pp-1);
					input = input.substr(pp, std::string::npos);
					(*numberValues)++;
				}
			}
			if(input.size())
			{
				(*numberValues)++;
			}
		}
		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_INVALID_ARGUMENT);
}

static 
jvxErrorType jvx_parseValueString_content(std::string input, jvxHandle* fld, jvxSize* numberValues, jvxDataFormat form)
{
	std::string token;
	jvxSize cnt = 0;
	jvxData* fldData = (jvxData*)fld;
	jvxInt8* fldInt8 = (jvxInt8*)fld;
	jvxInt16* fldInt16 = (jvxInt16* )fld;
	jvxInt32* fldInt32 = (jvxInt32* )fld;
	jvxInt64* fldInt64 = (jvxInt64* )fld;

	jvxData valD; 
	if(numberValues)
	{
		*numberValues = 0;
		if(input.size() > 0)
		{
			// Remove leading and ending []
			if((input[0] == '[') && (input[input.size()-1] == ']'))
			{
				input = input.substr(1,input.size()-2);
			}

			while(1)
			{
				size_t pp = std::string::npos;
				pp = input.find(input, ',');
				if(pp == std::string::npos)
				{
					break;
				}
				else
				{
					token = input.substr(0, pp-1);
					input = input.substr(pp, std::string::npos);
					if(cnt < *numberValues)
					{
						valD = atof(token.c_str());
						switch(form)
						{
						case JVX_DATAFORMAT_DATA:
							fldData[cnt] = (jvxData)valD;
							break;
						case JVX_DATAFORMAT_8BIT:
							fldInt8[cnt] = (jvxInt8)valD;
							break;
						case JVX_DATAFORMAT_16BIT_LE:
							fldInt16[cnt] = (jvxInt16)valD;
							break;
						case JVX_DATAFORMAT_24BIT_LE:
						case JVX_DATAFORMAT_32BIT_LE:
							fldInt32[cnt] = (jvxInt32)valD;
							break;
						case JVX_DATAFORMAT_64BIT_LE:
							fldInt16[cnt] = (jvxInt16)valD;
							break;
						default:
							assert(0);
						}
						cnt++;
					}
					else
					{
						break;
					}
				}
			}
			if(input.size())
			{
				if(cnt < *numberValues)
				{
					valD = atof(input.c_str());
					switch(form)
					{
					case JVX_DATAFORMAT_DATA:
						fldData[cnt] = (jvxData)valD;
						break;
					case JVX_DATAFORMAT_8BIT:
						fldInt8[cnt] = (jvxInt8)valD;
						break;
					case JVX_DATAFORMAT_16BIT_LE:
						fldInt16[cnt] = (jvxInt16)valD;
						break;
					case JVX_DATAFORMAT_24BIT_LE:
					case JVX_DATAFORMAT_32BIT_LE:
						fldInt32[cnt] = (jvxInt32)valD;
						break;
					case JVX_DATAFORMAT_64BIT_LE:
						fldInt16[cnt] = (jvxInt16)valD;
						break;
					default:
						assert(0);
					}
					cnt++;
				}
			}
		}
		*numberValues = cnt;
		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_INVALID_ARGUMENT);
}

#endif
