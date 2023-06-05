#include "pcie_image_qt.h"
#include "ui_mainwindow.h"
#include "pcie_func.h"
#include <QFile>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
static unsigned int pcie_trans_size[]=
{
    1024,
    4096,
    8192,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576,
    0x400000,
    0x800000
};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(pcie_init()<0)
	{
		QMessageBox::information(this,"ERROR","PCIe init error");
	}
    isc2h_start = false;
    ish2c_start = false;
	isinit = false;
	isplay = false;
	saturation = 128;
	issatu_incr = true;
    c2h_transfer_size = pcie_trans_size[ui->comboBox_c2h->currentIndex()];
	h2c_transfer_size = pcie_trans_size[ui->comboBox_h2c->currentIndex()];
    ui->pushButton_sel_dir->setDisabled(true);
    ui->pushButton_ureg_get->setDisabled(true);
    ui->pushButton_ureg_set->setDisabled(true);
	ui->lineEdit_reso->setText("wait for pic");
    m_QTimer_c2h = new QTimer(this);
    m_QTimer_h2c = new QTimer(this);
    connect(m_QTimer_c2h,SIGNAL(timeout()),this,SLOT(c2h_transfer_one_time()));
    connect(m_QTimer_h2c,SIGNAL(timeout()),this,SLOT(h2c_transfer_one_time()));
	connect(&pcie_c2h_event,SIGNAL(sig_inform_disp()),this,SLOT(disp_slot()));
//	set_saturation(saturation);
    image_width = 1920;
    image_height = 1080;
}

MainWindow::~MainWindow()
{
	if(pcie_c2h_event.isRunning())
	{
		pcie_c2h_event.terminate();
        pcie_c2h_event.wait();
	}
	disp_deinit();
	pcie_deinit();
	
    delete ui;
}
void MainWindow::c2h_transfer_one_time()
{
	unsigned int bd=0;
    bd = c2h_transfer(c2h_transfer_size);
    QString str = QString("%1").arg(bd);
    str += "MB/s";
    ui->lineEdit_c2h->setText(str);
}
void MainWindow::h2c_transfer_one_time()
{
    //transfer h2c
    unsigned int bd=0;
    bd = h2c_transfer(h2c_transfer_size);
    QString str = QString("%1").arg(bd);
    str += "MB/s";
    ui->lineEdit_h2c->setText(str);
}
void MainWindow::combo_box_c2h_slot(int index)
{
    c2h_transfer_size = pcie_trans_size[index];
}

void MainWindow::combo_box_h2c_slot(int index)
{
    h2c_transfer_size = pcie_trans_size[index];
}

void MainWindow::push_button_c2h_start_slot()
{
    if(false==isc2h_start)
    {
        isc2h_start = true;
        m_QTimer_c2h->start(100);
        ui->pushButton_c2h_start->setText("STOP");
    }
    else
    {
        isc2h_start = false;
        m_QTimer_c2h->stop();
        ui->pushButton_c2h_start->setText("START");
    }
}

void MainWindow::push_button_h2c_start_slot()
{
    if(false==ish2c_start)
    {
        ish2c_start = true;
        m_QTimer_h2c->start(100);
        ui->pushButton_h2c_start->setText("STOP");
    }
    else
    {
        ish2c_start = false;
        m_QTimer_h2c->stop();
        ui->pushButton_h2c_start->setText("START");
    }
}

void MainWindow::push_button_ureg_get_slot()
{

}

void MainWindow::push_button_ureg_set_slot()
{

}

void MainWindow::check_box_repeat_slot(bool)
{

}

void MainWindow::push_button_sel_dir_slot()
{
    QString filename;
    char * name;
    filename = QFileDialog::getOpenFileName(this,tr("Select Pic"),"",tr("Images (*.bmp)"));
    if(filename.isEmpty())
    {
        return;
    }
    QByteArray ba = filename.toLatin1();
    name=ba.data();
    if(true == isinit)
    {
        disp_deinit();
        delete m_Qimage;
        isinit = false;
    }

    if(disp_init(&image_width,&image_height,name) < 0)
    {
        //
    }
    isinit = true;
    QString str1 = QString("%1").arg(image_width);
    QString str2 = QString("%1").arg(image_height);
    str1 = str1 + "X" + str2;
    ui->lineEdit_reso->setText(str1);
    posix_memalign((void **)&buffer, 4096/*alignment*/,  (size_t)(image_width*image_height*3));
    m_Qimage = new QImage(buffer,image_width,image_height,QImage::Format_RGB888);
}

void MainWindow::push_button_play_slot()
{
	if(false == isplay)
	{
		if(!pcie_c2h_event.isRunning())
			pcie_c2h_event.start();
		disp_start();
		ui->pushButton_play->setText("STOP");
		ui->pushButton_sel_dir->setDisabled(true);
		isplay = true;
	}
	else
	{
		disp_reset();
		ui->pushButton_play->setText("PLAY");
//		ui->pushButton_sel_dir->setDisabled(false);
        ui->pushButton_sel_dir->setDisabled(true);
		isplay = false;
	}
}
void MainWindow::disp_slot()
{
    int fps;
    if(isplay == false)
        return;

    unsigned char *buffer = new unsigned char;
    QImage *m_Qimage = new QImage;
    posix_memalign((void **)&buffer, 4096/*alignment*/,  (size_t)(image_width*image_height*3));
    m_Qimage = new QImage(buffer,image_width,image_height,QImage::Format_RGB888);

    fps = pcie_read_frame(buffer);
    QString str1 = QString("%1").arg(fps);
    QString str2 = QString("%1").arg(saturation);
    str1+= "fps  satu=";
    str1+= str2;
    ui->lineEdit_satu->setText(str1);

//    set_saturation(saturation);
//    if(0xff == saturation)
//        issatu_incr = false;
//    if(0x00 == saturation)
//        issatu_incr = true;
//    saturation += issatu_incr ? 1 : -1;

    cv::Mat mat;
    mat = cv::Mat(m_Qimage->height(), m_Qimage->width(), CV_8UC3, (void*)m_Qimage->constBits(), m_Qimage->bytesPerLine());
//    cv::cvtColor(mat, mat, CV_BGR2RGB);
    QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
    *m_Qimage = image.rgbSwapped();

    m_Qpixmap=QPixmap::fromImage(*m_Qimage,Qt::AutoColor);
    m_Qpixmap=m_Qpixmap.scaled(ui->label->width()/8*8,ui->label->height()/8*8,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->label->setPixmap(m_Qpixmap);
//    ui->label->setPixmap(QPixmap::fromImage(*m_Qimage).scaled(ui->label->size()));
    delete buffer;
    delete m_Qimage;
}
