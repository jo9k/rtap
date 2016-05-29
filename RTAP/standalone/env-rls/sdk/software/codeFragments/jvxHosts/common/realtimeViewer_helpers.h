#ifndef __REALTIMEVIEWER_HELPERS_H__
#define __REALTIMEVIEWER_HELPERS_H__

#include "jvx.h"

#include "templates/common/CjvxRealtimeViewer.h"

#include "realtimeViewer_typedefs.h"

class QLineEdit_ext;
class QComboBox_ext;
class QSlider_ext;
class QwtPlotCurve;

#if defined (QT_NAMESPACE)
namespace QT_NAMESPACE {
#endif

class QLabel;
class QPushButton;
class QCheckBox;

#if defined (QT_NAMESPACE)
}
#endif

typedef struct
{
	jvxPropertyCategoryType category;
	jvxUInt64 allowStateMask;
	jvxUInt64 allowThreadingMask;
	jvxDataFormat format;
	jvxSize num;
	jvxPropertyAccessType accessType;
	jvxPropertyDecoderHintType decHtTp;
	jvxSize inPropIdx;
	jvxSize hdlIdx;
	std::string propDescription;
	std::string objDescription;
	jvxComponentType tp;
} jvxRealtimeViewerPropertyDescriptor;

typedef struct
{
	jvxRealtimeViewerPropertyDescriptor descriptory;
	jvxRealtimeViewerPropertyDescriptor descriptorx;
	jvxInt16 color;
	jvxInt16 linestyle;
	jvxInt16 show;
	jvxInt16 width;
	jvxInt16 idPropX;
	QwtPlotCurve* thePlotCurve;
	jvxData* dataX;
	jvxData* dataY;
	jvxSize numValues;

	jvxData maxX;
	jvxData maxY;
	jvxData minX;
	jvxData minY;
	jvxBool minmaxValid;
} jvxRealtimeViewerPropertyPlotItem;

typedef struct
{
	jvxRealtimeViewerPropertyDescriptor descriptor;

	jvxInt32 length;
	jvxInt16 num_chans;
	jvxBitField selection;

	jvxSize numElmsCircBuffer;
	jvxSize szCircBuffer;
	oneMultichannelCircBufferHeader* ptrCircBuffer;
	jvxBool isValid;
	QwtPlotCurve** thePlotCurves;
	jvxData* dataInc;
	jvxData** dataCpy;

	jvxInt16 lineWidth;
	jvxInt16 idChannelX;
	JVX_MUTEX_HANDLE* safeAccessHdl;
	jvxData maxX;
	jvxData maxY;
	jvxData minX;
	jvxData minY;
	jvxBool minmaxValid;
	jvxBool expectRescaleX;

} jvxRealtimeViewerPropertyCircPlotItem;

typedef struct
{
	jvxRealtimeViewerPropertyDescriptor descriptor;
	jvxData ymax;
	jvxData xmax;
	jvxData ymin;
	jvxData xmin;
	jvxInt16 autox;
	jvxInt16 autoy;
	std::string xaxistxt;
	std::string yaxistxt;
} jvxRealtimeViewerPropertyPlotGroup;

typedef struct
{
	jvxRealtimeViewerPropertyDescriptor descriptor;
	bool setup_complete;
	QLineEdit_ext* line_edit;
	QComboBox_ext* combo_box;
	QSlider_ext* slider;

#if defined (QT_NAMESPACE)
	std::vector<QT_NAMESPACE::QLabel*> label_flags;
QT_NAMESPACE::QLabel* label_min;
	QT_NAMESPACE::QLabel* label_max;
	QT_NAMESPACE::QLabel* label_valid;
	QT_NAMESPACE::QPushButton* button_1;
	QT_NAMESPACE::QPushButton* button_2;
	QT_NAMESPACE::QPushButton* button_3;
	QT_NAMESPACE::QPushButton* button_4;
	//QT_NAMESPACE::QCheckBox* checkbox_1;
#else
	std::vector<QLabel*> label_flags;
	QLabel* label_min;
	QLabel* label_max;
	QLabel* label_valid;
	QPushButton* button_1;
	QPushButton* button_2;
	QPushButton* button_3;
	QPushButton* button_4;
	//QCheckBox* checkbox_1;
#endif
	jvxInt16 selection;
} jvxRealtimeViewerPropertyItem;

jvxErrorType static_putConfiguration(IjvxConfigProcessor* theReader, jvxConfigData* readEntriesFrom, jvxHandle* privateDataItem, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeId, jvxInt16 itemId);
jvxErrorType static_getConfiguration(IjvxConfigProcessor* theReader, jvxConfigData* writeEntriesTo, jvxHandle* privateDataItem, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeId, jvxInt16 itemId);

jvxErrorType static_initData(jvxHandle* privateData, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeHint);
jvxErrorType static_allocateData(jvxHandle** privateData, jvxSize* sz, jvxRealtimePrivateMemoryDecoderEnum decodeHint);
jvxErrorType static_copyData(jvxHandle* copyTo, jvxHandle* copyFrom, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeId);
jvxErrorType static_deallocateData(jvxHandle* removeMyContent, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeId);

#endif

