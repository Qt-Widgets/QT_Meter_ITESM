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

    mArc = mAirspeedGauge->addArc(65);
    mArc->setColor( Qt::white );

    DegreesArc= mAirspeedGauge->addDegrees(75);
    clrBand = mAirspeedGauge->addColorBand(70);
    mValues = mAirspeedGauge->addValues(90);
    unit = mAirspeedGauge->addLabel(70);
    mLab = mAirspeedGauge->addLabel(40);
    mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    mAirspeedNeedle->setLabel(mLab);
    mAirspeedNeedle->setColor(Qt::red);

    // default to speed
    configureSpeedGage();

    /* speed gage */
/*
    QcArcItem *arc = mAirspeedGauge->addArc(65);
    arc->setColor( Qt::white );
    DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,200);
    DegreesArc->setColor( Qt::white);
    clrBand = mAirspeedGauge->addColorBand(70);
    clrBand->setValueRange(0,200);
    QcValuesItem *values = mAirspeedGauge->addValues(90);
    values->setValueRange(0,200);
    values->setColor( Qt::white );
    values->setStep( 20 );
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
*/


/* tacho */
/*
    QcArcItem *arc = mAirspeedGauge->addArc(65);
    arc->setColor( Qt::white );

    DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,8000 );
    DegreesArc->setColor( Qt::white);
    DegreesArc->setStep( 200 );

    clrBand = mAirspeedGauge->addColorBand(70);

    clrBand->setValueRange(0, 200 );

    clrBand->setColorsSegments( 0, 180, 200  );

    QcValuesItem *values = mAirspeedGauge->addValues(90);
    values->setValueRange(0,8000);
    values->setStep( 1000 );
    values->setColor( Qt::white );

    unit = mAirspeedGauge->addLabel(70);
    unit->setText( "RPM" );
    unit->setColor( Qt::white );
    QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    lab->setText("0");
    lab->setColor( Qt::white );
    mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    mAirspeedNeedle->setLabel(lab);
    mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 8000);
*/

    /* fuel */
/*
    QcArcItem *arc = mAirspeedGauge->addArc(65);
    arc->setColor( Qt::white );
    arc->setDgereeRange( 0,  90 );


    DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,100);
    DegreesArc->setStep( 25 );
    DegreesArc->setColor( Qt::white);
    DegreesArc->setDgereeRange( 0, 90 );

    clrBand = mAirspeedGauge->addColorBand(70);
    clrBand->setValueRange(0,100);
    clrBand->setDgereeRange( 0, 45 );

    QcValuesItem *values = mAirspeedGauge->addValues(90);
    values->setValueRange(0,100);
    values->setColor( Qt::white );
    values->setDgereeRange( 0, 90 );
    values->setStep( 25 );

    unit = mAirspeedGauge->addLabel(70);
    unit->setText( "lts" );
    unit->setColor( Qt::white );

    QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    lab->setText("0");
    lab->setColor( Qt::white );

    mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    mAirspeedNeedle->setLabel(lab);
    mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 100 );
    mAirspeedNeedle->setDgereeRange( 0, 90 );
*/




    mAirspeedGauge->addBackground(7);
    ui->horizontalLayout->addWidget(mAirspeedGauge);

    skt = new ClientConnection();
    connect( skt, SIGNAL(newData(qint8, qint16, qint8 )), this, SLOT( setValue(qint8, qint16, qint8 ))  );

    skt->connectToServer();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureSpeedGage( void )
{
    //QcArcItem *arc = mAirspeedGauge->addArc(65);
    //DegreesArc= mAirspeedGauge->addDegrees(75);
    //clrBand = mAirspeedGauge->addColorBand(70);
    //QcValuesItem *values = mAirspeedGauge->addValues(90);
    //unit = mAirspeedGauge->addLabel(70);
    //QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    //mAirspeedNeedle = mAirspeedGauge->addNeedle(75);





    //QcArcItem *arc = mAirspeedGauge->addArc(65);
    //arc->setColor( Qt::white );

    //DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,200);
    DegreesArc->setColor( Qt::white);
    //clrBand = mAirspeedGauge->addColorBand(70);
    clrBand->setValueRange(0,200);
    //QcValuesItem *values = mAirspeedGauge->addValues(90);
    mValues->setValueRange(0,200);
    mValues->setColor( Qt::white );
    mValues->setStep( 20 );
    //unit = mAirspeedGauge->addLabel(70);
    unit->setText( "Km/h" );
    unit->setColor( Qt::white );
    //QcLabelItem *lab = mAirspeedGauge->addLabel(40);

    mLab->setText("0");
    mLab->setColor( Qt::white );
    //mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    //mAirspeedNeedle->setLabel(mLab);
    //mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 200);

}

void MainWindow::configureTachoGage( void )
{
    //QcArcItem *arc = mAirspeedGauge->addArc(65);
    //arc->setColor( Qt::white );

    //DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,8000 );
    DegreesArc->setColor( Qt::white);
    DegreesArc->setStep( 200 );

    //clrBand = mAirspeedGauge->addColorBand(70);

    clrBand->setValueRange(0, 200 );

    clrBand->setColorsSegments( 0, 180, 200  );

    //QcValuesItem *values = mAirspeedGauge->addValues(90);
    mValues->setValueRange(0,8000);
    mValues->setStep( 1000 );
    mValues->setColor( Qt::white );

    //unit = mAirspeedGauge->addLabel(70);
    unit->setText( "RPM" );
    unit->setColor( Qt::white );
    //QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    mLab->setText("0");
    mLab->setColor( Qt::white );

    //mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    //mAirspeedNeedle->setLabel(mLab);
    //mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 8000);

}


void MainWindow::configureFuelGage( void )
{

    //QcArcItem *arc = mAirspeedGauge->addArc(65);
    //arc->setColor( Qt::white );
    mArc->setDgereeRange( 0,  90 );


    //DegreesArc= mAirspeedGauge->addDegrees(75);
    DegreesArc->setValueRange(0,100);
    DegreesArc->setStep( 25 );
    DegreesArc->setColor( Qt::white);
    DegreesArc->setDgereeRange( 0, 90 );

    //clrBand = mAirspeedGauge->addColorBand(70);
    clrBand->setValueRange(0,100);
    clrBand->setDgereeRange( 0, 45 );

    //QcValuesItem *values = mAirspeedGauge->addValues(90);

    mValues->setValueRange(0,100);
    mValues->setColor( Qt::white );
    mValues->setDgereeRange( 0, 90 );
    mValues->setStep( 25 );

    //unit = mAirspeedGauge->addLabel(70);
    unit->setText( "lts" );
    unit->setColor( Qt::white );

    //QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    mLab->setText("0");
    mLab->setColor( Qt::white );

    //mAirspeedNeedle = mAirspeedGauge->addNeedle(75);
    //mAirspeedNeedle->setLabel(mLab);
    //mAirspeedNeedle->setColor(Qt::red);
    mAirspeedNeedle->setValueRange(0, 100 );
    mAirspeedNeedle->setDgereeRange( 0, 90 );

}


void MainWindow::setValue(qint8 id, qint16 value, qint8 unitSel )
{

    switch( id )
    {

        case  ID_GAGE_SPEED:
        // set value


        if( type != SPEED )
        {
            configureSpeedGage();
            type = SPEED;
        }

        mAirspeedNeedle->setCurrentValue( value );

        switch ( unitSel )
        {
        // input 0 - 2000
            case KPH:
                unit->setText("Km/h");
                DegreesArc->setStep( 10 );
            break;
            case MPH:
                unit->setText("Mi/h");
                DegreesArc->setStep( 10 );
                break;
        }
        break;


        case ID_GAGE_TACHO:

        if( type != TACHO )
        {
            configureTachoGage();
            type = TACHO;
        }

        //input 0 - 8000
            mAirspeedNeedle->setCurrentValue( value );
        break;

        case ID_GAGE_FUEL:

        // temporary setup, maybe change look and feel later
        // marks range 20 - 90 degrees ->  0% 25% 50% 75% 100%

            if( type != FUEL )
            {
                configureFuelGage();
                type = FUEL;
            }

            mAirspeedNeedle->setCurrentValue( value );


            switch ( unitSel )
            {
            // input 0 - 1000
                case LTS:
                    unit->setText("lts");
                break;

                case GAL:
                    unit->setText("Gal");
                break;

            }

            break;

        default:

            break;

    }
}
