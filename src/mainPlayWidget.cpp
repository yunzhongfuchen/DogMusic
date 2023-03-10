#include "../lib/mainPlayWidget.h"
#include <QFile>
#include <QApplication>

mainPlay::mainPlay(QWidget *parent):
    QWidget(parent)
{
    this->setObjectName("main_playWidget");
    this->setMinimumSize(1020,80);
    this->setMaximumHeight(80);
    //this->setMaximumSize(1020,80);
    this->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground);

    LoadStyleSheet();


    play_hBoxLayout = new QHBoxLayout();
    play_hBoxLayout->setContentsMargins(0,0,0,0);

    PlayLayout();
    play_slider->installEventFilter(this);

    this->setLayout(play_hBoxLayout);

    QWidget *findWnd = nullptr;
    QWidgetList allWidgetlist = QApplication::allWidgets();
    for(QWidget *pwnd : allWidgetlist)
    {
        //qDebug() << pwnd->objectName();
        if(pwnd->objectName() == "main_Widget")
        {
            findWnd = pwnd;
            break;
        }
    }

    //音乐播放界面初始化
    music_playWidget = new music_play(findWnd);

    //音乐播放列表初始化
    musicplaylist = new Pop_playlist(findWnd);

    SingerImageReply = nullptr;

    mediaPlayerInit();
    //ScanSongs();

    ConnectInit();
}

void mainPlay::LoadStyleSheet(void)
{
    QFile file(":/qss/main_playWidget.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }

}

void mainPlay::PlayLayout(void)
{
    SongInfoLayout();
    PlayCtrlLayout();
    SoundEffectLayout();

    QSpacerItem *playSpacer1 = new QSpacerItem(0,10,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Expanding);
    QSpacerItem *playSpacer2 = new QSpacerItem(0,10,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Expanding);

    play_hBoxLayout->addWidget(songInfoWidget);
    play_hBoxLayout->addSpacerItem(playSpacer1);
    play_hBoxLayout->addWidget(playCtrlWidget);
    play_hBoxLayout->addSpacerItem(playSpacer2);
    play_hBoxLayout->addWidget(soundWidget);
    play_hBoxLayout->setContentsMargins(0,0,0,0);

}
void mainPlay::SongInfoLayout(void)
{
    musicCoverButton = new QPushButton();
    musicCoverButton->setObjectName("musicCover_button");
    musicCoverButton->setFixedSize(60,60);
    musicCoverButton->setSizePolicy(
                QSizePolicy::Minimum,
                QSizePolicy::Fixed);


    QPixmap pixmap(":/images/playWidget/head.png");
    QIcon icon;
    icon.addPixmap(pixmap);
    musicCoverButton->setIcon(icon);
    musicCoverButton->setIconSize(musicCoverButton->rect().size());

    favorButton = new QPushButton();
    favorButton->setObjectName("play_facor_button");
    favorButton->setFixedSize(20,20);


    songNameLabel = new QLabel();
    songNameLabel->setMaximumSize(300,25);
    songNameLabel->setMinimumSize(20,25);
    songNameLabel->setText(__QString("无"));
    songNameLabel->adjustSize();

    QHBoxLayout *songname_hLayout = new QHBoxLayout();
    QSpacerItem *songname_hSpacing = new QSpacerItem(50,30,
                                                     QSizePolicy::Expanding,
                                                     QSizePolicy::Expanding);
    songname_hLayout->addWidget(songNameLabel);
    songname_hLayout->addWidget(favorButton);
    songname_hLayout->addSpacerItem(songname_hSpacing);
    songname_hLayout->setSpacing(0);

    singerNameLabel = new QLabel();
    singerNameLabel->setMaximumSize(150,25);
    singerNameLabel->setText(__QString("无"));
    singerNameLabel->adjustSize();

    play_ginfoLayout = new QGridLayout();
    play_ginfoLayout->addWidget(musicCoverButton,0,0);
    play_ginfoLayout->addLayout(songname_hLayout,0,1);
    play_ginfoLayout->addWidget(singerNameLabel,1,1);

    play_ginfoLayout->setRowStretch(0,1);
    play_ginfoLayout->setRowStretch(1,1);
    //play_ginfoLayout->setColumnStretch(0,1);
    //play_ginfoLayout->setColumnStretch(1,2);

    songInfoWidget = new QWidget();
    songInfoWidget->setLayout(play_ginfoLayout);
    songInfoWidget->setMinimumSize(250,80);
    songInfoWidget->setMaximumSize(250,80);
}

void mainPlay::PlayCtrlLayout(void)
{
    for (int i=0;i<5;i++)
    {
        play_ctrlButton[i] = new QPushButton();
    }

    //播放方式
    play_ctrlButton[0]->setMinimumSize(30,30);
    play_ctrlButton[0]->setMaximumSize(30,30);
    play_ctrlButton[0]->setObjectName("playmodeButton");
	//QIcon icon0(QPixmap(":/images/playWidget/playmode_favor_nor.png").scaled(20, 20));
	play_ctrlButton[0]->setStyleSheet("QPushButton{border-image: url(:/images/playWidget/playmode_favor_nor.png);}" 
		"QPushButton:hover{border-image: url(:/images/playWidget/playmode_favor_hover.png);}");
    //上一首
    play_ctrlButton[1]->setMinimumSize(15,15);
    play_ctrlButton[1]->setMaximumSize(15,15);
    play_ctrlButton[1]->setObjectName("lastSongButton");
    //播放/暂停
    play_ctrlButton[2]->setMinimumSize(30,30);
    play_ctrlButton[2]->setMaximumSize(30,30);
    play_ctrlButton[2]->setCheckable(true);
    play_ctrlButton[2]->setObjectName("playpauseButton");
    QIcon icon(QPixmap(":/images/playWidget/play.png").scaled(20,20));
    play_ctrlButton[2]->setIcon(icon);

    //play_ctrlButton[2]->setStyleSheet("border-image: url(:/images/playWidget/pause.png);");
    //下一首
    play_ctrlButton[3]->setFixedSize(15,15);
    play_ctrlButton[3]->setObjectName("nextSongButton");
    //歌词
    play_ctrlButton[4]->setFixedSize(15,17);
    play_ctrlButton[4]->setObjectName("lyricButton");

    play_hLayout = new QHBoxLayout();
    play_hLayout->addWidget(play_ctrlButton[0]);
    play_hLayout->addWidget(play_ctrlButton[1]);
    play_hLayout->addWidget(play_ctrlButton[2]);
    play_hLayout->addWidget(play_ctrlButton[3]);
    play_hLayout->addWidget(play_ctrlButton[4]);
    //play_hLayout->addSpacing(20);
    //播放时长
    play_timeLabe[0] = new QLabel();
    play_timeLabe[0]->setText("00:00");
    play_timeLabe[0]->setFixedSize(60,20);
    //总时长
    play_timeLabe[1] = new QLabel();
    play_timeLabe[1]->setText("00:00");
    play_timeLabe[1]->setFixedSize(60,20);
    //进度条
    play_slider = new QSlider();
    play_slider->setObjectName("play_slide");
    play_slider->setOrientation(Qt::Horizontal);
    play_slider->setRange(0,100);
    play_slider->setFixedSize(300,15);

    play_gctrlLayout = new QGridLayout();
    //设置各个子部件之间的水平间隔，不会为该间距分配索引
    play_gctrlLayout->setHorizontalSpacing(10);
    play_gctrlLayout->addLayout(play_hLayout,0,1);
    play_gctrlLayout->addWidget(play_timeLabe[0],1,0);
    play_gctrlLayout->addWidget(play_slider,1,1);
    play_gctrlLayout->addWidget(play_timeLabe[1],1,2);
    //设置居中
    play_gctrlLayout->setAlignment(Qt::AlignCenter);
    play_gctrlLayout->setRowStretch(0,1);//设置行列比例系数
    play_gctrlLayout->setRowStretch(1,1);
    play_gctrlLayout->setColumnStretch(0,1);
    play_gctrlLayout->setColumnStretch(1,6);
    play_gctrlLayout->setColumnStretch(2,1);

    playCtrlWidget = new QWidget();
    playCtrlWidget->setLayout(play_gctrlLayout);
    playCtrlWidget->setMaximumSize(480,80);
    playCtrlWidget->setMinimumSize(480,80);
}

void mainPlay::SoundEffectLayout(void)
{
    for(int i=0;i<4;i++)
    {
        sound_PushButton[i] = new QPushButton();
    }
    sound_PushButton[0]->setMinimumSize(30,30);
    sound_PushButton[0]->setMaximumSize(30,30);
    sound_PushButton[0]->setObjectName("qualityButton");
    sound_PushButton[1]->setMinimumSize(24,22);
    sound_PushButton[1]->setMaximumSize(24,22);
    sound_PushButton[1]->setObjectName("effectButton");
    sound_PushButton[2]->setMinimumSize(21,24);
    sound_PushButton[2]->setMaximumSize(21,24);
    sound_PushButton[2]->setObjectName("muteButton");
    sound_PushButton[3]->setMinimumSize(20,20);
    sound_PushButton[3]->setMaximumSize(20,20);
    sound_PushButton[3]->setObjectName("playlistButton");

    sound_Volume = new QSlider();
    sound_Volume->setOrientation(Qt::Horizontal);
    sound_Volume->setObjectName("volume_Slider");
    sound_Volume->setRange(0,100);
    sound_Volume->setValue(50);
    sound_Volume->setMinimumSize(70,30);
    sound_Volume->setMaximumSize(70,30);

    sound_hBoxLayout= new QHBoxLayout();
    sound_hBoxLayout->addWidget(sound_PushButton[0]);
    sound_hBoxLayout->addWidget(sound_PushButton[1]);
    sound_hBoxLayout->addWidget(sound_PushButton[2]);
    //sound_hBoxLayout->addSpacing(10);
    sound_hBoxLayout->addWidget(sound_Volume);
    sound_hBoxLayout->addWidget(sound_PushButton[3]);

    soundWidget = new QWidget();
    soundWidget->setLayout(sound_hBoxLayout);
    soundWidget->setMinimumSize(300,80);
    soundWidget->setMaximumSize(300,80);
}

//大小变化
void mainPlay::resizeEvent(QResizeEvent *e)
{


}

void mainPlay::ConnectInit(void)
{
    //订阅清空列表事件
    PSEventController::subscribe(this,EVENT_CLEARLIST);
    //更新播放列表事件
    PSEventController::subscribe(this,EVENT_UPDATELIST);
    //订阅播放事件
    PSEventController::subscribe(this,EVENT_FINDMUSICPLAY);
    //订阅下一首播放事件
    PSEventController::subscribe(this,EVENT_FINDMUSICNEXTPLAY);
    //订阅播放列表指定歌曲事件
    PSEventController::subscribe(this,EVENT_PLAYAPPOINT_SONG);
    //订阅删除列表指定歌曲事件
    PSEventController::subscribe(this,EVENT_DELETEAPPOINT_SONG);


    //音乐封面点击页面跳转
    connect(musicCoverButton,SIGNAL(clicked()),this,SLOT(btn_cover_clicked()));
	connect(play_ctrlButton[4], SIGNAL(clicked()), this, SLOT(btn_cover_clicked()));
    //上一首按钮点击
    connect(play_ctrlButton[1],SIGNAL(clicked()),this,SLOT(btn_pre_clicked()));
    //播放按钮点击
    connect(play_ctrlButton[2],SIGNAL(clicked()),this,SLOT(btn_play_clicked()));
    //下一首按钮点击
    connect(play_ctrlButton[3],SIGNAL(clicked()),this,SLOT(btn_next_clicked()));

    //多媒体信号连接槽
    connect(musicPlayer,
            SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(musicPlayer,
            SIGNAL(durationChanged(qint64)),
            this,SLOT(musicPlayerDurationChanged(qint64)));
    connect(musicPlayer,
            SIGNAL(positionChanged(qint64)),
            this,SLOT(mediaPlayerPositionChanged(qint64)));
	connect(mediaPlaylist, &QMediaPlaylist::currentIndexChanged, this, &mainPlay::musicMetaDataChanged);
    connect(play_slider,
            SIGNAL(sliderReleased()),
            this,SLOT(durationSliderReleased()));

    //音乐播放列表连接槽
    connect(sound_PushButton[3],
            SIGNAL(clicked()),
            this,
            SLOT(btn_playlist_clicked()));
	//播放模式切换
	connect(play_ctrlButton[0], &QPushButton::clicked, this, &mainPlay::PlayModeChange);
	//音量控制
	connect(sound_Volume, &QSlider::valueChanged, this, &mainPlay::VolumnChange);
    QWidget *findWnd = nullptr;
    QWidgetList allWidgetlist = QApplication::allWidgets();
    for(QWidget *pwnd : allWidgetlist)
    {
        if(pwnd->objectName() == "localmusic")
        {
            findWnd = pwnd;
            break;
        }
    }

    //播放全部按钮
    connect(findWnd,SIGNAL(addall_signal( QList <MediaObjectInfo> *)),
            this,SLOT(Rev_ListMsg_slot( QList <MediaObjectInfo> *)));
	connect(MusicSignals::Instance(), &MusicSignals::signal_PlayAllSheetSong,
		this, &mainPlay::Rev_ListMsg_slot);
	connect(MusicSignals::Instance(), &MusicSignals::signal_PlayOneSheetSong,
		this, &mainPlay::PlayOneMusic);
	connect(MusicSignals::Instance(), &MusicSignals::signal_SetNextSheetSong,
		this, &mainPlay::SetNextSheetSong);
	connect(MusicSignals::Instance(), &MusicSignals::signal_UpdatePlayList,
		this, &mainPlay::on_psEvent_UpdateList);
    connect(play_ctrlButton[2],SIGNAL(toggled (bool)),this,SLOT(playbuttontoggled_slot(bool)));
}

void mainPlay::on_psEvent_playAppointSong(int index)
{
    musicPlayer->stop();

    mediaPlaylist->setCurrentIndex(index);

    musicPlayer->play();
}

void mainPlay::on_psEvent_deleteAppointSong(int index)
{
    if (index == mediaPlaylist->currentIndex())
    {
        musicPlayer->stop();
    }

    mediaObjectInfo.removeAt(index);
    mediaPlaylist->removeMedia(index);
    if (index == mediaPlaylist->currentIndex())
    {
        musicPlayer->play();
		musicMetaDataChanged();
    }
}

void mainPlay::playbuttontoggled_slot(bool state)
{
    if (play_ctrlButton[2]->isChecked())
    {
        QIcon icon(QPixmap(":/images/playWidget/pause.png").scaled(20,20));
        play_ctrlButton[2]->setIcon(icon);
    }
    else
    {
        QIcon icon(QPixmap(":/images/playWidget/play.png").scaled(20,20));
        play_ctrlButton[2]->setIcon(icon);
    }

}

void mainPlay::PlayOneMusic(MediaObjectInfo info)
{
	int i = 0;
	QList <MediaObjectInfo> mediaObjectInfoTemp = mediaObjectInfo;
	for (MediaObjectInfo item : mediaObjectInfoTemp)
	{
		//	如果列表中已经存在
		if (item.songId  == info.songId && item.singer == info.singer)	//同一首歌获取的url不一定每次都一样，因此不能用来做判断
		{
			//	如果为当前播放，直接退出
			if (i == mediaPlaylist->currentIndex())
			{
				return;
			}
			musicPlayer->stop();
			//	将列表中该项移动到当前播放的下一项
			int index = mediaPlaylist->currentIndex();
			if (i> mediaPlaylist->currentIndex())
			{
				for (int j = i; j > index + 1; j--)
				{
					MediaObjectInfo temp = mediaObjectInfoTemp[j];
					mediaObjectInfoTemp[j] = mediaObjectInfoTemp[j - 1];
					mediaObjectInfoTemp[j - 1] = temp;
				}
				MusicSignals::Instance()->PlayAllSheetSong(&mediaObjectInfoTemp);
				mediaPlaylist->setCurrentIndex(index+1);
			}
			else
			{
				for (int j = i; j < index; j++)
				{
					MediaObjectInfo temp = mediaObjectInfoTemp[j];
					mediaObjectInfoTemp[j] = mediaObjectInfoTemp[j + 1];
					mediaObjectInfoTemp[j + 1] = temp;
				}
				MusicSignals::Instance()->PlayAllSheetSong(&mediaObjectInfoTemp);
				mediaPlaylist->setCurrentIndex(index);
			}
			
			return;
		}
		++i;
	}
	// 否则添加到下一项
	int insertindex = mediaPlaylist->currentIndex() + 1;

	mediaPlaylist->insertMedia(insertindex, QUrl(info.filePath));

	mediaObjectInfo.insert(insertindex, info);

	musicplaylist->addNextSong(info, insertindex);

	btn_next_clicked();
}

void mainPlay::SetNextSheetSong(MediaObjectInfo info)
{
	int i = 0;
	QList <MediaObjectInfo> mediaObjectInfoTemp = mediaObjectInfo;
	for (MediaObjectInfo item : mediaObjectInfoTemp)
	{
		//	如果列表中已经存在
		if (item == info)
		{
			//	如果为当前播放，直接退出
			if (i == mediaPlaylist->currentIndex())
			{
				return;
			}
			musicPlayer->stop();
			//	将列表中该项移动到当前播放的下一项
			int index = mediaPlaylist->currentIndex();
			if (i > mediaPlaylist->currentIndex())
			{
				for (int j = i; j > index + 1; j--)
				{
					MediaObjectInfo temp = mediaObjectInfoTemp[j];
					mediaObjectInfoTemp[j] = mediaObjectInfoTemp[j - 1];
					mediaObjectInfoTemp[j - 1] = temp;
				}
				MusicSignals::Instance()->UpdatePlayList(mediaObjectInfoTemp);
			}
			else
			{
				for (int j = i; j < index; j++)
				{
					MediaObjectInfo temp = mediaObjectInfoTemp[j];
					mediaObjectInfoTemp[j] = mediaObjectInfoTemp[j + 1];
					mediaObjectInfoTemp[j + 1] = temp;
				}
				MusicSignals::Instance()->UpdatePlayList(mediaObjectInfoTemp);
				//mediaPlaylist->setCurrentIndex(index);
			}

			return;
		}
		++i;
	}
	// 否则添加到下一项
	int insertindex = mediaPlaylist->currentIndex() + 1;

	mediaPlaylist->insertMedia(insertindex, QUrl(info.filePath));

	mediaObjectInfo.insert(insertindex, info);

	musicplaylist->addNextSong(info, insertindex);
}

void mainPlay::on_psEvent_findmuicPlay(MediaObjectInfo info,QString url)
{
    /*mediaPlaylist->clear();
    mediaPlaylist->addMedia(QUrl(url));

    mediaObjectInfo.clear();
    MediaObjectInfo media;
    media.singer = info.Singer;
    media.fileName = info.SongName;
    media.filePath = url;
    media.filesize = "0M";
    media.playtime = info.playtime;
    media.AlbumName = info.AlbumName;

    mediaObjectInfo.append(media);

    musicPlayer->play();*/

    for(MediaObjectInfo item : mediaObjectInfo)
    {
        if (item == info)
        {
            return;
        }
    }

    int insertindex = mediaPlaylist->currentIndex() + 1;

    mediaPlaylist->insertMedia(insertindex,QUrl(url));

    mediaObjectInfo.insert(insertindex,info);

    musicplaylist->addNextSong(info,insertindex);

    btn_next_clicked();
}

void mainPlay::on_psEvent_findmusicNextPlay(MediaObjectInfo info,QString url)
{
    for(MediaObjectInfo item : mediaObjectInfo)
    {
        if (item == info)
        {
            return;
        }
    }

    int insertindex = mediaPlaylist->currentIndex() + 1;

    mediaPlaylist->insertMedia(insertindex,QUrl(url));

    mediaObjectInfo.insert(insertindex,info);

    musicplaylist->addNextSong(info,insertindex);
}



void mainPlay::on_psEvent_UpdateList(QList <MediaObjectInfo> list)
{
    mediaObjectInfo.clear();
    mediaObjectInfo = list;
    mediaPlaylist->clear();

    for (int i=0; i<mediaObjectInfo.count(); i++)
    {
        mediaPlaylist->addMedia(QUrl::fromLocalFile(mediaObjectInfo.at(i).filePath));
    }
}

void mainPlay::on_psEvent_ClearList()
{
    musicPlayer->stop();
    mediaPlaylist->clear();
	mediaObjectInfo.clear();
}

void mainPlay::Rev_ListMsg_slot( QList <MediaObjectInfo> *info)
{
    mediaObjectInfo.clear();
    mediaObjectInfo = *info;

    //清空播放器列表
    mediaPlaylist->clear();

    for(int i=0; i<mediaObjectInfo.count(); i++)
    {
        mediaPlaylist->addMedia(QUrl::fromLocalFile(mediaObjectInfo.at(i).filePath));
    }
	play_ctrlButton[2]->clicked();
}

void mainPlay::btn_playlist_clicked(void)
{
    if(musicplaylist)
    {
        if(musicplaylist->isHidden())
        {
            QWidget *navigation = musicplaylist->parentWidget()->findChild<QWidget *>("navigationWidget");
            QWidget *listWidget = musicplaylist->parentWidget()->findChild<QWidget *>("list_mainWidget");

            int x = musicplaylist->parentWidget()->width()-450 + musicplaylist->parentWidget()->mapToGlobal(QPoint(0,0)).rx();
            int y = navigation->height()+5 + musicplaylist->parentWidget()->mapToGlobal(QPoint(0,0)).ry();

            //int x = musicplaylist->parentWidget()->width()-450;
            //int y = navigation->height()+5;
            qDebug() << "height:" << listWidget->height()-5;

            musicplaylist->setGeometry(x-10,y,450,listWidget->height()-5);
            //musicplaylist->raise();
            musicplaylist->show();
            qDebug() << "width:" << musicplaylist->width();
            //musicplaylist->setFocus();
        }
        else
        {
            qDebug() << "playhide";
            //musicplaylist->setFocus();
            musicplaylist->hide();
        }
    }
}

void mainPlay::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::StoppedState:
        play_ctrlButton[2]->setChecked(false);
        break;
    case QMediaPlayer::PlayingState:
        play_ctrlButton[2]->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        play_ctrlButton[2]->setChecked(false);
        break;
    }
}

void mainPlay::mediaPlaylistCurrentIndexChanged(int index)
{
    if (-1 == index)
        return;

    songNameLabel->setText(mediaObjectInfo.at(index).fileName);
    songNameLabel->adjustSize();
    singerNameLabel->setText(mediaObjectInfo.at(index).singer);

    music_playWidget->msgLabel[0]->setText(mediaObjectInfo.at(index).fileName);
    music_playWidget->msgButton[0]->setText(mediaObjectInfo.at(index).AlbumName);
    music_playWidget->msgButton[1]->setText(mediaObjectInfo.at(index).singer);

    QString source = "";
    if (mediaObjectInfo.at(index).songSource == LOCALMUSIC)
        source = __QString("本地音乐");
    else if(mediaObjectInfo.at(index).songSource == NETWORKMUSIC)
        source = __QString("搜索页");
    else
        source = __QString("未知");

    QString lyricPath = "";

    if (mediaObjectInfo.at(index).songSource == LOCALMUSIC)
    {
           lyricPath = QString(mediaObjectInfo.at(index).filePath)
            .replace(".mp3",".lrc");
    }

    //music_playWidget->msgButton[2]->setText(source);

    music_playWidget->Load_lyrics(mediaObjectInfo.at(index),lyricPath);
}

void mainPlay::getSingerImage(MediaObjectInfo info)
{
    QString url = info.SingerImage;
    NetWorkManager::get(url,GetSingerImage_Reply,this);
}

void mainPlay::GetSingerImage_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
    mainPlay *p_mainPlay = reinterpret_cast<mainPlay *>(pCusData);

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
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        QIcon icon;
        icon.addPixmap(pixmap);
        p_mainPlay->musicCoverButton->setIcon(icon);

        p_mainPlay->music_playWidget->pixAlbum =
                pixmap.scaled(226, 226, Qt::KeepAspectRatio, Qt::FastTransformation);
    }

    p_mainPlay = nullptr;
}

void mainPlay::reply_singimageFinish(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.load(bytes);
        QIcon icon;
        icon.addPixmap(pixmap);
        musicCoverButton->setIcon(icon);

        /*QFile file("G:/Linux/UI/cloudmusic/bin/mp3/head.jpg");
        if (file.open(QIODevice::Append))
        {
            file.write(bytes);
            file.close();
        }*/
    }
}

void mainPlay::musicMetaDataChanged(void)
{
    int index = mediaPlaylist->currentIndex();
	musicplaylist->playlist_table->setCurrentCell(index, QItemSelectionModel::Select);;
    if (mediaObjectInfo.at(index).songSource == LOCALMUSIC)//如果这首歌是本地音乐
    {

        QString songname = musicPlayer->metaData(QMediaMetaData::Title).toString();
        music_playWidget->msgLabel[0]->setText(songname);
        QString singer = musicPlayer->metaData(QMediaMetaData::Author).toString();
        music_playWidget->msgButton[1]->setText(singer);
        QString albumName = musicPlayer->metaData(QMediaMetaData::AlbumTitle).toString();
        music_playWidget->msgButton[0]->setText(albumName);

        songNameLabel->setText(songname);
        songNameLabel->adjustSize();
        singerNameLabel->setText(singer);
    }
    else
    {
        songNameLabel->setText(mediaObjectInfo.at(index).fileName);
        songNameLabel->adjustSize();
        singerNameLabel->setText(mediaObjectInfo.at(index).singer);

        music_playWidget->msgLabel[0]->setText(mediaObjectInfo.at(index).fileName);
        music_playWidget->msgButton[0]->setText(mediaObjectInfo.at(index).AlbumName);
        music_playWidget->msgButton[1]->setText(mediaObjectInfo.at(index).singer);

    }

    QString source = "";
    if (mediaObjectInfo.at(index).songSource == LOCALMUSIC)
        source = __QString("本地音乐");
    else if(mediaObjectInfo.at(index).songSource == NETWORKMUSIC)
        source = __QString("搜索页");
    else
        source = __QString("未知");

    music_playWidget->msgButton[2]->setText(source);


    QString lyricPath = "";

    if (mediaObjectInfo.at(index).songSource == LOCALMUSIC)
    {
           lyricPath = QString(mediaObjectInfo.at(index).filePath)
            .replace(".mp3",".lrc");
    }
    else
    {
        getSingerImage(mediaObjectInfo.at(index));
    }

    music_playWidget->Load_lyrics(mediaObjectInfo.at(index),lyricPath);
}

void mainPlay::musicPlayerDurationChanged(qint64 duration)
{
    play_slider->setRange(0,duration / 1000);
    int second = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute,10);
    else
        mediaDuration = "0" + QString::number(minute,10);

    if (second >= 10)
        mediaDuration = mediaDuration + ":" + QString::number(second,10);
    else
        mediaDuration = mediaDuration + ":0" + QString::number(second,10);

    //修改播放总时常
    play_timeLabe[1]->setText(mediaDuration);

}

void mainPlay::mediaPlayerPositionChanged(qint64 position)
{
    if (!play_slider->isSliderDown())
        play_slider->setValue(position/1000);

    int second = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute,10);
    else
        mediaDuration = "0" + QString::number(minute,10);

    if (second >= 10)
        mediaDuration = mediaDuration + ":" + QString::number(second,10);
    else
        mediaDuration = mediaDuration + ":0" + QString::number(second,10);

    //修改播放总时常
    play_timeLabe[0]->setText(mediaDuration);

}

void mainPlay::durationSliderReleased()
{
    musicPlayer->setPosition(play_slider->value()*1000);
}

void mainPlay::btn_pre_clicked(void)
{
    musicPlayer->stop();
    int count = mediaPlaylist->mediaCount();
    if(0 == count || -1 == count)
        return;

    //列表上一个
    mediaPlaylist->previous();
    musicPlayer->play();
}

void mainPlay::btn_play_clicked(void)
{
    int state = musicPlayer->state();

    switch(state)
    {
    case QMediaPlayer::StoppedState:
        musicPlayer->play();
        break;

    case QMediaPlayer::PlayingState:
        musicPlayer->pause();
        break;

    case QMediaPlayer::PausedState:
        musicPlayer->play();
        break;
    }
}

void mainPlay::btn_next_clicked(void)
{
    musicPlayer->stop();
    int count = mediaPlaylist->mediaCount();
    if(0 == count)
        return;

    //列表下一个
    mediaPlaylist->next();
    musicPlayer->play();
}

void mainPlay::mousePressEvent(QMouseEvent *event)
{
    //mainPlay::mousePressEvent(event);

    double currentper = (float)event->pos().rx()/(float)play_slider->width();
    int pos = currentper*(play_slider->maximum()- play_slider->minimum()) + play_slider->minimum();

    //play_slider->setValue(pos);
}

bool mainPlay::eventFilter(QObject *watcher,QEvent *event)
{
    if (watcher == play_slider && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        //获取播放条的设置范围
        double range = play_slider->maximum()- play_slider->minimum() + play_slider->minimum();
        //获取当前鼠标点击占播放条的百分比
        double pos = (float)mouseEvent->pos().rx()/(float)play_slider->width()*range;
        play_slider->setValue(pos);
        musicPlayer->setPosition(play_slider->value()*1000);
    }
    //return QWidget::eventFilter(watcher,event);
    return false;
}

void mainPlay::PlayModeChange()
{
	if (mediaPlaylist->playbackMode() == QMediaPlaylist::Loop)
	{
		
	}
	switch (mediaPlaylist->playbackMode())
	{
	case QMediaPlaylist::Loop:
		{
			mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);
			play_ctrlButton[0]->setStyleSheet("QPushButton{border-image: url(:/images/playWidget/playmode_random.png);}"
				"QPushButton:hover{border-image: url(:/images/playWidget/playmode_random_hover.png);}");
			break;
		}
	case QMediaPlaylist::Random:
		{
			mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
			play_ctrlButton[0]->setStyleSheet("QPushButton{border-image: url(:/images/playWidget/playmode_one.png);}"
				"QPushButton:hover{border-image: url(:/images/playWidget/playmode_one_hover.png);}");
			break;
		}
	case QMediaPlaylist::CurrentItemInLoop:
		{
			mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
			play_ctrlButton[0]->setStyleSheet("QPushButton{border-image: url(:/images/playWidget/playmode_favor_nor.png);}"
				"QPushButton:hover{border-image: url(:/images/playWidget/playmode_favor_hover.png);}");
			break;
		}
	default:
		break;
	}
}

void mainPlay::VolumnChange(int vol)
{
	musicPlayer->setVolume(vol);
}

void mainPlay::btn_cover_clicked(void)
{
    QWidget *listWidget = nullptr;
    QWidget *naviWidget = nullptr;

    QWidgetList allWidgetlist = QApplication::allWidgets();
    for(QWidget *pwnd : allWidgetlist)
    {
        if(pwnd->objectName() == "list_mainWidget")
        {
            listWidget = pwnd;
            break;
        }
    }
    for(QWidget *pwnd : allWidgetlist)
    {
        if(pwnd->objectName() == "navigationWidget")
        {
            naviWidget = pwnd;
            break;
        }
    }

    if (music_playWidget->isHidden())
    {
        music_playWidget->show();
        music_playWidget->raise();
        QPropertyAnimation *showAnimation = new QPropertyAnimation(music_playWidget,"geometry");
        showAnimation->setDuration(200);
        showAnimation->setStartValue(QRect(0,listWidget->height()+naviWidget->height(),0,0));
        showAnimation->setEndValue(QRect(0,70,listWidget->width(),listWidget->height()));

        showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation *hideAnimation = new QPropertyAnimation(music_playWidget,"geometry");
        hideAnimation->setDuration(200);
        hideAnimation->setStartValue(QRect(0,70,listWidget->width(),listWidget->height()));
        hideAnimation->setEndValue(QRect(0,listWidget->height()+naviWidget->height(),0,0));
        hideAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QTimer::singleShot(200,this,SLOT(hide_animation_timeout()));
    }
}

void mainPlay::hide_animation_timeout(void)
{
    music_playWidget->hide();
}


void mainPlay::mediaPlayerInit(void)
{
    musicPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);

    music_playWidget->music_player = musicPlayer;

    //确保列表是空的
    mediaPlaylist->clear();
    //设置音乐播放器的列表为mediaPlaylist
    musicPlayer->setPlaylist(mediaPlaylist);
    //设置播放模式，loop是列表循环
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
}

void mainPlay::ScanSongs(void)
{
    QDir dir(QCoreApplication::applicationDirPath() + "/mp3");

    QDir dirsolutePath(dir.absolutePath());

    if(dirsolutePath.exists())
    {
        //定义过滤器
        QStringList filter;
        //查询所有.mp3.wav后缀的文件
        filter << "*.mp3";
        //获取该目录下的所有文件
        QFileInfoList files = dirsolutePath.entryInfoList(filter,QDir::Files);

        for(int i=0;i<files.count(); i++)
        {
            MediaObjectInfo info;
            //使用UTF-8编码
            QString fileName = QString::fromUtf8(files.at(i)
                                                 .fileName()
                                                 .replace(".mp3",  "")
                                                 .toUtf8()
                                                 .data());
            qDebug() << fileName;

            if(fileName.contains("-"))
                info.fileName = fileName + "\n" +fileName.split("-").at(1);
            else
                info.fileName = fileName;

            info.filePath = QString::fromUtf8(files.at(i)
                                              .filePath()
                                              .toUtf8()
                                              .data());

            //qDebug() << "fileName:" << info.fileName << "filePath:" << info.filePath;

            //媒体列表添加歌曲
            if (mediaPlaylist->addMedia(QUrl::fromLocalFile(info.filePath)))
            {
                //添加到容器数组里存储
                mediaObjectInfo.append(info);
                //添加歌曲名字至列表
            }
        }
    }
}
