/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QwtPolar Widget Library
 * Copyright (C) 2008   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_POLAR_CURVE_H
#define QWT_POLAR_CURVE_H

#include "qwt_polar_global.h"
#include "qwt_polar_item.h"
#include <qwt_point_polar.h>
#include <qwt_series_data.h>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE
class QwtSymbol;
class QwtCurveFitter;

/*!
  \brief An item, that represents a series of points

  A curve is the representation of a series of points in polar coordinates.
  The points are connected to the curve using the abstract QwtData interface.

  \sa QwtPolarPlot, QwtSymbol, QwtScaleMap
*/

class QWT_POLAR_EXPORT QwtPolarCurve: public QwtPolarItem
{
public:
    /*!
        Curve styles.
        \sa setStyle(), style()
    */
    enum CurveStyle
    {
        //! Don't draw a curve. Note: This doesn't affect the symbols.
        NoCurve,

        /*!
          Connect the points with straight lines. The lines might
          be interpolated depending on the 'Fitted' attribute. Curve
          fitting can be configured using setCurveFitter().
         */
        Lines,

        //! Values > 100 are reserved for user specific curve styles
        UserCurve = 100
    };

    /*!
        \brief Attributes how to represent the curve on the legend

        If none of the flags is activated QwtPlotCurve tries to find
        a color representing the curve and paints a rectangle with it.
        In the default setting all attributes are off.

        \sa setLegendAttribute(), testLegendAttribute()
     */

    enum LegendAttribute
    {
        /*!
          If the curveStyle() is not NoCurve a line is painted with the
          curvePen().
         */
        LegendShowLine = 0x01,

        //! If the curve has a valid symbol it is painted.
        LegendShowSymbol = 0x02
    };

    //! Legend attributes
    typedef QFlags<LegendAttribute> LegendAttributes;


    explicit QwtPolarCurve();
    explicit QwtPolarCurve( const QwtText &title );
    explicit QwtPolarCurve( const QString &title );

    virtual ~QwtPolarCurve();

    virtual int rtti() const;

    void setLegendAttribute( LegendAttribute, bool on = true );
    bool testLegendAttribute( LegendAttribute ) const;

    void setData( QwtSeriesData<QwtPointPolar> *data );
    const QwtSeriesData<QwtPointPolar> *data() const;

    size_t dataSize() const;
    QwtPointPolar sample( int i ) const;

    void setPen( const QPen & );
    const QPen &pen() const;

    void setStyle( CurveStyle style );
    CurveStyle style() const;

    void setSymbol( QwtSymbol * );
    const QwtSymbol *symbol() const;

    void setCurveFitter( QwtCurveFitter * );
    QwtCurveFitter *curveFitter() const;

    virtual void draw( QPainter *p,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QPointF &pole, double radius,
        const QRectF &canvasRect ) const;

    virtual void draw( QPainter *p,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QPointF &pole, int from, int to ) const;

    virtual QwtInterval boundingInterval( int scaleId ) const;

    virtual QwtGraphic legendIcon( int index, const QSizeF & ) const;

protected:

    void init();

    virtual void drawCurve( QPainter *, int style,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QPointF &pole, int from, int to ) const;

    virtual void drawSymbols( QPainter *, const QwtSymbol &,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QPointF &pole, int from, int to ) const;

    void drawLines( QPainter *,
        const QwtScaleMap &azimuthMap, const QwtScaleMap &radialMap,
        const QPointF &pole, int from, int to ) const;

private:
    QwtSeriesData<QwtPointPolar> *d_series;

    class PrivateData;
    PrivateData *d_data;
};

//! \return the the curve data
inline const QwtSeriesData<QwtPointPolar> *QwtPolarCurve::data() const
{
    return d_series;
}

/*!
    \param i index
    \return point at position i
*/
inline QwtPointPolar QwtPolarCurve::sample( int i ) const
{
    return d_series->sample( i );
}

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPolarCurve::LegendAttributes )

#endif