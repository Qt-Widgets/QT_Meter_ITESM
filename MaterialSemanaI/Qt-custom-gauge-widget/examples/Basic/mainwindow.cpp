/***************************************************************************
**                                                                        **
**  QcGauge, for instrumentation, and real time data measurement          **
**  visualization widget for Qt.                                          **
**  Copyright (C) 2015 Hadj Tahar Berrima                                 **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU Lesser General Public License as        **
**  published by the Free Software Foundation, either version 3 of the    **
**  License, or (at your option) any later version.                       **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU Lesser General Public License for more details.                   **
**                                                                        **
**  You should have received a copy of the GNU Lesser General Public      **
**  License along with this program.                                      **
**  If not, see http://www.gnu.org/licenses/.                             **
**                                                                        **
****************************************************************************
**           Author:  Hadj Tahar Berrima                                  **
**           Website: http://pytricity.com/                               **
**           Contact: berrima_tahar@yahoo.com                             **
**           Date:    1 dec 2014                                          **
**           Version:  1.0                                                **
****************************************************************************/

#include <mainwindow.h>
#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet( "background-color: black; ");
    mAirspeedGauge = new QcGaugeWidget;
    QcArcItem *arc = mAirspeedGauge->addArc(62);
    arc->setColor( Qt::white );
    QcDegreesItem *item= mAirspeedGauge->addDegrees(70);
    item->setValueRange(0,200);
    item->setColor( Qt::white);
    clrBand = mAirspeedGauge->addColorBand(65);
    clrBand->setValueRange(0,200);
    QcValuesItem *values = mAirspeedGauge->addValues(80);
    values->setValueRange(0,200);
    values->setColor( Qt::white );
    unit = mAirspeedGauge->addLabel(70);
    unit->setText( "Km/h" );
    unit->setColor( Qt::white );
    QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    lab->setText("0");
    lab->setColor( Qt::white );
    mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    mAirspeedNeedle->setLabel(lab);
    mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 200);
    mAirspeedGauge->addBackground(7);
    ui->horizontalLayout->addWidget(mAirspeedGauge);
    skt = new SocketTest();
    connect( skt, SIGNAL(newData(float,int)), this, SLOT( setValue(float,int))  );
    skt->OpenSocket( 1020 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

#define KPH 0
#define MPH 1
void MainWindow::setValue( float value, int unitSel )
{
    mAirspeedNeedle->setCurrentValue( value );
    switch ( unitSel )
    {
        case MPH:
            unit->setText("Mi/h");
            clrBand->setValueRange(0,160);
            clrBand->setColorsSegments( 6, 75, 160 );
            break;
        case KPH:
        default:
            unit->setText("Km/h");
            clrBand->setValueRange(0,200);
            clrBand->setColorsSegments( 10, 120, 200 );
            break;
    }
}
