#include "pcie_manage.h"

pcie_manage::pcie_manage(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(""
                  "*{font-size:16px;color:#000;font-family:微软雅黑;}"
                 );
    QGridLayout *pLayout = new QGridLayout(this);

    pLabOpt = new QLabel;
    pLabAddr = new QLabel;
    pLabLength = new QLabel;
    pComboBox = new QComboBox;
    pLineEditAddr = new QLineEdit;
    pLineEditLength = new QLineEdit;
    pButRead = new QPushButton;
    pButWrite = new QPushButton;
    pTextEdit = new QTextEdit;

    pLabOpt->setText(tr("测试模式"));
    pLabAddr->setText(tr("偏移地址"));
    pLabLength->setText(tr("读数据长度"));
    pComboBox->setItemDelegate(new QStyledItemDelegate());
    pComboBox->addItem(tr("控制模式"));
    pComboBox->addItem(tr("用户模式"));
    pComboBox->addItem(tr("内存模式"));
    pButRead->setText(tr("读"));
    pButWrite->setText(tr("写"));

    QLabel *pLabWarning = new QLabel;
    pLabWarning->setText(tr("地址或数据错误<br>可能造成崩溃<br>请注意！！"));
    pLayout->setColumnStretch(1, 1);
    pLayout->addWidget(pLabWarning,     0, 0, 3, 1);
    pLayout->addWidget(pLabOpt,         0, 2);
    pLayout->addWidget(pComboBox,       0, 3);
    pLayout->addWidget(pLabAddr,        1, 2);
    pLayout->addWidget(pLineEditAddr,   1, 3);
    pLayout->addWidget(pButWrite,       1, 4);
    pLayout->addWidget(pLabLength,      2, 2);
    pLayout->addWidget(pLineEditLength, 2, 3);
    pLayout->addWidget(pButRead,        2, 4);
    pLayout->addWidget(pTextEdit,       3, 0, 1, 5);
}

int getHexFromText(char *pParseData, int parseLen, unsigned char *pBackData, int maxlen)
{
    int i;
    int cot=0;
    int sub=0;
    unsigned char d0;
    unsigned char d1;

    for(i=0;i<parseLen;i++)
    {
        if(pParseData[i] == ' ')
        {
            sub=0;
        }
        else
        {
            d1 = pParseData[i];
            if( (d1>='0') && (d1<='9') )
            {
                d1 -= '0';
            }
            else if( (d1>='a') && (d1<='f') )
            {
                d1 = d1-'a'+10;
            }
            else if( (d1>='A') && (d1<='F') )
            {
                d1 = d1-'A'+10;
            }
            else
            {
                continue;
            }
            if(sub)
            {
                d0 |= d1;
                pBackData[cot++] = d0;
                if(cot >= maxlen)
                {
                    break;
                }
                sub = 0;
            }
            else
            {
                sub = 1;
                d0 = d1<<4;
            }
        }
    }
    return cot;
}

void getTextFromHex(unsigned char *pData, int len, QString &str)
{
    char buff[10];

    str.resize(len*3);
    str = "";
    for(int i=0;i<len;i++)
    {
        sprintf(buff, "%02X", pData[i]);
        if(i)
        {
            str += " ";
        }
        str += buff;
    }
}
