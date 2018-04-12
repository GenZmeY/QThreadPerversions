#ifndef QMAIN_H
#define QMAIN_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QRandomGenerator>
#include <QMutex>

#include <limits.h>

#include "QWorker.h"

#define TRY_COUNT 10            // Количество повторов
#define VECTOR_SIZE 10000000    // Размер массива

#define THREADS   3             // Не менять!

class QMain : public QObject
{
	Q_OBJECT
public:
	explicit QMain(QObject *parent = nullptr);
	~QMain();

signals:
	void closeProgram();
	void startWorker(QVector<int> data);
	void restart();

public slots:
	void beginSort();
	void workerSendResult(QString name, QVector<int> data, int msecs);
	void workerStopped();

private:
	QThread t1, t2, t3;
	QWorker *w1, *w2, *w3;
	QMutex mutex;
	QVector<volatile bool*> stopPointers;
	bool stop;
	int activeThreads;
	int currentTry;

private:
	QVector<int> generateRandomVector();
};

#endif // QMAIN_H
