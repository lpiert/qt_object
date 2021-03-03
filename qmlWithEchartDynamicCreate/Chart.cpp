#include "Chart.h"
#include <QDir>
#include <QJsonParseError>
#include <QJsonValue>


Chart::Chart()
{}


Chart* Chart::instance()
{
    static Chart* obj = new Chart();
    return obj;
}


QJsonObject Chart::getOpition(const QVariant& type) const
{
    auto it = m_options.find(type);
    if(it != m_options.end()) return it.value();
    return QJsonObject();

}


void Chart::loadOpotions()
{
    if(m_options.size()) return;

    QString path = "C:\\Users\\sentortu\\source\\codeFree\\qt\\quick\\option\\opt.json";
    QDir dir(path);
    path = dir.fromNativeSeparators(path);//  "\\"转为"/"
    QFile file(path);
    if(!file.open(QFile::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonParseError err;
    QJsonDocument json_doc = QJsonDocument::fromJson(data, &err);
    if (json_doc.isNull()) {
        qDebug() << err.errorString();
        return;
    }
    QJsonObject objects = json_doc.object();
    int size = objects.size();
	for (int i = 0; i < size; ++i)
	{
		auto keys = objects.keys();
		for (auto key : keys)
		{
			QJsonValue json = objects.value(key);
			if (json.isObject())
			{
				m_options[key] = json.toObject();
			}
		}
	}
}

