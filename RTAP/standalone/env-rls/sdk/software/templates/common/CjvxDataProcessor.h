#ifndef __CJVXDATAPROCESSOR_H__
#define __CJVXDATAPROCESSOR_H__

#include "jvx.h"
#include "helpers/HjvxDataProcessor.h"

class CjvxDataProcessor
{
protected:

	typedef struct
	{
		bool allocated_sender_to_receiver;
		bool allocated_receiver_to_sender;
		std::string rt_error_descr;
		jvxErrorType rt_error_type;
		jvxInt32 rt_error_id;
	} oneSlot;

	struct
	{
		jvxSize slotCnt;
		jvxState inDataLinkMode;

		std::vector<oneSlot> active_slots;

		IjvxObject* thisisme;

		jvxComponentType iActAsComponent;

		jvxBool supportsMultiThreading;
	} _common_spec_proc_set;

public:

	CjvxDataProcessor()
	{
		_common_spec_proc_set.slotCnt = 0;
		_common_spec_proc_set.inDataLinkMode = JVX_STATE_INIT;
		_common_spec_proc_set.thisisme = NULL;
		_common_spec_proc_set.iActAsComponent = JVX_COMPONENT_UNKNOWN;
		_common_spec_proc_set.supportsMultiThreading = false;
	};

	jvxErrorType _data_link_init()
	{
		if(_common_spec_proc_set.inDataLinkMode  == JVX_STATE_INIT)
		{
			_common_spec_proc_set.slotCnt = 0;
			_common_spec_proc_set.inDataLinkMode = JVX_STATE_PREPARED;
			_common_spec_proc_set.active_slots.clear();

		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _data_link_start()
	{
		if(_common_spec_proc_set.inDataLinkMode  == JVX_STATE_PREPARED)
		{
			_common_spec_proc_set.inDataLinkMode = JVX_STATE_PROCESSING;
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _data_link_stop()
	{
		if(_common_spec_proc_set.inDataLinkMode  == JVX_STATE_PROCESSING)
		{
			_common_spec_proc_set.inDataLinkMode = JVX_STATE_PREPARED;
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _data_link_terminate()
	{
		if(_common_spec_proc_set.inDataLinkMode  == JVX_STATE_PREPARED)
		{
			_common_spec_proc_set.inDataLinkMode = JVX_STATE_INIT;
			_common_spec_proc_set.active_slots.clear();
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _reference_object(IjvxObject** theObj)
	{
		if(theObj)
		{
			*theObj = _common_spec_proc_set.thisisme;
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _act_as_component(jvxComponentType* iActAsComponent)
	{
		if(iActAsComponent)
		{
			*iActAsComponent = _common_spec_proc_set.iActAsComponent;
			if(*iActAsComponent == JVX_COMPONENT_UNKNOWN)
			{
				if(_common_spec_proc_set.thisisme)
				{
					_common_spec_proc_set.thisisme->request_specialization(NULL, iActAsComponent, NULL);
				}
			}
		}
		return(JVX_NO_ERROR);
	};


	jvxErrorType _prepare_data_link_slot(jvxLinkDataDescriptor* theData)
	{
		if(_common_spec_proc_set.inDataLinkMode == JVX_STATE_PREPARED)
		{
			if(theData)
			{
				oneSlot newSlot;
				newSlot.allocated_receiver_to_sender = false;
				newSlot.allocated_sender_to_receiver = false;
				newSlot.rt_error_descr = "";
				newSlot.rt_error_id = -1;
				newSlot.rt_error_type = JVX_NO_ERROR;

				theData->slot_id = _common_spec_proc_set.active_slots.size();
				_common_spec_proc_set.active_slots.push_back(newSlot);

				theData->receiver.destination = NULL;
				theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
				theData->sender.source = NULL;
				theData->sender.tp = JVX_COMPONENT_UNKNOWN;

				theData->sender_to_receiver.buffersize = 0;
				theData->sender_to_receiver.format = JVX_DATAFORMAT_NONE;
				theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver = NULL;
				theData->sender_to_receiver.number_buffers = 0;
				theData->sender_to_receiver.number_channels = 0;
				theData->sender_to_receiver.rate = 0;
				theData->sender_to_receiver.user_hints = NULL;
				theData->sender_to_receiver.slot_id = theData->slot_id;

				theData->receiver_to_sender.buffersize = 0;
				theData->receiver_to_sender.format = JVX_DATAFORMAT_NONE;
				theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender = NULL;
				theData->receiver_to_sender.number_buffers = 0;
				theData->receiver_to_sender.number_channels = 0;
				theData->receiver_to_sender.rate = 0;
				theData->receiver_to_sender.user_hints = NULL;
				theData->receiver_to_sender.slot_id = theData->slot_id;

				return(JVX_NO_ERROR);
			}
			return(JVX_ERROR_INVALID_ARGUMENT);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	jvxErrorType _postprocess_data_link_slot(jvxSize* num_unDeallocated)
	{
		jvxSize i;
		if(_common_spec_proc_set.inDataLinkMode  == JVX_STATE_PREPARED)
		{
			if(num_unDeallocated)
			{
				*num_unDeallocated = 0;

				for(i = 0; i < _common_spec_proc_set.active_slots.size(); i++)
				{
					if(_common_spec_proc_set.active_slots[i].allocated_receiver_to_sender == true)
					{
						(*num_unDeallocated)++;
					}
					if(_common_spec_proc_set.active_slots[i].allocated_sender_to_receiver == true)
					{
						(*num_unDeallocated)++;
					}
				}
			}
		}
		return(JVX_NO_ERROR);
	}

	jvxErrorType _init_data_link(jvxLinkDataDescriptor* theData, bool sender_to_receiver, bool placeMemoryIntoExternalPointer)
	{

		jvxErrorType res = JVX_NO_ERROR;
		jvxSize i, j;
		jvxState theState = JVX_STATE_NONE;

		if(placeMemoryIntoExternalPointer)
		{
			theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
			theData->receiver.destination = _common_spec_proc_set.thisisme;
			if(theData->receiver.destination)
			{
				theData->receiver.destination->request_specialization(NULL, &theData->receiver.tp, NULL);
			}
			JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**, theData->sender_to_receiver.number_buffers);
			assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver);

			for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
			{
				theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i] = NULL;
				JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*, theData->sender_to_receiver.number_channels);
				assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i]);
				for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
				{
					theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j] = NULL;
					switch(theData->sender_to_receiver.format)
					{
					case JVX_DATAFORMAT_DATA:
						jvx_allocateField_cpp<jvxData>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
						break;
					case JVX_DATAFORMAT_64BIT_LE:
						jvx_allocateField_cpp<jvxInt64>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
						break;
					case JVX_DATAFORMAT_32BIT_LE:
					case JVX_DATAFORMAT_24BIT_LE:
						jvx_allocateField_cpp<jvxInt32>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
						break;
					case JVX_DATAFORMAT_16BIT_LE:
						jvx_allocateField_cpp<jvxInt16>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
						break;
					case JVX_DATAFORMAT_8BIT:
						jvx_allocateField_cpp<jvxInt8>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
						break;
					default:
						assert(0);
					}
					assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
				}
			}
		}
		else
		{
			if(_common_spec_proc_set.inDataLinkMode == JVX_STATE_PREPARED)
			{
				if(theData)
				{
					if(sender_to_receiver)
					{
						if(theData->sender_to_receiver.slot_id < _common_spec_proc_set.active_slots.size())
						{
							theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
							theData->receiver.destination = _common_spec_proc_set.thisisme;
							if(theData->receiver.destination)
							{
								theData->receiver.destination->request_specialization(NULL, &theData->receiver.tp, NULL);
							}
							JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**, theData->sender_to_receiver.number_buffers);
							assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver);

							for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
							{
								theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i] = NULL;
								JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*, theData->sender_to_receiver.number_channels);
								assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i]);
								for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
								{
									theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j] = NULL;
									switch(theData->sender_to_receiver.format)
									{
									case JVX_DATAFORMAT_DATA:
										jvx_allocateField_cpp<jvxData>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
										break;
									case JVX_DATAFORMAT_64BIT_LE:
										jvx_allocateField_cpp<jvxInt64>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
										break;
									case JVX_DATAFORMAT_32BIT_LE:
									case JVX_DATAFORMAT_24BIT_LE:
										jvx_allocateField_cpp<jvxInt32>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
										break;
									case JVX_DATAFORMAT_16BIT_LE:
										jvx_allocateField_cpp<jvxInt16>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
										break;
									case JVX_DATAFORMAT_8BIT:
										jvx_allocateField_cpp<jvxInt8>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j], theData->sender_to_receiver.buffersize);
										break;
									default:
										assert(0);
									}
									assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
								}
							}
							_common_spec_proc_set.active_slots[theData->sender_to_receiver.slot_id].allocated_sender_to_receiver = true;
						}
						else
						{
							res = JVX_ERROR_ID_OUT_OF_BOUNDS;
						}
					}
					else
					{
						if(theData->receiver_to_sender.slot_id < _common_spec_proc_set.active_slots.size())
						{
							theData->sender.tp = JVX_COMPONENT_UNKNOWN;
							theData->sender.source = _common_spec_proc_set.thisisme;
							if(theData->sender.source)
							{
								theData->sender.source->request_specialization(NULL, &theData->sender.tp, NULL);
							}

							JVX_SAFE_NEW_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender, jvxHandle**, theData->receiver_to_sender.number_buffers);
							assert(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender);

							for(i = 0; i < theData->receiver_to_sender.number_buffers; i++)
							{
								theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i] = NULL;
								JVX_SAFE_NEW_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i], jvxHandle*, theData->receiver_to_sender.number_channels);
								assert(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i]);
								for(j = 0; j < (jvxSize)theData->receiver_to_sender.number_channels; j++)
								{
									theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j] = NULL;
									switch(theData->receiver_to_sender.format)
									{
									case JVX_DATAFORMAT_DATA:
										jvx_allocateField_cpp<jvxData>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j], theData->receiver_to_sender.buffersize);
										break;
									case JVX_DATAFORMAT_64BIT_LE:
										jvx_allocateField_cpp<jvxInt64>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j], theData->receiver_to_sender.buffersize);
										break;
									case JVX_DATAFORMAT_32BIT_LE:
									case JVX_DATAFORMAT_24BIT_LE:
										jvx_allocateField_cpp<jvxInt32>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j], theData->receiver_to_sender.buffersize);
										break;
									case JVX_DATAFORMAT_16BIT_LE:
										jvx_allocateField_cpp<jvxInt16>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j], theData->receiver_to_sender.buffersize);
										break;
									case JVX_DATAFORMAT_8BIT:
										jvx_allocateField_cpp<jvxInt8>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j], theData->receiver_to_sender.buffersize);
										break;
									default:
										assert(0);
									}
									assert(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
								}
							}
							_common_spec_proc_set.active_slots[theData->receiver_to_sender.slot_id].allocated_receiver_to_sender = true;
						}
						else
						{
							res = JVX_ERROR_ID_OUT_OF_BOUNDS;
						}
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		return(res);
	};

	jvxErrorType _init_data_link_no_allocate(jvxLinkDataDescriptor* theData, bool sender_to_receiver, bool placeMemoryIntoExternalPointer)
	{

		jvxErrorType res = JVX_NO_ERROR;
		jvxSize i, j;
		jvxState theState = JVX_STATE_NONE;

		if(placeMemoryIntoExternalPointer)
		{
			theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
			theData->receiver.destination = _common_spec_proc_set.thisisme;
			if(theData->receiver.destination)
			{
				theData->receiver.destination->request_specialization(NULL, &theData->receiver.tp, NULL);
			}
			JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**, theData->sender_to_receiver.number_buffers);
			assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver);

			for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
			{
				theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i] = NULL;
				JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*, theData->sender_to_receiver.number_channels);
				assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i]);
				for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
				{
					theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j] = NULL;
					/* No allocation here.. */
				}
			}
		}
		else
		{
			if(_common_spec_proc_set.inDataLinkMode == JVX_STATE_PREPARED)
			{
				if(theData)
				{
					if(sender_to_receiver)
					{
						if(theData->sender_to_receiver.slot_id < _common_spec_proc_set.active_slots.size())
						{
							theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
							theData->receiver.destination = _common_spec_proc_set.thisisme;
							if(theData->receiver.destination)
							{
								theData->receiver.destination->request_specialization(NULL, &theData->receiver.tp, NULL);
							}
							JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**, theData->sender_to_receiver.number_buffers);
							assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver);

							for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
							{
								theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i] = NULL;
								JVX_SAFE_NEW_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*, theData->sender_to_receiver.number_channels);
								assert(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i]);
								for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
								{
									theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j] = NULL;
									/* No allocation here */
								}
							}
							_common_spec_proc_set.active_slots[theData->sender_to_receiver.slot_id].allocated_sender_to_receiver = true;
						}
						else
						{
							res = JVX_ERROR_ID_OUT_OF_BOUNDS;
						}
					}
					else
					{
						if(theData->receiver_to_sender.slot_id < _common_spec_proc_set.active_slots.size())
						{
							theData->sender.tp = JVX_COMPONENT_UNKNOWN;
							theData->sender.source = _common_spec_proc_set.thisisme;
							if(theData->sender.source)
							{
								theData->sender.source->request_specialization(NULL, &theData->receiver.tp, NULL);
							}

							JVX_SAFE_NEW_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender, jvxHandle**, theData->receiver_to_sender.number_buffers);
							assert(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender);

							for(i = 0; i < theData->receiver_to_sender.number_buffers; i++)
							{
								theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i] = NULL;
								JVX_SAFE_NEW_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i], jvxHandle*, theData->receiver_to_sender.number_channels);
								assert(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i]);
								for(j = 0; j < (jvxSize)theData->receiver_to_sender.number_channels; j++)
								{
									theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j] = NULL;
									/* No allocation */
								}
							}
							_common_spec_proc_set.active_slots[theData->receiver_to_sender.slot_id].allocated_receiver_to_sender = true;
						}
						else
						{
							res = JVX_ERROR_ID_OUT_OF_BOUNDS;
						}
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		return(res);
	};

	jvxErrorType _release_data_link(jvxLinkDataDescriptor* theData, bool sender_to_receiver, bool placeMemoryIntoExternalPointer)
	{
		jvxSize i, j;
		jvxErrorType res = JVX_NO_ERROR;
		jvxState theState = JVX_STATE_NONE;

		if(placeMemoryIntoExternalPointer)
		{
			// Here, we are allowed to deallocate
			for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
			{
				for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
				{
					switch(theData->sender_to_receiver.format)
					{
					case JVX_DATAFORMAT_DATA:
						jvx_deallocateField_cpp<jvxData>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
						break;
					case JVX_DATAFORMAT_64BIT_LE:
						jvx_deallocateField_cpp<jvxInt64>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
						break;
					case JVX_DATAFORMAT_32BIT_LE:
					case JVX_DATAFORMAT_24BIT_LE:
						jvx_deallocateField_cpp<jvxInt32>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
						break;
					case JVX_DATAFORMAT_16BIT_LE:
						jvx_deallocateField_cpp<jvxInt16>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
						break;
					case JVX_DATAFORMAT_8BIT:
						jvx_deallocateField_cpp<jvxInt8>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
						break;
					default:
						assert(0);
					}
				}
				JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*);
			}

			JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**);

			theData->receiver.destination = NULL;
			theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
		}
		else
		{
			if(_common_spec_proc_set.inDataLinkMode == JVX_STATE_PREPARED)
			{
				if(theData)
				{
					if(sender_to_receiver)
					{
						if(theData->receiver.destination == _common_spec_proc_set.thisisme)
						{
							if(theData->sender_to_receiver.slot_id < _common_spec_proc_set.active_slots.size())
							{
								// Here, we are allowed to deallocate
								for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
								{
									for(j = 0; j < (jvxSize)theData->sender_to_receiver.number_channels; j++)
									{
										switch(theData->sender_to_receiver.format)
										{
										case JVX_DATAFORMAT_DATA:
											jvx_deallocateField_cpp<jvxData>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
											break;
										case JVX_DATAFORMAT_64BIT_LE:
											jvx_deallocateField_cpp<jvxInt64>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
											break;
										case JVX_DATAFORMAT_32BIT_LE:
										case JVX_DATAFORMAT_24BIT_LE:
											jvx_deallocateField_cpp<jvxInt32>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
											break;
										case JVX_DATAFORMAT_16BIT_LE:
											jvx_deallocateField_cpp<jvxInt16>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
											break;
										case JVX_DATAFORMAT_8BIT:
											jvx_deallocateField_cpp<jvxInt8>(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i][j]);
											break;
										default:
											assert(0);
										}
									}
									JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*);
								}

								JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**);

								theData->receiver.destination = NULL;
								theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
								_common_spec_proc_set.active_slots[theData->sender_to_receiver.slot_id].allocated_sender_to_receiver = false;
							}
							else
							{
								res = JVX_ERROR_ID_OUT_OF_BOUNDS;
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
					}
					else
					{
						if(theData->sender.source == _common_spec_proc_set.thisisme)
						{
							if(theData->receiver_to_sender.slot_id < _common_spec_proc_set.active_slots.size())
							{
								// Here, we are allowed to deallocate
								for(i = 0; i < theData->receiver_to_sender.number_buffers; i++)
								{
									for(j = 0; j < (jvxSize)theData->receiver_to_sender.number_channels; j++)
									{
										switch(theData->receiver_to_sender.format)
										{
										case JVX_DATAFORMAT_DATA:
											jvx_deallocateField_cpp<jvxData>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
											break;
										case JVX_DATAFORMAT_64BIT_LE:
											jvx_deallocateField_cpp<jvxInt64>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
											break;
										case JVX_DATAFORMAT_32BIT_LE:
										case JVX_DATAFORMAT_24BIT_LE:
											jvx_deallocateField_cpp<jvxInt32>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
											break;
										case JVX_DATAFORMAT_16BIT_LE:
											jvx_deallocateField_cpp<jvxInt16>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
											break;
										case JVX_DATAFORMAT_8BIT:
											jvx_deallocateField_cpp<jvxInt8>(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i][j]);
											break;
										default:
											assert(0);
										}
									}
									JVX_SAFE_DELETE_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i], jvxHandle*);
								}

								JVX_SAFE_DELETE_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender, jvxHandle**);

								theData->receiver.destination = NULL;
								theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
								_common_spec_proc_set.active_slots[theData->receiver_to_sender.slot_id].allocated_receiver_to_sender = false;
							}
							else
							{
								res = JVX_ERROR_ID_OUT_OF_BOUNDS;
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		return(res);
	};

	jvxErrorType _release_data_link_no_allocate(jvxLinkDataDescriptor* theData, bool sender_to_receiver, bool placeMemoryIntoExternalPointer)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;
		jvxState theState = JVX_STATE_NONE;

		if(placeMemoryIntoExternalPointer)
		{
			// Here, we are allowed to deallocate
			for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
			{
				JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*);
			}

			JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**);

			theData->receiver.destination = NULL;
			theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
		}
		else
		{
			if(_common_spec_proc_set.inDataLinkMode == JVX_STATE_PREPARED)
			{
				if(theData)
				{
					if(sender_to_receiver)
					{
						if(theData->receiver.destination == _common_spec_proc_set.thisisme)
						{
							if(theData->sender_to_receiver.slot_id < _common_spec_proc_set.active_slots.size())
							{
								// Here, we are allowed to deallocate
								for(i = 0; i < theData->sender_to_receiver.number_buffers; i++)
								{

									JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[i], jvxHandle*);
								}

								JVX_SAFE_DELETE_FLD(theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver, jvxHandle**);

								theData->receiver.destination = NULL;
								theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
								_common_spec_proc_set.active_slots[theData->sender_to_receiver.slot_id].allocated_sender_to_receiver = false;
							}
							else
							{
								res = JVX_ERROR_ID_OUT_OF_BOUNDS;
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
					}
					else
					{
						if(theData->sender.source == _common_spec_proc_set.thisisme)
						{
							if(theData->receiver_to_sender.slot_id < _common_spec_proc_set.active_slots.size())
							{
								// Here, we are allowed to deallocate
								for(i = 0; i < theData->receiver_to_sender.number_buffers; i++)
								{
									JVX_SAFE_DELETE_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[i], jvxHandle*);
								}

								JVX_SAFE_DELETE_FLD(theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender, jvxHandle**);

								theData->receiver.destination = NULL;
								theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
								_common_spec_proc_set.active_slots[theData->receiver_to_sender.slot_id].allocated_receiver_to_sender = false;
							}
							else
							{
								res = JVX_ERROR_ID_OUT_OF_BOUNDS;
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		return(res);
	};

	jvxErrorType _supports_multithreading(jvxBool* supports)
	{
		if(supports)
		{
			*supports = _common_spec_proc_set.supportsMultiThreading;
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _process_st( jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender)
	{
		// Talkthrough
		jvxSize ii;
		jvxSize maxChans = JVX_MAX(theData->sender_to_receiver.number_channels,theData->receiver_to_sender.number_channels);
		jvxSize minChans = JVX_MIN(theData->sender_to_receiver.number_channels,theData->receiver_to_sender.number_channels);

		// This default function does not tolerate a lot of unexpected settings
		assert(theData->sender_to_receiver.format == theData->receiver_to_sender.format);
		assert(theData->sender_to_receiver.buffersize == theData->receiver_to_sender.buffersize);
		assert(theData->receiver_to_sender.number_buffers == 1);
		assert(theData->sender_to_receiver.number_buffers == 1);

		if(minChans)
		{
			for(ii = 0; ii < maxChans; ii++)
			{
				jvxSize idxIn = ii % theData->sender_to_receiver.number_channels;
				jvxSize idxOut = ii % theData->receiver_to_sender.number_channels;

				assert(theData->sender_to_receiver.format == theData->receiver_to_sender.format);
				jvx_convertSamples_memcpy(
					theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver[idx_sender_to_receiver][idxIn],
					theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender[idx_receiver_to_sender][idxOut],
					jvxDataFormat_size[theData->sender_to_receiver.format],
					theData->sender_to_receiver.buffersize);
			}
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _process_mt(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender, jvxSize* channelSelect, jvxSize numEntriesChannels, jvxInt32 offset_input, jvxInt32 offset_output, jvxInt32 numEntries)
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	jvxErrorType _last_error_process(char* fld_text, jvxSize fldSize, jvxErrorType* err, jvxInt32* id_error, jvxLinkDataDescriptor* theData)
	{
		jvxErrorType res = JVX_NO_ERROR;
		jvxState theState = JVX_STATE_NONE;

		if(theState == JVX_STATE_PROCESSING)
		{
			assert(theData->slot_id < _common_spec_proc_set.active_slots.size());
			if(fld_text)
			{
				memset(fld_text, 0, fldSize);
				jvxSize mSize = JVX_MIN(fldSize, _common_spec_proc_set.active_slots[theData->slot_id].rt_error_descr.size() -1);
				memcpy(fld_text, _common_spec_proc_set.active_slots[theData->slot_id].rt_error_descr.c_str(), mSize);
			}
			if(err)
			{
				*err = _common_spec_proc_set.active_slots[theData->slot_id].rt_error_type;
			}
			if(id_error)
			{
				*id_error = _common_spec_proc_set.active_slots[theData->slot_id].rt_error_id;
			}

			_common_spec_proc_set.active_slots[theData->slot_id].rt_error_descr = "";
			_common_spec_proc_set.active_slots[theData->slot_id].rt_error_id = -1;
			_common_spec_proc_set.active_slots[theData->slot_id].rt_error_type = JVX_NO_ERROR;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	void _safe_neutral_contents(jvxLinkDataDescriptor* theData)
	{
		theData->receiver.destination = NULL;
		theData->receiver.tp = JVX_COMPONENT_UNKNOWN;
		theData->sender.source = NULL;
		theData->sender.tp = JVX_COMPONENT_UNKNOWN;

		theData->receiver_to_sender.buffersize = 0;
		theData->receiver_to_sender.format = JVX_DATAFORMAT_NONE;
		theData->receiver_to_sender.from_receiver_buffer_allocated_by_sender = NULL;
		theData->receiver_to_sender.number_buffers = 0;
		theData->receiver_to_sender.number_channels = 0;
		theData->receiver_to_sender.rate = 0;
		theData->receiver_to_sender.slot_id = -1;

		theData->sender_to_receiver.buffersize = 0;
		theData->sender_to_receiver.format = JVX_DATAFORMAT_NONE;
		theData->sender_to_receiver.to_receiver_buffer_allocated_by_receiver = NULL;
		theData->sender_to_receiver.number_buffers = 0;
		theData->sender_to_receiver.number_channels = 0;
		theData->sender_to_receiver.rate = 0;
		theData->sender_to_receiver.slot_id = -1;

		theData->slot_id = -1;
	};

};

#endif
