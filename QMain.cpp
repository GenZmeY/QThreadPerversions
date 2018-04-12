#include "QMain.h"

QMain::QMain(QObject *parent) : QObject(parent)
{
	qDebug() << QString("[ QMain   ] QMain()");

	qRegisterMetaType<QVector<int>>("QVector<int>");

	w1 = new QWorker("Worker1");
	w2 = new QWorker("Worker2");
	w3 = new QWorker("Worker3");

	currentTry = 0;

	connect(w1, SIGNAL(workerFinished(QString, QVector<int>, int)), this, SLOT(workerSendResult(QString, QVector<int>, int)), Qt::DirectConnection);
	connect(w2, SIGNAL(workerFinished(QString, QVector<int>, int)), this, SLOT(workerSendResult(QString, QVector<int>, int)), Qt::DirectConnection);
	connect(w3, SIGNAL(workerFinished(QString, QVector<int>, int)), this, SLOT(workerSendResult(QString, QVector<int>, int)), Qt::DirectConnection);

	connect(this, SIGNAL(restart()), this, SLOT(beginSort()), Qt::QueuedConnection);

	connect(this, SIGNAL(startWorker(QVector<int>)), w1, SLOT(mergeSort(QVector<int>)));
	connect(this, SIGNAL(startWorker(QVector<int>)), w2, SLOT(bubbleSort(QVector<int>)));
	connect(this, SIGNAL(startWorker(QVector<int>)), w3, SLOT(quickSort(QVector<int>)));

	stopPointers << &w1->stop << &w2->stop << &w3->stop;

	w1->moveToThread(&t1); t1.start();
	w2->moveToThread(&t2); t2.start();
	w3->moveToThread(&t3); t3.start();
}

QMain::~QMain()
{
	qDebug() << QString("[ QMain   ] ~QMain()");
	t1.quit(); t1.wait(); delete w1; t1.deleteLater();
	t2.quit(); t1.wait(); delete w2; t2.deleteLater();
	t3.quit(); t1.wait(); delete w3; t3.deleteLater();
}

QVector<int> QMain::generateRandomVector()
{
	QVector<int> data;
	data.reserve(VECTOR_SIZE);

	QRandomGenerator rnd;

	qDebug() << QString("[ QMain   ] Generating random vector...");
	for (int i = 0; i < VECTOR_SIZE; ++i)
	{
		data.append(rnd.bounded(INT_MIN, INT_MAX));
	}
	qDebug() << QString("[ QMain   ] Ready!");

	return data;
}

void QMain::beginSort()
{
	activeThreads = THREADS;
	stop = false;
	currentTry++;

	qDebug() << QString("[ QMain   ] ------------");
	qDebug() << QString("[ QMain   ] Try %1 of %2").arg(currentTry).arg(TRY_COUNT);

	emit startWorker(generateRandomVector());
}

void QMain::workerSendResult(QString name, QVector<int> data, int msecs)
{
	mutex.lock();
	if (stop)
	{
		qDebug() << QString("[ %1 ] sort stopped.").arg(name);
	}
	else
	{
		stop = true;
		for (volatile bool *ptr : stopPointers) *ptr = true;
		qDebug() << QString("[ %1 ] Finished with %2 msecs!").arg(name).arg(msecs);
		qDebug() << QString("[ %1 ] Elements: %2").arg(name).arg(data.size());
		// Do something with result
		//for (int a : data)
		//	qDebug() << a;
	}
	workerStopped();
	mutex.unlock();
}

void QMain::workerStopped()
{
	if (!--activeThreads)
	{
		if (currentTry == TRY_COUNT)
			emit closeProgram();
		else
			emit restart(); // Прямой вызов нельзя - иначе управление перейдет к "финишировавшему потоку"
	}
}
