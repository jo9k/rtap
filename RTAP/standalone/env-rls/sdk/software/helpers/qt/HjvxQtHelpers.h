#ifndef __HJVXQTHELPERS_H__
#define __HJVXQTHELPERS_H__

#include <QLabel>

static void setBackgroundLabelColor(QColor col, QLabel* lab)
{
	QPalette pal;
	lab->setAutoFillBackground(true);
	pal = lab->palette();
    QBrush brush(col);
    brush.setStyle(Qt::SolidPattern);
    pal.setBrush(QPalette::All, QPalette::Window, brush);
    lab->setPalette(pal);
}

#endif
