#include <QCoreApplication>
#include <QTimer>

#include "QMain.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QMain shell;

	QObject::connect(&shell, SIGNAL(closeProgram()), &a, SLOT(quit()));
	QTimer::singleShot(0,&shell,SLOT(beginSort()));

	return a.exec();
}
