#include <QApplication>
#include <Database/dbManager.h>
#include <QDesktopWidget>
#include <QSharedMemory>
#include <QProcess>
#include <QFile>
#include <Windows.h>

#include "lib/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QSharedMemory shared_memory;
	shared_memory.setKey(QString("666666"));//设置固定共享内存段的key值

	if (!shared_memory.create(1)) //创建1byte的共享内存段
	{
		return -1;
	}
	dbManager::destoryAllDBConn();
	//	启动网易云服务端，才能调用相关API
	QProcess* m_process = new QProcess();
	QString cmdstr = QString("./app.exe");
	m_process->start(cmdstr);
	MainWindow w;
	QDesktopWidget *desktop = QApplication::desktop();
	w.move((desktop->width() - w.width()) / 2, (desktop->height() - w.height()) / 2);
	w.show();
	//dbManager::destoryAllDBConn();

	return a.exec();
}
