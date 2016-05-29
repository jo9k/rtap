#ifndef __HJVXDATAPROCESSOR_H__
#define __HJVXDATAPROCESSOR_H__

template<typename Tfrom, typename Tto> static void jvx_convertSamples_from_fxp_norm_to_flp(Tfrom* source, Tto* dest, jvxSize numSamples, Tto norm, 
										jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Tto accu; 	
	source += offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accu = (Tto) *source;
		source += hopFrom; 
		accu *= norm;
		*dest = accu;
		dest += hopTo;
	}
}

// =======================================================================================

template<typename Taccu, typename Tto> static void jvx_convertSamples_from_bytes_norm_to_flp(char* source, Tto* dest, jvxSize numSamples, Tto norm, jvxSize numBytes,
												jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Tto accuTo; 	
	Taccu accuFrom; 
	source += (offsetFrom*numBytes);
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = 0;
		memcpy(&accuFrom, source, numBytes);
		accuTo = (Tto)accuFrom ;
		accuTo *= norm;
		*dest = accuTo;
		source += numBytes * hopFrom;
		dest += hopTo;
	}
}

// =============================================================================================================

template<typename Tfrom, typename Tto> static void jvx_convertSamples_from_flp_norm_to_fxp(Tfrom* source, Tto* dest, jvxSize numSamples, Tfrom norm,
													jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Tfrom accu; 	
	source += offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accu = *source;
		accu *= norm;
		*dest = (Tto)accu;
		source += hopFrom;
		dest += hopTo;
	}
}

// =======================================================================================

template<typename Tfrom, typename Taccu> static void jvx_convertSamples_from_flp_norm_to_bytes(Tfrom* source, char* dest, jvxSize numSamples, Tfrom norm, jvxSize numBytes,
											jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Tfrom accuFrom; 	
	Taccu accuTo;
	source += offsetFrom;
	dest += (numBytes * offsetTo);
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = *source * norm;
		accuTo = (Taccu)accuFrom;
		memcpy(dest, &accuTo, numBytes);
		source += hopFrom;
		dest += (numBytes * hopTo);
	}
}

// =============================================================================================================

template<typename Tto, typename Taccu> static void jvx_convertSamples_from_bytes_shiftleft_to_fxp(
	char* source, Tto* dest, jvxSize numSamples, jvxSize numBytes, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Taccu accuFrom;
	source += numBytes * offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = 0;
		memcpy(&accuFrom, source, numBytes);
		accuFrom = accuFrom << numShift;
		*dest = (Tto)accuFrom;
		source += numBytes * hopFrom;
		dest += hopTo;
	}
}

template<typename Tto, typename Taccu> static void jvx_convertSamples_from_bytes_shiftright_to_fxp(
	char* source, Tto* dest, jvxSize numSamples, jvxSize numBytes, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Taccu accuFrom; 
	source += numBytes * offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = 0;
		memcpy(&accuFrom, source, numBytes);
		accuFrom = accuFrom >> numShift;
		*dest = (Tto)accuFrom;
		source += numBytes * hopFrom;
		dest += hopTo;
	}
}

// =======================================================================================

static void jvx_convertSamples_memcpy(void* from, void* to, jvxSize szElm, jvxSize numElms)
{
	memcpy(to, from, szElm * numElms);
}

static void jvx_convertSamples_memcpy_offset(void* from, void* to, jvxSize szElm, jvxSize numElms, jvxInt32 offsetInput, jvxInt32 offsetOutput)
{
	memcpy(((jvxByte*)to) + (offsetOutput*szElm), ((jvxByte*)from) + (szElm * offsetInput), szElm * numElms);
}

template<typename Tfrom, typename Tto, typename Taccu> static void jvx_convertSamples_from_fxp_noshift_to_fxp(
	Tfrom* source, Tto* dest, jvxSize numSamples, 
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;	
	Taccu accuFrom;
	source += offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = (Taccu) *source;
		accuFrom = accuFrom;
		*dest = (Tto)accuFrom;
		source += hopFrom;
		dest += hopTo;
	}
}
// =======================================================================================

template<typename Tfrom, typename Tto, typename Taccu> static void jvx_convertSamples_from_fxp_shiftleft_to_fxp(
	Tfrom* source, Tto* dest, jvxSize numSamples, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;	
	Taccu accuFrom;
	source += offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = (Taccu) *source;
		accuFrom = accuFrom << numShift;
		*dest = (Tto)accuFrom;
		source += hopFrom;
		dest += hopTo;
	}
}

template<typename Tfrom, typename Tto, typename Taccu> static void jvx_convertSamples_from_fxp_shiftright_to_fxp(
	Tfrom* source, Tto* dest, jvxSize numSamples, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;	
	Taccu accuFrom;
	source += offsetFrom;
	dest += offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = (Taccu) *source;
		accuFrom = accuFrom >> numShift;
		*dest = (Tto)accuFrom;
		source += hopFrom;
		dest += hopTo;
	}
}

// =======================================================================================

template<typename Tfrom, typename Taccu> static void jvx_convertSamples_from_fxp_shiftleft_to_bytes(
	Tfrom* source, char* dest, jvxSize numSamples, jvxSize numBytes, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Taccu accuFrom;
	source += offsetFrom;
	dest += offsetTo * numBytes;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = (Taccu)*source;
		accuFrom = accuFrom << numShift;
		memcpy(dest, &accuFrom, numBytes);
		source += hopFrom;
		dest += numBytes * hopTo;
	}
}

template<typename Tfrom, typename Taccu> static void jvx_convertSamples_from_fxp_shiftright_to_bytes(
	Tfrom* source, char* dest, jvxSize numSamples, jvxSize numBytes, jvxSize numShift,
	jvxSize offsetFrom = 0, jvxSize hopFrom = 1, jvxSize offsetTo = 0, jvxSize hopTo = 1)
{
	jvxSize i;
	Taccu accuFrom;
	source += offsetFrom;
	dest += numBytes * offsetTo;
	for(i = 0; i < numSamples; i++)
	{
		accuFrom = (Taccu)*source;
		accuFrom = accuFrom >> numShift;
		memcpy(dest, &accuFrom, numBytes);
		source += hopFrom;
		dest += numBytes * hopTo;
	}
}

// =======================================================================================

static jvxErrorType jvx_fieldLevelGainClip(jvxHandle** buf, jvxSize numC, jvxSize buffersize, jvxDataFormat format,
								jvxData* avrgVal, jvxData* maxVal, jvxData* gainVal, jvxData clipVal,
								jvxData smooth)
{
	jvxSize i,j;
	jvxData* ptrDbl = NULL;
	jvxData* ptrFlt = NULL;
	jvxInt32* ptrInt32 = NULL;
	jvxInt64* ptrInt64 = NULL;
	jvxInt16* ptrInt16 = NULL;
	jvxInt8* ptrInt8 = NULL;
	jvxData tmp;

	switch(format)
	{
	case JVX_DATAFORMAT_DATA:
		for(i = 0; i < numC; i++)
		{
			ptrDbl = reinterpret_cast<jvxData*>(buf[i]);
			for(j = 0; j < buffersize; j++)
			{
				tmp = *ptrDbl;
				tmp *= gainVal[i];
				if(tmp > clipVal)
				{
					tmp = clipVal;
				}
				if(tmp < -clipVal)
				{
					tmp = clipVal;
				}
				*ptrDbl++ = tmp;

				tmp *= tmp;
				if(tmp > maxVal[i])
				{
					maxVal[i] = tmp;
				}
				tmp = avrgVal[i] * (smooth) + (1-smooth) * tmp;
				avrgVal[i] = tmp;
			}
		}
		break;

	case JVX_DATAFORMAT_8BIT:
		for(i = 0; i < numC; i++)
		{
			ptrInt8 = reinterpret_cast<jvxInt8*>(buf[i]);
			for(j = 0; j < buffersize; j++)
			{
				tmp = JVX_INT8_2_DATA(*ptrInt8);
				tmp *= gainVal[i];
				if(tmp > clipVal)
				{
					tmp = clipVal;
				}
				if(tmp < -clipVal)
				{
					tmp = clipVal;
				}
				*ptrInt8++ = JVX_DATA_2_INT8(tmp);

				tmp *= tmp;
				if(tmp > maxVal[i])
				{
					maxVal[i] = tmp;
				}
				tmp = avrgVal[i] * (smooth) + (1-smooth) * tmp;
				avrgVal[i] = tmp;
			}
		}
		break;

	case JVX_DATAFORMAT_16BIT_LE:
		for(i = 0; i < numC; i++)
		{
			ptrInt16 = reinterpret_cast<jvxInt16*>(buf[i]);
			for(j = 0; j < buffersize; j++)
			{
				tmp = JVX_INT16_2_DATA(*ptrInt16);
				tmp *= gainVal[i];
				if(tmp > clipVal)
				{
					tmp = clipVal;
				}
				if(tmp < -clipVal)
				{
					tmp = clipVal;
				}
				*ptrInt16++ = JVX_DATA_2_INT16(tmp);

				tmp *= tmp;
				if(tmp > maxVal[i])
				{
					maxVal[i] = tmp;
				}
				tmp = avrgVal[i] * (smooth) + (1-smooth) * tmp;
				avrgVal[i] = tmp;
			}
		}
		break;

	case JVX_DATAFORMAT_24BIT_LE:
	case JVX_DATAFORMAT_32BIT_LE:
		for(i = 0; i < numC; i++)
		{
			ptrInt32 = reinterpret_cast<jvxInt32*>(buf[i]);
			for(j = 0; j < buffersize; j++)
			{
				tmp = JVX_INT32_2_DATA(*ptrInt32);
				tmp *= gainVal[i];
				if(tmp > clipVal)
				{
					tmp = clipVal;
				}
				if(tmp < -clipVal)
				{
					tmp = clipVal;
				}
				*ptrInt32++ = JVX_DATA_2_INT32(tmp);

				tmp *= tmp;
				if(tmp > maxVal[i])
				{
					maxVal[i] = tmp;
				}
				tmp = avrgVal[i] * (smooth) + (1-smooth) * tmp;
				avrgVal[i] = tmp;
			}
		}
		break;

	case JVX_DATAFORMAT_64BIT_LE:
		for(i = 0; i < numC; i++)
		{
			ptrInt64 = reinterpret_cast<jvxInt64*>(buf[i]);
			for(j = 0; j < buffersize; j++)
			{
				tmp = JVX_INT64_2_DATA(*ptrInt16);
				tmp *= gainVal[i];
				if(tmp > clipVal)
				{
					tmp = clipVal;
				}
				if(tmp < -clipVal)
				{
					tmp = clipVal;
				}
				*ptrInt64++ = JVX_DATA_2_INT64(tmp);

				tmp *= tmp;
				if(tmp > maxVal[i])
				{
					maxVal[i] = tmp;
				}
				tmp = avrgVal[i] * (smooth) + (1-smooth) * tmp;
				avrgVal[i] = tmp;
			}
		}
		break;
	}
	return(JVX_NO_ERROR);
}

// =======================================================================================

static jvxErrorType jvx_fieldMix(jvxHandle* buf_addthis, jvxHandle* buf_tothis, jvxSize buffersize, jvxDataFormat format)
{
	jvxSize j;
	jvxData* ptrDbl1 = NULL, * ptrDbl2 = NULL;
	jvxData* ptrFlt1 = NULL, * ptrFlt2 = NULL;
	jvxInt32* ptrInt321 = NULL, * ptrInt322 = NULL;
	jvxInt64* ptrInt641 = NULL, * ptrInt642 = NULL;
	jvxInt16* ptrInt161 = NULL, * ptrInt162 = NULL;
	jvxInt8* ptrInt81 = NULL, * ptrInt82 = NULL;
	jvxData tmp1, tmp2;

	switch(format)
	{
	case JVX_DATAFORMAT_DATA:
		ptrDbl1 = reinterpret_cast<jvxData*>(buf_addthis);
		ptrDbl2 = reinterpret_cast<jvxData*>(buf_tothis);
		for(j = 0; j < buffersize; j++)
		{
			tmp1 = *ptrDbl1++;
			tmp2 = *ptrDbl2;
			tmp2 += tmp1;
			*ptrDbl2++ = tmp2;				
		}
		break;


	case JVX_DATAFORMAT_8BIT:
		ptrInt81 = reinterpret_cast<jvxInt8*>(buf_addthis);
		ptrInt82 = reinterpret_cast<jvxInt8*>(buf_tothis);
		for(j = 0; j < buffersize; j++)
		{
			tmp1 = JVX_INT8_2_DATA(*ptrInt81);
			tmp2 = JVX_INT8_2_DATA(*ptrInt82);
			tmp2 += tmp1;
			*ptrInt82 = JVX_DATA_2_INT8(tmp2);				
			ptrInt81++;
			ptrInt82++;
		}

		break;

	case JVX_DATAFORMAT_16BIT_LE:
		ptrInt161 = reinterpret_cast<jvxInt16*>(buf_addthis);
		ptrInt162 = reinterpret_cast<jvxInt16*>(buf_tothis);
		for(j = 0; j < buffersize; j++)
		{
			tmp1 = JVX_INT16_2_DATA(*ptrInt161);
			tmp2 = JVX_INT16_2_DATA(*ptrInt162);
			tmp2 += tmp1;
			*ptrInt162 = JVX_DATA_2_INT16(tmp2);				
			ptrInt161++;
			ptrInt162++;
		}

		break;

	case JVX_DATAFORMAT_24BIT_LE:
	case JVX_DATAFORMAT_32BIT_LE:
		ptrInt321 = reinterpret_cast<jvxInt32*>(buf_addthis);
		ptrInt322 = reinterpret_cast<jvxInt32*>(buf_tothis);
		for(j = 0; j < buffersize; j++)
		{
			tmp1 = JVX_INT32_2_DATA(*ptrInt321);
			tmp2 = JVX_INT32_2_DATA(*ptrInt322);
			tmp2 += tmp1;
			*ptrInt322 = JVX_DATA_2_INT32(tmp2);				
			ptrInt321++;
			ptrInt322++;
		}

		break;

	case JVX_DATAFORMAT_64BIT_LE:
		ptrInt641 = reinterpret_cast<jvxInt64*>(buf_addthis);
		ptrInt642 = reinterpret_cast<jvxInt64*>(buf_tothis);
		for(j = 0; j < buffersize; j++)
		{
			tmp1 = JVX_INT64_2_DATA(*ptrInt641);
			tmp2 = JVX_INT64_2_DATA(*ptrInt642);
			tmp2 += tmp1;
			*ptrInt642 = JVX_DATA_2_INT64(tmp2);				
			ptrInt641++;
			ptrInt642++;
		}

		break;
	}
	return(JVX_NO_ERROR);
}


static jvxDataProcessorHintDescriptor* 
jvx_hintDesriptor_push_front(jvxHandle* hint, jvxDataProcessorHintDescriptorType hintDescriptorId, jvxComponentType assocHint, jvxDataProcessorHintDescriptor* next)
{
	jvxDataProcessorHintDescriptor* newElm = new jvxDataProcessorHintDescriptor;
	newElm->hint = hint;
	newElm->hintDescriptorId = hintDescriptorId;
	newElm->assocHint = assocHint;
	newElm->next = next;
	return(newElm);
}

static jvxDataProcessorHintDescriptor* 
jvx_hintDesriptor_pop_front(jvxDataProcessorHintDescriptor* popElm, jvxHandle** hint, jvxDataProcessorHintDescriptorType* hintDescriptorId, jvxComponentType* assocHint)
{
	jvxDataProcessorHintDescriptor* returnMe = NULL;
	if(hint)
	{
		*hint =popElm->hint;		
	}		
	
	if(hintDescriptorId)
	{
		*hintDescriptorId = popElm->hintDescriptorId;
	}
	if(assocHint)
	{
		*assocHint = popElm->assocHint;
	}
	returnMe = popElm->next;
	delete(popElm);
	return(returnMe);
}









#endif
