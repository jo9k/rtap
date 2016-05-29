#ifndef __HJVXSEQUENCER_H__
#define __HJVXSEQUENCER_H__

// ==================================================================
// ==================================================================

static std::string jvxSequencerElementType_str[] =
{
	"none",
	"activate",
	"prepare",
	"start",
	"stop",
	"post process",
	"deactivate",
	"condition wait",
	"cond. jump",
	"jump",
	"command",
	"wait until stop",
	"relative jump",
	"break",
	"request viewer update",
	"start processing loop",
	"stop processing loop",
	"callback",
">limit<"
};


inline std::string jvxSequencerElementType_txt(jvxSize id)
{
	assert(id < JVX_SEQUENCER_TYPE_COMMAND_LIMIT);
	return(jvxSequencerElementType_str[id]);
}

#endif
