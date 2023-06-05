#ifndef PCIE_IMAGE_DEMO_H
#define PCIE_IMAGE_DEMO_H
#include <QThread>
#include <QMessageBox>
#include <QLineEdit>
#include <QSlider>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QMainWindow>
#include <QFile>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}


class event_thread : public QThread
{
    Q_OBJECT
public :
    event_thread();
    ~event_thread();
    void run();
	/*
public:
    int pcie_event_fd;
    void *pcie_ctrl_addr;
    void pcie_event_open(char *);
    void pcie_event_close( );
*/
signals :
    void sig_inform_disp();

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
            void combo_box_c2h_slot(int);
            void combo_box_h2c_slot(int);
            void push_button_c2h_start_slot();
            void push_button_h2c_start_slot();
            void push_button_ureg_get_slot();
            void push_button_ureg_set_slot();
            void check_box_repeat_slot(bool);
            void push_button_sel_dir_slot();
            void push_button_play_slot();
            void c2h_transfer_one_time();
            void h2c_transfer_one_time();
			void disp_slot();
public:
public:
    event_thread pcie_c2h_event;
    int image_width;
    int image_height;
    bool isc2h_start;
    bool ish2c_start;
    bool isplay;
    bool isinit;
	bool issatu_incr;
	uchar saturation;
    void *pcie_ctrl_base;
    int    pcie_ctrl_fd;
    int    pcie_dma_c2h_fd;
    unsigned char *buffer;
    unsigned int c2h_transfer_size;
    unsigned int h2c_transfer_size;
    QImage *m_Qimage;
    QPixmap m_Qpixmap;
    QTimer *m_QTimer_c2h;
    QTimer *m_QTimer_h2c;
private:
    Ui::MainWindow *ui;
};

#endif // MIZ7035_DEMO_H
