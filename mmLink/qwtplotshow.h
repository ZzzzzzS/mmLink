/****************************************************************************
MIT License

Copyright (c) 2020 ZhouZishun

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#ifndef QWTPLOTSHOW_H
#define QWTPLOTSHOW_H

#include <QObject>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <QPointF>

class QwtPlotShow : public QObject
{
    Q_OBJECT
public:
    explicit QwtPlotShow(QwtPlot *Plot,QObject *parent = nullptr);
    ~QwtPlotShow();

    QwtPlotGrid *Grid; //qwt的网格
    QwtPlotCurve *Curve;//qwt画笔

public slots:
    void addNewDataSlot(QVector<double> &x,QVector<double> &data,bool AutoScale=true);
    void ClearSlot();
signals:

private:
    QwtPlotMagnifier *Magnifer;
    QwtPlotPanner *Panner;
    QwtPlot *CurrentPlot;

};

#endif // QWTPLOTSHOW_H
