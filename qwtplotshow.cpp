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
    Plot->enableAxis(QwtPlot::yLeft,false);
    Plot->enableAxis(QwtPlot::yRight,true);
    Plot->setAxisAutoScale(QwtPlot::yLeft,true);
    Plot->setAxisAutoScale(QwtPlot::xBottom,true);

    this->Curve->attach(Plot);
    this->Curve->setStyle(QwtPlotCurve::Lines);
    //this->Curve->setCurveAttribute(QwtPlotCurve::Fitted,true);//是曲线更光滑
    this->Curve->setPen(Qt::blue);

    this->Grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    this->Grid->setMinorPen(Qt::gray,0,Qt::DotLine);
    this->Grid->enableX(true);
    this->Grid->enableY(true);
    this->Grid->enableYMin(true);
    this->Grid->enableXMin(false);
    this->Grid->attach(Plot);

    //this->CurrentPlot->setAxisScale(QwtPlot::xBottom,0,100);
    //this->CurrentPlot->setAxisScale(QwtPlot::yRight,0,1000);
}

QwtPlotShow::~QwtPlotShow()
{

}

void QwtPlotShow::addNewDataSlot(QVector<double>  &x,QVector<double> &data)
{
    this->Curve->setSamples(x,data);
    /*if(x.last()>=100)
    {
        this->CurrentPlot->setAxisScale(QwtPlot::xBottom,x.last()-100,x.last());
    }*/
    this->CurrentPlot->replot();
}

void QwtPlotShow::ClearSlot()
{
    QVector<QPointF> value;
    value.clear();
    this->Curve->setSamples(value);
    this->CurrentPlot->setAxisScale(QwtPlot::xBottom,0,100);
    this->CurrentPlot->setAxisScale(QwtPlot::yRight,0,1000);
    this->CurrentPlot->replot();
}
