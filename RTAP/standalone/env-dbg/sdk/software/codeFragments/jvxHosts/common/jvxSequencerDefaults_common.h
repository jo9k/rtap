#ifndef __JVXSEQUENCERDEFAULTS_COMMON_H__
#define __JVXSEQUENCERDEFAULTS_COMMON_H__

// ##################################################################################
// ##################################################################################

#define JVX_NUMBER_DEFAULT_STEPS_SEQUENCER_RUN 8
static oneSequencerStep defaultSteps_run[JVX_NUMBER_DEFAULT_STEPS_SEQUENCER_RUN] =
{
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE, JVX_COMPONENT_AUDIO_NODE, "Prepare Audio Node", "PrepANode", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE, JVX_COMPONENT_AUDIO_DEVICE, "Prepare Audio Device", "PrepADevice", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_CALLBACK, JVX_COMPONENT_UNKNOWN, "Callback after all prepared", "AfterAllPrep", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_COMMAND_STD_START, JVX_COMPONENT_AUDIO_NODE, "Start Audio Node", "StartANode", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_COMMAND_STD_START, JVX_COMPONENT_AUDIO_DEVICE, "Start Audio Device", "StartADevice", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_CALLBACK, JVX_COMPONENT_UNKNOWN, "Callback after all started", "AfterAllStarted", 1, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER, JVX_COMPONENT_UNKNOWN, "Update Viewer", "UpdateViewer", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_RUN, JVX_SEQUENCER_TYPE_BREAK, JVX_COMPONENT_UNKNOWN, "Break sequencer steps", "Break", 0, -1, "no label", "no label"}
};

#define JVX_NUMBER_DEFAULT_STEPS_SEQUENCER_LEAVE 6
static oneSequencerStep defaultSteps_leave[JVX_NUMBER_DEFAULT_STEPS_SEQUENCER_LEAVE] =
{
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_CALLBACK, JVX_COMPONENT_UNKNOWN, "Callback before all stop", "BeforeAllStop", 16, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_COMMAND_STD_STOP, JVX_COMPONENT_AUDIO_DEVICE, "Stop Audio Device", "StopADevice", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_COMMAND_STD_STOP, JVX_COMPONENT_AUDIO_NODE, "Stop Audio Node", "StopANode", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_CALLBACK, JVX_COMPONENT_UNKNOWN, "Callback before all postprocess", "BeforeAllPost", 17, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS, JVX_COMPONENT_AUDIO_DEVICE, "Postprocess Audio Device", "PostADevice", 0, 1000, "no label", "no label"},
	{JVX_SEQUENCER_QUEUE_TYPE_LEAVE, JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS, JVX_COMPONENT_AUDIO_NODE, "Postprocess Audio Node", "PostANode", 0, 1000, "no label", "no label"}
};

#endif