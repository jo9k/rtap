#ifndef __HJVXAUDIOFILEIO_H__
#define __HJVXAUDIOFILEIO_H__


typedef struct
{
	/*
	JVX_AUDIO_FILE_TAG_BTFLD_ARTIST = 0x1,
	JVX_AUDIO_FILE_TAG_BTFLD_TITLE = 0x2,
	JVX_AUDIO_FILE_TAG_BTFLD_CREATION_DATA = 0x4,
	JVX_AUDIO_FILE_TAG_BTFLD_GENRE = 0x8
	*/
	jvxBitField tagsSet;
	std::string tags[JVX_NUMBER_AUDIO_FILE_TAGS];

} jvxAssignedFileTags;

static void
jvx_initTagName(jvxAssignedFileTags& tags)
{
	int i;
	tags.tagsSet = 0;
	for(i = 0; i < JVX_NUMBER_AUDIO_FILE_TAGS; i++)
	{
		tags.tags[i].clear();
	}
}

#define JVX_GENERIC_HOST_FNAME_PREFIX "jvx_record_"
static jvxErrorType
jvx_tagExprToTagStruct(std::string tagDescr, jvxAssignedFileTags& lst, std::string& path)
{
	int i,j;
	std::string lookforTags = "<TAGS>";
	std::string lookforPath = "<PATH>";
	std::string tok1;
	std::string tok_remain;
	std::string collect;
	std::string trash;
	jvxInt32 idxFound = false;
	jvx_initTagName(lst);

	tok1 = tagDescr.substr(0,lookforTags.size());
	if(tagDescr.size() < lookforTags.size())
	{
		return(JVX_ERROR_PARSE_ERROR);
	}
	tok_remain = tagDescr.substr(lookforTags.size(), std::string::npos);
	if(tok1 == lookforTags)
	{
		while(tok_remain.size() >= 4)
		{
			// Tags always with 4 chars
			tok1 = tok_remain.substr(0,4);

			tok_remain = tok_remain.substr(4, std::string::npos);

			idxFound = -1;
			for(i = 0; i < JVX_NUMBER_AUDIO_FILE_TAGS; i++)
			{
				lookforTags = jvxAudioFileTagType_txt[i];
				if(lookforTags == tok1)
				{
					idxFound = i;
					break;
				}
			}
			if(idxFound >= 0)
			{
				collect.clear();
				trash = tok_remain.substr(0,1);
				if(tok_remain.size() < 1)
				{
					return(JVX_ERROR_PARSE_ERROR);
				}
				tok_remain = tok_remain.substr(1, std::string::npos);
				if(trash == ":")
				{
					while(1)
					{
						trash = tok_remain.substr(0,1);
						if(tok_remain.size() < 1)
						{
							return(JVX_ERROR_PARSE_ERROR);
						}
						tok_remain = tok_remain.substr(1, std::string::npos);
						if(trash != ":")
						{
							collect+= trash;
						}
						else
						{
							break;
						}
					}

					int count = 0;
					count = atoi(collect.c_str());
					collect.clear();
					for(j = 0; j < count; j++)
					{
						trash = tok_remain.substr(0,1);
						if(tok_remain.size() < 1)
						{
							return(JVX_ERROR_PARSE_ERROR);
						}
						tok_remain = tok_remain.substr(1, std::string::npos);

						collect += trash;
					}

					lst.tags[idxFound] = collect;
					lst.tagsSet |= (jvxBitField)1 << idxFound;
				}
				else
				{
					return(JVX_ERROR_PARSE_ERROR);
				}
			}
			else
			{
				return(JVX_ERROR_PARSE_ERROR);
			}
		}
	}
	else if(tok1 == lookforPath)
	{
		path = tok_remain;
	}
	else
	{
		return(JVX_ERROR_PARSE_ERROR);
	}
	return(JVX_NO_ERROR);
}

static std::string
jvx_tagStructToUserView(jvxAssignedFileTags& tags)
{
	int j;
	std::string retString = "";
	for(j = 0; j < JVX_NUMBER_AUDIO_FILE_TAGS; j++)
	{
		if(tags.tagsSet & (jvxBitField)1 << j)
		{
			if(!retString.empty())
			{
				retString += ", " + tags.tags[j];
			}
			else
			{
				retString += tags.tags[j];
			}
		}
	}
	return(retString);
}

static jvxErrorType
jvx_tagStructToTagExpr(std::string& tagExpr, jvxAssignedFileTags lst, std::string path)
{
	int i;
	std::string txt;
	if(lst.tagsSet)
	{
		tagExpr = "<TAGS>";
		for(i = 0; i < JVX_NUMBER_AUDIO_FILE_TAGS; i++)
		{
			if(lst.tagsSet & (jvxBitField)1 << i)
			{
				tagExpr += jvxAudioFileTagType_txt[i];
				txt = lst.tags[i];
				tagExpr += ":" + jvx_int2String(txt.size()) + ":";
				tagExpr += txt;
			}
		}
	}
	else
	{
		tagExpr = "<PATH>";
		tagExpr += path;
	}
	return(JVX_NO_ERROR);
}

static void
jvx_produceFilenameWavOut(std::string& fNameOut, std::string path)
{
	fNameOut = JVX_GENERIC_HOST_FNAME_PREFIX;
	fNameOut += JVX_DATE();
	fNameOut += "__";
	fNameOut += JVX_TIME();
	fNameOut = jvx_replaceCharacter( fNameOut,'/', '_');
	fNameOut = jvx_replaceCharacter( fNameOut,':', '_');
	fNameOut = path + "/" + fNameOut;
	fNameOut += ".wav";
}

#endif
