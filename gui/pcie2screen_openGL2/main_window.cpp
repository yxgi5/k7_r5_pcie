#include "main_window.h"
#include "xdma_programe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    QVBoxLayout *pVBox = new QVBoxLayout(centralWidget());
    pVBox->setMargin(0);
    pVBox->setSpacing(0);
    titleName = tr("丰柯电子(FIGKEY)");
    setWindowTitle(titleName);

    pOpenGLFrame = new uOpenglYuv(this);
    pVBox->addWidget(pOpenGLFrame);

    pPlayState = new QLabel(this);
    pPlayState->setStyleSheet("QLabel{"
                              "background-color:rgba(0, 0, 0, 160);"
                              "border-style: outset;"
                              "border-width: 2px;"
                              "border-radius: 5px;"
                              "border-color: #8B7355;"
                              "font: bold 28px;"
                              "min-width:200px;max-width:200px;"
                              "min-height:40px;max-height:40px;"
                              "color:white;"
                              "font-family:华文新魏;"
                              "}");
    pPlayState->setAlignment(Qt::AlignCenter);
    QHBoxLayout *pHBox = new QHBoxLayout(pOpenGLFrame);
    pHBox->addWidget(pPlayState);

    //QObject::connect(pPlayState, SIGNAL(clicked(bool)), this, SLOT(startCapture()));

    xdma_programe *pXdma = new xdma_programe;
    if(pXdma->getDevice())
    {
        pXdma->c2h_event_fd = pXdma->open_event("/dev/xdma0_events_0");
        qDebug("c2h_event_fd=%x",pXdma->c2h_event_fd);
        pXdma->control_fd = pXdma->open_control("/dev/xdma0_user");
        qDebug("control_fd=%x",pXdma->control_fd);
        pXdma->control_base = pXdma->mmap_control(pXdma->control_fd,MAP_SIZE);
        qDebug("control_base=%x",pXdma->control_base);
        pXdma->ddr_frm_addr=0x10000000;
        pXdmaGetImg = new xdma_getImg(pXdma, (char *)pOpenGLFrame->pRGB, &pOpenGLFrame->optnotice);
        pPlayState->setText(tr("播放暂停中"));
        QObject::connect(pXdmaGetImg, SIGNAL(flushImg()), pOpenGLFrame, SLOT(update()));
    }
    else
    {
        pXdmaGetImg = NULL;
        pPlayState->setText(tr("设备未就绪"));
    }
    pTimer = new QTimer;
    QObject::connect(pOpenGLFrame, SIGNAL(mouseDoubleClickNotice(int)), this, SLOT(changeWindows(int)));
    QObject::connect(pOpenGLFrame, SIGNAL(flushFps(int)), this, SLOT(flushFps(int)));
    QObject::connect(pTimer, SIGNAL(timeout()), this, SLOT(timeDelay()));
    resize(640, 360);
    isPressed = false;
    isRelease = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::startCapture()
{
    if(!pXdmaGetImg)
    {
        return;
    }
    if(pPlayState->isVisible())
    {
        pPlayState->hide();
        pXdmaGetImg->setstart(true);
    }
    else
    {
        pPlayState->show();
        setWindowTitle(titleName);
        pXdmaGetImg->setstart(false);
    }
}

void MainWindow::changeWindows(int index)
{
    switch(index)
    {
    case 0:
        isPressed = true;
        break;
    case 1:
        pTimer->start(400);
        isRelease = true;
        break;
    case 2:
        if(isFullScreen())
        {
            showNormal();
            QCursor cursor(Qt::ArrowCursor);
            QApplication::setOverrideCursor(cursor);
            QApplication::changeOverrideCursor(cursor);
        }
        else
        {
            showFullScreen();
            QCursor cursor(Qt::BlankCursor);
            QApplication::setOverrideCursor(cursor);
            QApplication::changeOverrideCursor(cursor);
        }
        isPressed = false;
        isRelease = false;
        break;
    case 3:
        isPressed = false;
        isRelease = false;
        break;
    }
}

void MainWindow::timeDelay()
{
    if(isPressed && isRelease)
    {
        startCapture();
    }
    isPressed = false;
    isRelease = false;
    pTimer->stop();
}

void MainWindow::flushFps(int speed)
{
    QString str;

    str.sprintf("    fps:%02d.%02d", speed/100, speed%100);
    setWindowTitle(titleName + str);
}
