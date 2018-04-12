#include "QWorker.h"

QWorker::QWorker(QString _name, QObject *parent) : QObject(parent)
{
	name = _name;
	qDebug() << QString("[ %1 ] QWorker()").arg(name);
}

QWorker::~QWorker()
{
	qDebug() << QString("[ %1 ] ~QWorker()").arg(name);
}

void QWorker::mergeSort(QVector<int> _data)
{
	QTime timeBegin = QTime::currentTime();
	qDebug() << QString("[ %1 ] sort started.").arg(name);

	stop = false;
	data = mergeSortBody(_data);

	emit workerFinished(name, data, timeBegin.msecsTo(QTime::currentTime()));
}

QVector<int> QWorker::mergeSortBody(QVector<int> data)
{
	if (data.size() <= 1 || stop)
		return data;

	QVector<int> left  = mergeSortBody(data.mid(0, data.size() / 2));
	QVector<int> right = mergeSortBody(data.mid(left.size(), data.size() - left.size()));

	return mergeBody(left, right);
}

QVector<int> QWorker::mergeBody(QVector<int> left, QVector<int> right)
{
	QVector<int> result;
	result.reserve(left.size() + right.size());

	int leftFirstIndex  = 0;
	int rightFirstIndex = 0;

	while (leftFirstIndex < left.size() && rightFirstIndex < right.size() && !stop)
		if (left.at(leftFirstIndex) <= right.at(rightFirstIndex))
			result.append(left.at(leftFirstIndex++));
		else
			result.append(right.at(rightFirstIndex++));

	if (left.size() - leftFirstIndex > 0)
		result.append(left.mid(leftFirstIndex));
	if (right.size() - rightFirstIndex > 0)
		result.append(right.mid(rightFirstIndex));

	return result;
}
void QWorker::bubbleSort(QVector<int> _data)
{
	QTime timeBegin = QTime::currentTime();
	qDebug() << QString("[ %1 ] sort started.").arg(name);

	stop = false;
	data = _data;

	bool swapped;
	do
	{
		swapped = false;
		for (int i = 1; i < data.size() && !stop; ++i)
			if (data.at(i-1) > data.at(i))
			{
				int tmp   = data.at(i);
				data[i]   = data.at(i-1);
				data[i-1] = tmp;
				swapped   = true;
			}
	}
	while (swapped != false && !stop);

	emit workerFinished(name, data, timeBegin.msecsTo(QTime::currentTime()));
}

void QWorker::quickSort(QVector<int> _data)
{
	QTime timeBegin = QTime::currentTime();
	qDebug() << QString("[ %1 ] sort started.").arg(name);

	stop = false;
	data = _data;

	quickSortBody(data);

	emit workerFinished(name, data, timeBegin.msecsTo(QTime::currentTime()));
}

void QWorker::quickSortBody(QVector<int> &data, int start, int end)
{
	if (end == -1) end = data.size() - 1;

	if (start >= end || stop) return;

	int base = quickSortPart(data, start, end);

	quickSortBody(data, start, base - 1);
	quickSortBody(data, base + 1, end);
}

int QWorker::quickSortPart(QVector<int> &data, int start, int end)
{
	int temp;
	int marker = start;

	for (int i = start; i <= end && !stop; ++i)
		if (data.at(i) < data.at(end))
		{
			temp         = data.at(marker);
			data[marker] = data.at(i);
			data[i]      = temp;
			marker++;
		}

	temp = data.at(marker);
	data[marker] = data.at(end);
	data[end] = temp;

	return marker;
}
