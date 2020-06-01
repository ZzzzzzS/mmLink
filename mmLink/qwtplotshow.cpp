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

#include "qwtplotshow.h"

QwtPlotShow::QwtPlotShow(QwtPlot *Plot,QObject *parent) : QObject(parent)
{
    this->Grid=new QwtPlotGrid();
    this->Curve=new QwtPlotCurve();
    this->Magnifer=new QwtPlotMagnifier(Plot->canvas());//设置鼠标滚动
    this->Panner=new QwtPlotPanner(Plot->canvas());
    this->CurrentPlot=Plot;

    Plot->enableAxis(QwtPlot::xTop,false);
    Plot->enableAxis(QwtPlot::xBottom,true);
    Plot->enableAxis(QwtPlot::yLeft,true);
    Plot->enableAxis(QwtPlot::yRight,false);
    Plot->setAxisAutoScale(QwtPlot::yLeft,true);
    Plot->setAxisAutoScale(QwtPlot::xBottom,true);

    this->Curve->attach(Plot);
    this->Curve->setStyle(QwtPlotCurve::Lines);
    this->Curve->setPen(Qt::blue);

    this->Grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    this->Grid->setMinorPen(Qt::gray,0,Qt::DotLine);
    this->Grid->enableX(true);
    this->Grid->enableY(true);
    this->Grid->enableYMin(false);
    this->Grid->enableXMin(false);
    this->Grid->attach(Plot);
}

QwtPlotShow::~QwtPlotShow()
{

}

void QwtPlotShow::addNewDataSlot(QVector<double>  &x, QVector<double> &data, bool AutoScale)
{
    this->Curve->setSamples(x,data);
    if(AutoScale==true)
    {
        this->CurrentPlot->setAxisAutoScale(QwtPlot::yLeft,true);
        this->CurrentPlot->setAxisAutoScale(QwtPlot::xBottom,true);
        this->CurrentPlot->updateAxes();
    }
    this->CurrentPlot->replot();

}

void QwtPlotShow::ClearSlot()
{
    QVector<QPointF> value;
    value.clear();
    this->Curve->setSamples(value);
    this->CurrentPlot->setAxisScale(QwtPlot::xBottom,0,100);
    this->CurrentPlot->setAxisScale(QwtPlot::yRight,0,100);
    this->CurrentPlot->replot();
}
