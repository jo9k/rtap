#ifndef __CJVXCONDITIONS_H__
#define __CJVXCONDITIONS_H__

#include "jvx.h"

typedef jvxErrorType (*callback_trigger_command)(jvxSize uniqueId, jvxHandle* privData);
typedef jvxErrorType (*callback_trigger_relative_jump)(jvxSize uniqueId, jvxSize* pc_increment, jvxHandle* privData);

class CjvxSequencerControl
{
protected:
	typedef struct
	{
		bool* fulfilled;
		jvxSize uniqueId;
		std::string description;
		jvxUInt64 stateValid;
	} oneCondition;

	typedef struct
	{
		callback_trigger_command theStaticFunc;
		jvxHandle* priv;
		jvxSize uniqueId;
		std::string description;
		jvxUInt64 stateValid;
	} oneCommand;

	typedef struct
	{
		callback_trigger_relative_jump theStaticFunc;
		jvxHandle* priv;
		jvxSize uniqueId;
		std::string description;
		jvxUInt64 stateValid;
	} oneRelativeJump;

	struct
	{
		std::vector<oneCondition> registeredConditions;
		std::vector<oneCommand> registeredCommands;
		std::vector<oneRelativeJump> registeredJumps;
	} _common_set_sequencerControl;

public:

	CjvxSequencerControl()
	{
	};

	 ~CjvxSequencerControl()
	 {
	 };

	 inline jvxErrorType _number_conditions(jvxSize* num)
	 {
		 if(num)
		 {
			 *num = _common_set_sequencerControl.registeredConditions.size();
		 }
		 return(JVX_NO_ERROR);
	 };
	
	 inline jvxErrorType _description_condition(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid)
	 {
		 if(idx < _common_set_sequencerControl.registeredConditions.size())
		 {
			 if(uniqueId)
			 {
				 *uniqueId = _common_set_sequencerControl.registeredConditions[idx].uniqueId;
			 }

			 HjvxObject_produceString(description, _common_set_sequencerControl.registeredConditions[idx].description);
			 return(JVX_NO_ERROR);
		 }
		 return(JVX_ERROR_ID_OUT_OF_BOUNDS);
	 };

	 inline jvxErrorType _status_condition(jvxSize uniqueId, jvxBool* fulfilled, IjvxObject* theObj)
	 {
		 jvxErrorType res = JVX_NO_ERROR;
		 std::vector<oneCondition>::iterator selection; 
		 jvxState stat = JVX_STATE_NONE;
		 jvxSize maxSpace = 0;
		 jvxSize szElem = 0;

		 selection = findInConditionList(_common_set_sequencerControl.registeredConditions, uniqueId);
		 if(selection == _common_set_sequencerControl.registeredConditions.end())
		 {
			 res = JVX_ERROR_ELEMENT_NOT_FOUND;
		 }

		 if(res == JVX_NO_ERROR)
		 {
			 theObj->state(&stat);
			 if(stat & selection->stateValid)
			 {
				 if(fulfilled)
				 {
					 *fulfilled = *selection->fulfilled;
				 }
			 }
			 else
			 {
				 res = JVX_ERROR_WRONG_STATE;
			 }
		 }
		 return(res);
	 };

	
	inline jvxErrorType _register_condition(jvxSize uniqueId, const char* descr, jvxBool* fulFilled, jvxUInt64 stateValid)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneCondition>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;


		selection = findInConditionList(_common_set_sequencerControl.registeredConditions, uniqueId);
		if(selection == _common_set_sequencerControl.registeredConditions.end())
		{
			oneCondition elm;
			elm.fulfilled = fulFilled;
			elm.description = descr;
			elm.uniqueId = uniqueId;
			elm.stateValid = stateValid;

			_common_set_sequencerControl.registeredConditions.push_back(elm);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	}


	inline jvxErrorType _unregister_condition(jvxSize uniqueId)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneCondition>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInConditionList(_common_set_sequencerControl.registeredConditions, uniqueId);
		if(selection != _common_set_sequencerControl.registeredConditions.end())
		{
			_common_set_sequencerControl.registeredConditions.erase(selection);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	};

	inline std::vector<oneCondition>::iterator findInConditionList(std::vector<oneCondition>& lst, jvxSize uniqueId)
	{
		std::vector<oneCondition>::iterator elm = lst.begin();
		while(elm != lst.end())
		{
			if(elm->uniqueId == uniqueId)
			{
				break;
			}
			elm++;
		}
		return(elm);
	}


	inline jvxErrorType _number_commands(jvxSize* num)
	 {
		 if(num)
		 {
			 *num = _common_set_sequencerControl.registeredCommands.size();
		 }
		 return(JVX_NO_ERROR);
	 };
	
	 inline jvxErrorType _description_command(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid)
	 {
		 if(idx < _common_set_sequencerControl.registeredCommands.size())
		 {
			 if(uniqueId)
			 {
				 *uniqueId = _common_set_sequencerControl.registeredCommands[idx].uniqueId;
			 }

			 HjvxObject_produceString(description, _common_set_sequencerControl.registeredCommands[idx].description);
			 return(JVX_NO_ERROR);
		 }
		 return(JVX_ERROR_ID_OUT_OF_BOUNDS);
	 };

	 inline jvxErrorType _trigger_command(jvxSize uniqueId, IjvxObject* theObj)
	 {
		 jvxErrorType res = JVX_NO_ERROR;
		 std::vector<oneCommand>::iterator selection; 
		 jvxState stat = JVX_STATE_NONE;
		 jvxSize maxSpace = 0;
		 jvxSize szElem = 0;

		 selection = findInCommandList(_common_set_sequencerControl.registeredCommands, uniqueId);
		 if(selection == _common_set_sequencerControl.registeredCommands.end())
		 {
			 res = JVX_ERROR_ELEMENT_NOT_FOUND;
		 }

		 if(res == JVX_NO_ERROR)
		 {
			 theObj->state(&stat);
			 if(stat & selection->stateValid)
			 {
				 if(selection->theStaticFunc)
				 {
					 res = selection->theStaticFunc(uniqueId, selection->priv);
				 }
			 }
			 else
			 { 
				 res = JVX_ERROR_WRONG_STATE;
			 }
		 }
		 return(res);
	 };

	
	inline jvxErrorType _register_command(jvxSize uniqueId, const char* descr, callback_trigger_command fPtr, jvxHandle* priv, jvxUInt64 stateValid)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneCommand>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;


		selection = findInCommandList(_common_set_sequencerControl.registeredCommands, uniqueId);
		if(selection == _common_set_sequencerControl.registeredCommands.end())
		{
			oneCommand elm;
			elm.theStaticFunc = fPtr;
			elm.priv = priv;
			elm.description = descr;
			elm.uniqueId = uniqueId;
			elm.stateValid = stateValid;

			_common_set_sequencerControl.registeredCommands.push_back(elm);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	}


	inline jvxErrorType _unregister_command(jvxSize uniqueId)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneCommand>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInCommandList(_common_set_sequencerControl.registeredCommands, uniqueId);
		if(selection != _common_set_sequencerControl.registeredCommands.end())
		{
			_common_set_sequencerControl.registeredCommands.erase(selection);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	};

	inline std::vector<oneCommand>::iterator findInCommandList(std::vector<oneCommand>& lst, jvxSize uniqueId)
	{
		std::vector<oneCommand>::iterator elm = lst.begin();
		while(elm != lst.end())
		{
			if(elm->uniqueId == uniqueId)
			{
				break;
			}
			elm++;
		}
		return(elm);
	}





	inline jvxErrorType _number_relative_jumps(jvxSize* num)
	 {
		 if(num)
		 {
			 *num = _common_set_sequencerControl.registeredJumps.size();
		 }
		 return(JVX_NO_ERROR);
	 };
	
	 inline jvxErrorType _description_relative_jump(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid)
	 {
		 if(idx < _common_set_sequencerControl.registeredJumps.size())
		 {
			 if(uniqueId)
			 {
				 *uniqueId = _common_set_sequencerControl.registeredJumps[idx].uniqueId;
			 }

			 HjvxObject_produceString(description, _common_set_sequencerControl.registeredJumps[idx].description);
			 return(JVX_NO_ERROR);
		 }
		 return(JVX_ERROR_ID_OUT_OF_BOUNDS);
	 };

	 inline jvxErrorType _trigger_relative_jump(jvxSize uniqueId, jvxSize* pc_increment, IjvxObject* theObj)
	 {
		 jvxErrorType res = JVX_NO_ERROR;
		 std::vector<oneRelativeJump>::iterator selection; 
		 jvxState stat = JVX_STATE_NONE;
		 jvxSize maxSpace = 0;
		 jvxSize szElem = 0;

		 selection = findInJumpList(_common_set_sequencerControl.registeredJumps, uniqueId);
		 if(selection == _common_set_sequencerControl.registeredJumps.end())
		 {
			 res = JVX_ERROR_ELEMENT_NOT_FOUND;
		 }

		 if(res == JVX_NO_ERROR)
		 {
			 theObj->state(&stat);
			 if(stat & selection->stateValid)
			 {
				 if(selection->theStaticFunc)
				 {
					 res = selection->theStaticFunc(uniqueId, pc_increment, selection->priv);
				 }
			 }
			 else
			 { 
				 res = JVX_ERROR_WRONG_STATE;
			 }
		 }
		 return(res);
	 };

	
	inline jvxErrorType _register_jump(jvxSize uniqueId, const char* descr, callback_trigger_relative_jump fPtr, jvxHandle* priv, jvxUInt64 stateValid)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneRelativeJump>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;


		selection = findInJumpList(_common_set_sequencerControl.registeredJumps, uniqueId);
		if(selection == _common_set_sequencerControl.registeredJumps.end())
		{
			oneRelativeJump elm;
			elm.theStaticFunc = fPtr;
			elm.priv = priv;
			elm.description = descr;
			elm.uniqueId = uniqueId;
			elm.stateValid = stateValid;

			_common_set_sequencerControl.registeredJumps.push_back(elm);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	}


	inline jvxErrorType _unregister_jump(jvxSize uniqueId)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneRelativeJump>::iterator selection; 
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInJumpList(_common_set_sequencerControl.registeredJumps, uniqueId);
		if(selection != _common_set_sequencerControl.registeredJumps.end())
		{
			_common_set_sequencerControl.registeredJumps.erase(selection);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return(res);
	};

	inline std::vector<oneRelativeJump>::iterator findInJumpList(std::vector<oneRelativeJump>& lst, jvxSize uniqueId)
	{
		std::vector<oneRelativeJump>::iterator elm = lst.begin();
		while(elm != lst.end())
		{
			if(elm->uniqueId == uniqueId)
			{
				break;
			}
			elm++;
		}
		return(elm);
	}
};

#endif
