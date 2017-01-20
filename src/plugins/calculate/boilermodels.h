#ifndef REFMODELS_H
#define REFMODELS_H

#include <QString>
#include <QHash>

class QStandardItemModel;

namespace Calculate {

class Scum;
class Boiler;
class WaterBoiler;
class SteamBoiler;
class TreeModelCompleter;

class BoilerModels
{
public:
    BoilerModels();
    ~BoilerModels();

    static double asDouble(const QString &p);

    Scum *scum(const QString &name) const;
    Boiler *boiler(const QString &name) const;

    QStandardItemModel *waterBoilerModel() { return m_waterBoilerModel; }
    QStandardItemModel *steamBoilerModel() { return m_steamBoilerModel; }
    QStandardItemModel *scumModel() { return m_scumModel; }
    QStandardItemModel *heatModel() { return m_heatModel; }

private:
    void init();

    // models
    void initWaterModel();
    void initSteamModel();
    void initScumModel();
    void initHeatModel();

    WaterBoiler *waterBoiler(const QString &name) const;
    SteamBoiler *steamBoiler(const QString &name) const;

    QHash<QString,WaterBoiler*> *m_waterBoilerHash;
    QHash<QString,SteamBoiler*> *m_steamBoilerHash;
    QHash<QString,Scum*> *m_scumHash;

    QStandardItemModel *m_waterBoilerModel;
    QStandardItemModel *m_steamBoilerModel;
    QStandardItemModel *m_scumModel;

    QStandardItemModel *m_heatModel;
};

}

#endif // REFMODELS_H
