#ifndef QWORKER_H
#define QWORKER_H

#include <QObject>
#include <QDebug>
#include <QTime>

class QWorker : public QObject
{
	Q_OBJECT
public:
	explicit QWorker(QString _name, QObject *parent = nullptr);
	~QWorker();

	volatile bool stop;

public slots:
	void mergeSort(QVector<int> _data);
	void bubbleSort(QVector<int> _data);
	void quickSort(QVector<int> _data);

signals:
	void workerFinished(QString name, QVector<int> data, int msecs);

private:
	QVector<int> mergeSortBody(QVector<int> data);
	QVector<int> mergeBody(QVector<int> left, QVector<int> right);

	void quickSortBody(QVector<int> &data, int start = 0, int end = -1);
	int  quickSortPart(QVector<int> &data, int start, int end);

private:
	QString name;
	QVector<int> data;

private: // sort sub
};

#endif // QWORKER_H
