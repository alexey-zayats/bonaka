#include "boilermodels.h"
#include "boiler.h"
#include "scum.h"

#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>

namespace Calculate {

double BoilerModels::asDouble(const QString &p) {
    QString num = p;
    num = num.replace(',','.');
    return num.toDouble();
}


BoilerModels::BoilerModels()
    : m_waterBoilerHash(0),
      m_steamBoilerHash(0),
      m_scumHash(0),
      m_waterBoilerModel(0),
      m_steamBoilerModel(0),
      m_scumModel(0),
      m_heatModel(0)
{
    init();
}


BoilerModels::~BoilerModels()
{

    WaterBoiler *wBoiler = 0;
    QStringList wk = m_waterBoilerHash->keys();
    for( int i = 0; i < wk.size(); i++) {
        wBoiler = m_waterBoilerHash->take(wk.at(i));
        delete wBoiler;
    }
    m_waterBoilerHash->clear();
    delete m_waterBoilerHash;
    m_waterBoilerHash = 0;

    SteamBoiler *sBoiler = 0;
    QStringList sk = m_steamBoilerHash->keys();
    for( int i = 0; i < sk.size(); i++) {
        sBoiler = m_steamBoilerHash->take(sk.at(i));
        delete sBoiler;
    }
    m_steamBoilerHash->clear();
    delete m_steamBoilerHash;
    m_steamBoilerHash = 0;

    Scum *scum = 0;
    QStringList keys = m_scumHash->keys();
    for( int i = 0; i < keys.size(); i++) {
        scum = m_scumHash->take(keys.at(i));
        delete scum;
    }
    m_scumHash->clear();
    delete m_scumHash;
    m_scumHash = 0;

    delete m_waterBoilerModel;
    delete m_steamBoilerModel;
    delete m_scumModel;

    delete m_heatModel;
}

Boiler *BoilerModels::boiler(const QString &name) const
{
    Boiler *boiler = waterBoiler(name);
    if (boiler == 0 )
        boiler = steamBoiler(name);
    return boiler;
}

WaterBoiler *BoilerModels::waterBoiler(const QString &name) const
{
    WaterBoiler *boiler = 0;
    if ( m_waterBoilerHash->contains(name) )
        boiler = m_waterBoilerHash->value(name);

    return boiler;
}

SteamBoiler *BoilerModels::steamBoiler(const QString &name) const
{
    SteamBoiler *boiler = 0;
    if ( m_steamBoilerHash->contains(name) )
        boiler = m_steamBoilerHash->value(name);

    return boiler;
}

Scum *BoilerModels::scum(const QString &name) const
{
    Scum *scum = 0;
    if ( m_scumHash->contains(name) )
        scum = m_scumHash->value(name);

    return scum;
}

void BoilerModels::init()
{
    initWaterModel();
    initSteamModel();
    initScumModel();
    initHeatModel();
}

void BoilerModels::initWaterModel()
{
    QFile file(":/calculate/reference/waterboiler.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    m_waterBoilerHash = new QHash<QString,WaterBoiler*>();
    m_waterBoilerModel = new QStandardItemModel(lines.size(), 5);
    m_waterBoilerModel->setHeaderData(0, Qt::Horizontal, "Марка");
    m_waterBoilerModel->setHeaderData(1, Qt::Horizontal, "Производительность");
    m_waterBoilerModel->setHeaderData(2, Qt::Horizontal, "Давление");
    m_waterBoilerModel->setHeaderData(3, Qt::Horizontal, "Водяной объем");
    m_waterBoilerModel->setHeaderData(4, Qt::Horizontal, "Топливо");

    WaterBoiler *boiler = 0;

    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 5) continue;

        QString mark        = vars.at(0);
        double perfomance   = asDouble(vars.at(1));
        double pressure     = asDouble(vars.at(2));
        double waterVolume  = asDouble(vars.at(3));
        int fuel            = vars.at(4).toInt();

        boiler = new WaterBoiler(mark, perfomance, pressure, waterVolume, fuel);
        m_waterBoilerHash->insert(mark, boiler);

        m_waterBoilerModel->setData(m_waterBoilerModel->index(i, 0), mark);
        m_waterBoilerModel->setData(m_waterBoilerModel->index(i, 1), perfomance);
        m_waterBoilerModel->setData(m_waterBoilerModel->index(i, 2), pressure);
        m_waterBoilerModel->setData(m_waterBoilerModel->index(i, 3), waterVolume);
        m_waterBoilerModel->setData(m_waterBoilerModel->index(i, 4), Boiler::fuelTitle(fuel).join(","));
    }
}

void BoilerModels::initSteamModel()
{
    QFile file(":/calculate/reference/steamboiler.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    m_steamBoilerHash = new QHash<QString,SteamBoiler*>();
    m_steamBoilerModel = new QStandardItemModel(lines.size(), 6);

    m_steamBoilerModel->setHeaderData(0, Qt::Horizontal, "Марка");
    m_steamBoilerModel->setHeaderData(1, Qt::Horizontal, "Производительность");
    m_steamBoilerModel->setHeaderData(2, Qt::Horizontal, "Давление");
    m_steamBoilerModel->setHeaderData(3, Qt::Horizontal, "Водяной объем");
    m_steamBoilerModel->setHeaderData(4, Qt::Horizontal, "Паровой объем");
    m_steamBoilerModel->setHeaderData(5, Qt::Horizontal, "Топливо");

    SteamBoiler *boiler = 0;

    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 6) continue;

        QString mark        = vars.at(0);
        double perfomance   = asDouble(vars.at(1));
        double pressure     = asDouble(vars.at(2));
        double waterVolume  = asDouble(vars.at(3));
        double steamVolume  = asDouble(vars.at(4));
        int fuel = vars.at(5).toInt();

        boiler = new SteamBoiler(mark, perfomance, pressure, waterVolume, steamVolume, fuel);

        m_steamBoilerHash->insert(mark, boiler);

        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 0),  mark);
        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 1),  perfomance);
        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 2),  pressure);
        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 3),  waterVolume);
        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 4),  steamVolume);
        m_steamBoilerModel->setData(m_steamBoilerModel->index(i, 5),  Boiler::fuelTitle(fuel).join(","));

    }
}

void BoilerModels::initScumModel()
{
    QFile file(":/calculate/reference/scumkind.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    m_scumHash = new QHash<QString,Scum*>();
    m_scumModel = new QStandardItemModel(lines.size(), 1);
    m_scumModel->setHeaderData(0, Qt::Horizontal, "Вид накипи");

    Scum *scum = 0;
    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 3) continue;

        QString kind    = vars.at(0);
        qreal down   = asDouble(vars.at(1));
        qreal up     = asDouble(vars.at(2));

        scum = new Scum(kind, down, up);
        m_scumHash->insert(kind, scum);

        m_scumModel->setData(m_scumModel->index(i, 0), kind);
    }
}

void BoilerModels::initHeatModel()
{
    QFile file(":/calculate/reference/heating-period.txt");
    if (!file.open(QFile::ReadOnly))
        return ;

    m_heatModel = new QStandardItemModel();
    QVector<QStandardItem *> parents(10);
    parents[0] = m_heatModel->invisibleRootItem();

    QStandardItem *item = 0;

    while (!file.atEnd()) {
        QString line = file.readLine();
        QString trimmedLine = line.trimmed();
        if (line.isEmpty() || trimmedLine.isEmpty())
            continue;

        QRegExp re("^\\s+");
        int nonws = re.indexIn(line);
        int level = 0;
        if (nonws == -1) {
            level = 0;
        } else {
            if (line.startsWith("\t")) {
                level = re.cap(0).length();
            } else {
                level = re.cap(0).length()/4;
            }
        }

        if (level+1 >= parents.size())
            parents.resize(parents.size()*2);

        item = new QStandardItem();

        QStringList a = trimmedLine.split("\t");
        if ( a.at(a.size()-1).toInt() > 0 ) {
            item->setText(a.at(a.size()-2));
            item->setData(a.at(a.size()-1));
        } else {
            item->setText(trimmedLine);
        }

        parents[level]->appendRow(item);
        parents[level+1] = item;
    }
}


}
