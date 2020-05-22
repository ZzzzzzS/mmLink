#ifndef QWTPLOTSHOW_H
#define QWTPLOTSHOW_H

#include <QObject>
#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_grid.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_plot_item.h>
#include <Qwt/qwt_plot_magnifier.h>
#include <Qwt/qwt_plot_panner.h>
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
