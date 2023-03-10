#include "../lib/music_playwidget.h"
#include <QFile>
#include <thread>

using namespace std;
const QString ApiOfGetLyricById = "http://localhost:3000/lyric?id=%1";


music_play::music_play(QWidget *parent) :
    QWidget(parent)
{
    this->hide();
    this->setObjectName("music_playWidget");
    this->setAttribute(Qt::WA_StyledBackground);

    music_ScrollArea = new QScrollArea();
    music_ScrollArea->setObjectName("main_ScrollArea");
    music_ScrollArea->installEventFilter(this);
    music_hScrollLayout = new QHBoxLayout();
    music_ScrollArea->setLayout(music_hScrollLayout);

    music_playLayout();

    this->setLayout(music_playlayout);
    this->setContentsMargins(0,0,0,0);
    /*this->setSizePolicy(QSizePolicy::Expanding,
                        QSizePolicy::Expanding);*/


    QFile file(":/qss/music_playWidget.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style;
        style = QLatin1String(file.readAll());
        this->setStyleSheet(style);
    }

    //ͼƬ??ת?Ƕ?
    cd_rotate = 0;
    bofanggan_rotate = 0;
    getSongLyricReply = nullptr;

    ConnectInit();
	InitCD();
}

void music_play::ConnectInit(void)
{
    //????????????????
    PSEventController::subscribe(this,EVENT_FINDMUSICLYRIC);

    //???ʸ??¶?ʱ??
    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),
            this,SLOT(LyricUpdate_TimerOut()));
    timer->start(100);

    ////??????ʱ??
    animation_Timer = new QTimer();
    connect(animation_Timer, &QTimer::timeout,
		this, [=] {std::thread t1(&music_play::Animation_TimerOut, this);
		t1.detach(); 
	}); 
    animation_Timer->start(50);
}

void music_play::on_psEvent_findmusicLyric(QString lyric)
{
    AnalysicLyric(&lyric);
}

void music_play::Animation_TimerOut()
{
    if(music_player->state() == QMediaPlayer::PlayingState)
    {
        cd_rotate = (++cd_rotate)%360;
        if (bofanggan_rotate < 25)
        {
            bofanggan_rotate+=5;
        }
    }
    else if (music_player->state() == QMediaPlayer::StoppedState)
    {
        if (bofanggan_rotate >0)
        {
            bofanggan_rotate-=5;
        }
    }
    else if (music_player->state() == QMediaPlayer::PausedState)
    {
        if (bofanggan_rotate >0)
        {
            bofanggan_rotate-=5;
        }
    }

    //this->update();
    this->music_ScrollArea->update();
}

void music_play::music_playLayout(void)
{
    music_playlayout = new QHBoxLayout();
    music_playlayout->addWidget(music_ScrollArea);
    music_playlayout->setContentsMargins(0,0,0,0);

    QSpacerItem *leftSpacer = new QSpacerItem(30,525,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Expanding);
    QSpacerItem *rightSpacer = new QSpacerItem(30,525,
                                               QSizePolicy::Expanding,
                                               QSizePolicy::Expanding);

    Lefd_cdWidget();
    Right_lyricWidget();

    QHBoxLayout *play_layout = new QHBoxLayout();
    play_layout->addLayout(cd_layout);
    play_layout->addSpacing(100);
    play_layout->addLayout(lyric_layout);


    music_hScrollLayout->addSpacerItem(leftSpacer);
    music_hScrollLayout->addLayout(play_layout);
    music_hScrollLayout->addSpacerItem(rightSpacer);
}

void music_play::Lefd_cdWidget(void)
{
    cd_label = new QLabel();
    cd_label->setObjectName("cd_Button");
    cd_label->setMaximumSize(350,350);
    cd_label->setMinimumSize(350,350);

    /*QImage Image;
    Image.load(":/images/music_play/heijiaopian.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    QPixmap fitpixmap = pixmap.scaled(350,350,
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation);

    cd_label->setPixmap(fitpixmap);
    cd_label->setAlignment(Qt::AlignCenter);*/

    QHBoxLayout *button_layout = new QHBoxLayout();

    //for(int i=0;i<4;i++)
    //{
    //    music_button[i] = new QPushButton();
    //    music_button[i]->setMaximumSize(30,30);
    //    music_button[i]->setMinimumSize(30,30);
    //    button_layout->addWidget(music_button[i]);
    //}

    cd_layout = new QVBoxLayout();

    cd_layout->addWidget(cd_label);
    cd_layout->addLayout(button_layout);
}
void music_play::Right_lyricWidget(void)
{
    lyric_display = new QTextBrowser();
    lyric_display->setMinimumSize(400,300);
    lyric_display->setMaximumSize(400,300);

    lyric_Edit = new QTextEdit();
    lyric_Edit->setMinimumSize(450,310);
    lyric_Edit->setMaximumSize(450,310);
    lyric_Edit->setReadOnly(true);
    //??ֹ???????ı???ѡ??
    lyric_Edit->setTextInteractionFlags(Qt::NoTextInteraction);

    lyric_Edit->verticalScrollBar()->setMaximumWidth(7);

    BlockInit();

    lyric_layout = new QVBoxLayout();

    MsgLayout();

    lyric_layout->addLayout(message_vLayout);
    lyric_layout->addWidget(lyric_Edit);
    lyric_layout->setContentsMargins(0,0,0,0);
    lyric_layout->setSpacing(0);
}

void music_play::MsgLayout(void)
{
    message_vLayout = new QVBoxLayout();
    message_vLayout->setContentsMargins(0,0,0,0);
    message_hLayout = new QHBoxLayout();
    message_hLayout->setContentsMargins(0,0,0,0);

    for (int i=0; i<4; i++)
    {
        msgLabel[i] = new QLabel();
        msgLabel[i]->setObjectName(QString("msgLabel%1").arg(i));
        if(i>0)
        {
            msgLabel[i]->setMaximumSize(50,20);
            msgLabel[i]->setMinimumSize(50,20);
        }
        else
        {
            msgLabel[i]->setMinimumSize(400,50);
            msgLabel[i]->setMaximumSize(400,50);
        }
    }

    msgLabel[0]->setText(__QString("δ֪"));
    msgLabel[1]->setText(__QString("ר??:"));
    msgLabel[2]->setText(__QString("????:"));
    msgLabel[3]->setText(__QString("??Դ:"));

    for (int i=0; i<3; i++)
    {
        msgButton[i] = new QPushButton();
        msgButton[i]->setObjectName(QString("msgButton%1").arg(i));
        msgButton[i]->setMinimumSize(100,20);
        msgButton[i]->setMaximumSize(100,20);
        msgButton[i]->setText(__QString("δ֪"));
    }

    message_hLayout->addWidget(msgLabel[1]);
    message_hLayout->addWidget(msgButton[0]);
    message_hLayout->addWidget(msgLabel[2]);
    message_hLayout->addWidget(msgButton[1]);
    message_hLayout->addWidget(msgLabel[3]);
    message_hLayout->addWidget(msgButton[2]);

    message_vLayout->addWidget(msgLabel[0]);
    message_vLayout->addLayout(message_hLayout);
}

void music_play::BlockInit()
{
    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(10,QTextBlockFormat::LineDistanceHeight);
    auto textCursor = lyric_Edit->textCursor();

    QTextCharFormat fmt;
    fmt.setForeground(QColor(102,102,102));

    fmt.setFont(QFont("΢???ź?",11,QFont::Normal,false));
    textCursor.mergeCharFormat(fmt);

    textCursor.setBlockFormat(blockFormat);
    lyric_Edit->setTextCursor(textCursor);
}

void music_play::Load_lyrics(MediaObjectInfo info,QString path)
{

    qDebug() << "path:" << path;

    if (info.songSource == LOCALMUSIC)
    {
        if(path == nullptr)
            return;
        QFile file(path);

        if(!file.exists())
        {
            return ;
        }

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << "false";

        QString lyric = file.readAll();

        AnalysicLyric(&lyric);

        file.close();
    }
    else if (info.songSource == NETWORKMUSIC)
    {
        GetSongLyricBySongId(info.songId);
    }
}


void music_play::GetSongLyricBySongId(QString Id)
{
    QString url = ApiOfGetLyricById.arg(Id);
    NetWorkManager::get(url, GetLyric_Reply,this);
}

void music_play::GetLyric_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
    music_play *p_musicplay = reinterpret_cast<music_play*>(pCusData);

    if (errCode == E_NetTimeOut)
    {
        qDebug() << "Reply_timeout";
    }
    else if (errCode == E_NetReplyError)
    {
        qDebug() << "Reply_error";
    }
    else if (errCode == E_NetOK)
    {
        QByteArray array = bytes;
        QJsonParseError jsonError;
        QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);

        if (jsonError.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                QJsonObject obj = json.object();
                QJsonValue val = obj.value("lrc");
                QJsonObject obj1 = val.toObject();
                p_musicplay->songLyric = obj1["lyric"].toString();

                p_musicplay->AnalysicLyric(&p_musicplay->songLyric);
                //PSEventController::publish(EVENT_FINDMUSICLYRIC,Q_ARG(QString,songLyric));
            }
        }
        else
        {
            qDebug() << "JSON_ERROR:" << jsonError.error;
        }
    }
    p_musicplay = nullptr;
}


void music_play::AnalysicLyric(const QString *lyricString)
{
    QString lyric = *lyricString;
    QStringList lrclist = lyric.split("\n");

    lyric_Edit->clear();
    BlockInit();

    size = lrclist.size();

    QRegularExpression rex("\\[(ar)?(ti)?(al)?(by)?(offset)?(\\d+)?:(\\d+)?(\\.\\d+)?(\\S+)?\\]");

    lrcMap.clear();

    for(int i=0;i<lrclist.size()-1;i++)
    {
        QString lrc= lrclist.at(i);
        //qDebug() << "befor:" << lrclist.at(i);
        QRegExp RegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\d\\]");

        bool match = RegExp.indexIn(lrclist.at(i));
        if (match == false)
        {
            int s_1 = lrc.mid(1,2).toInt(); //??
            int s_2 = lrc.mid(4,2).toInt(); //??
            int s_3 = lrc.mid(7,2).toInt(); //????

            int lrctime = (s_1*60+s_2)*100+s_3;

            QString lrcstr = lrc.mid(11);
            //qDebug() << "time" << lrctime << "lrc:" << lrcstr;
            lyric_Edit->append(lrcstr);
            lrcMap.insert(lrctime,lrcstr);
        }

        RegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");//??֪??Ϊɶ???кü??ָ??ʸ?ʽ???ͽ??????ְѡ???????

        match = RegExp.indexIn(lrclist.at(i));
        if (match == false)
        {
            int s_1 = lrc.mid(1,2).toInt(); //??
            int s_2 = lrc.mid(4,2).toInt(); //??
            int s_3 = lrc.mid(7,2).toInt(); //????

            int lrctime = (s_1*60+s_2)*100+s_3;

            QString lrcstr = lrc.mid(10);
            //qDebug() << "time" << lrctime << "lrc:" << lrcstr;
            lyric_Edit->append(lrcstr);
            lrcMap.insert(lrctime,lrcstr);
        }
    }
    lyric_Edit->moveCursor(QTextCursor::Start);
}

void music_play::LyricUpdate_TimerOut(void)
{
    if (lrcMap.isEmpty())
        return ;
    int time = music_player->position()/10;

    QMap <int,QString>::iterator iter = lrcMap.begin();

    while(iter != lrcMap.end())
    {
        QMap <int,QString>::iterator iter1 = iter;
        iter1++;

        iter1 = (iter1 == lrcMap.begin() && iter == lrcMap.end()) ? lrcMap.end() : iter1;

        if((time >= iter.key()-100) && (time <= (iter1).key()))//?ж?һ???и??ʵĳ???ʱ?䣬?????????Ļ???????ƥ?????Կ??ٸ???
        //if((iter.key()>=time-55) && (iter.key() <= time+55)) 
        {
            if (LyricPosition != iter.key())
            {
                LyricPosition = iter.key();
                int row = std::distance(lrcMap.begin(),iter);

                QTextCursor cursor = lyric_Edit->textCursor();
                QTextCharFormat fmt;
                fmt.setForeground(QColor(102,102,102));

                fmt.setFont(QFont("΢???ź?",11,QFont::Normal,false));
                cursor.mergeCharFormat(fmt);

                QTextBlock block = lyric_Edit->document()->findBlockByNumber(row);
                lyric_Edit->setTextCursor(QTextCursor(block));
                lyric_Edit->find(iter.value());
                //ѡ?и???????
                cursor = lyric_Edit->textCursor();
                fmt.setForeground(Qt::black);
                fmt.setFont(QFont("΢???ź?",11,QFont::Bold,false));
                cursor.mergeCharFormat(fmt);

                
				auto endIter = lrcMap.end();
				--endIter;
				float nowTime = row;
				float endTime = lrcMap.count();
				int high = lyric_Edit->verticalScrollBar()->maximum()+ SINGLESTEP;	//??һ?о??룬????????ʱ????һ????ʾ????
                int value = int(nowTime / endTime * high);
                lyric_Edit->verticalScrollBar()->setValue(value);
            }
            break;
        }

        iter++;
    }
}

bool music_play::eventFilter(QObject *watched,QEvent *event)
{
    Q_UNUSED(event);

    //??׽cd??????ʱ??

	if (watched == this->music_ScrollArea && event->type() == QEvent::Paint)
	{
		QPainter painter(this->music_ScrollArea);
		painter.save();
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::Antialiasing);

		//???ڽ?Ƭ
		painter.translate(cd_label->pos().rx(), cd_label->pos().ry());
		painter.translate(cd_label->width() / 2, cd_label->height() / 2);
		painter.rotate(cd_rotate);
		painter.translate(-cd_label->width() / 2, -cd_label->height() / 2);
		painter.drawPixmap(0, 0, cd_label->width(), cd_label->height(), pixCD);

		painter.restore();

		//???ڽ?Ƭ?м???ר??ͼƬ

		painter.save();

		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		QPainterPath path;
		painter.translate(cd_label->pos().rx(), cd_label->pos().ry());
		path.addEllipse(cd_label->rect().center(), 113, 113);
		painter.setClipPath(path);

		painter.translate(cd_label->width() / 2, cd_label->height() / 2);
		painter.rotate(cd_rotate);
		painter.translate(-113, -113);

		painter.drawPixmap(0, 0, 226, 226, pixAlbum);

		painter.restore();

		//????????
		painter.save();
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::Antialiasing);

		int x = cd_label->pos().rx() + cd_label->width() / 2 - 150;
		int y = cd_label->pos().ry() - 195;
		painter.translate(x, y);
		painter.translate(150, 150);
		painter.rotate(bofanggan_rotate);
		painter.translate(-150, -150);
		painter.drawPixmap(0, 0, 300, 300, pixRod);

		painter.restore();
	}
	

    return 0;
}

void music_play::InitCD()
{
	pixCD.load(":/images/music_play/heijiaopian.png");
	pixCD.scaled(350, 350);

	if (!pixdata)
	{
		pixAlbum = QPixmap(":/images/playWidget/head.png");
	}
	else
	{
		pixAlbum = pixdata;
	}
	pixAlbum.scaled(226, 226);
	
	pixRod.load(":/images/music_play/bofanggan.png");
	pixRod.scaled(300, 300);
}