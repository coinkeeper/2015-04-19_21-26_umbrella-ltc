#include "splashscreen.h"
#include "clientversion.h"
#include "util.h"

#include <QPainter>
#undef loop /* ugh, remove this when the #define loop is gone from util.h */
#include <QApplication>

SplashScreen::SplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap, f)
{
    // set reference point, paddings
    int paddingLeftCol2         = 170;
    int paddingTopCol2          = 306;
    
    int line1 = 0;
    int line2 = 13 - 3;
    int line3 = 26 - 3;
    int line4 = 39 - 3;
    int line5 = 52 - 3;
    int line6 = 65 - 3;

    float fontFactor            = 1.0;

    // define text to place
    QString titleText       = QString(QApplication::applicationName()).replace(QString("-testnet"), QString(""), Qt::CaseSensitive); // cut of testnet, place it as single object further down
    QString versionText     = QString("Version %1 ").arg(QString::fromStdString(FormatFullVersion()));
    QString copyrightText1   = QChar(0xA9)+QString(" 2009-2013 ") + QString(tr("The Bitcoin developers"));
    QString copyrightText2   = QChar(0xA9)+QString(" 2011-2013 ") + QString(tr("The Litecoin developers"));
    QString copyrightText3   = QChar(0xA9)+QString(" 2013-2013 ") + QString(tr("Dr. Kimoto Chan"));
    QString copyrightText4   = QChar(0xA9)+QString(" 2013-2014 ") + QString(tr("Cohors LLC"));
    
    QString font            = "Arial";

    // load the bitmap for writing some text over it
    QPixmap newPixmap;
    if(GetBoolArg("-testnet")) {
        newPixmap     = QPixmap(":/images/splash_testnet");
    }
    else {
        newPixmap     = QPixmap(":/images/splash");
    }

    QPainter pixPaint(&newPixmap);
    pixPaint.setPen(QColor(100,100,100));

    pixPaint.setFont(QFont(font, 9*fontFactor));
    pixPaint.drawText(paddingLeftCol2,paddingTopCol2+line6,versionText);

    // draw copyright stuff
      pixPaint.setFont(QFont(font, 9*fontFactor));
      pixPaint.drawText(paddingLeftCol2,paddingTopCol2+line1,copyrightText1);
      pixPaint.drawText(paddingLeftCol2,paddingTopCol2+line2,copyrightText2);
      pixPaint.drawText(paddingLeftCol2,paddingTopCol2+line3,copyrightText3);
      pixPaint.drawText(paddingLeftCol2,paddingTopCol2+line4,copyrightText4);
      
    pixPaint.end();

    this->setPixmap(newPixmap);
}
