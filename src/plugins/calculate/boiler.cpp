#include "boiler.h"
#include <QStringList>

namespace Calculate
{

Boiler::Boiler(QString mark,
                double perfomance,
                double pressure,
                double waterVolume,
                int fuel)
{
    m_mark = mark;
    m_perfomance = perfomance;
    m_pressure = pressure;
    m_waterVolume = waterVolume;
    m_fuel = fuel;
}

QStringList Boiler::fuelTitle(int num)
{
    /*
    coal = 2,   // уголь
    gas = 4,    // газ
    masut = 8,  // мазут
    liquid = 16 // жидкое
    */
    QStringList list;
    switch(num) {
    case coal:
        list.append("Уголь");
        break;
    case gas:
        list.append("Газ");
        break;
    case masut:
        list.append("Мазут");
        break;
    case liquid:
        list.append("Жидкое");
        break;
    case gas|masut:
        list.append("Газ");
        list.append("Мазут");
        break;
    case gas|liquid:
        list.append("Газ");
        list.append("Жидкое");
        break;
    case coal|gas:
        list.append("Уголь");
        list.append("Газ");
        break;
    case coal|gas|masut:
        list.append("Уголь");
        list.append("Газ");
        list.append("Мазут");
        break;
    }
    return list;
}


WaterBoiler::WaterBoiler(QString mark,
                          double perfomance,
                          double pressure,
                          double waterVolume,
                          int fuel)
    : Boiler(mark, perfomance, pressure, waterVolume, fuel)
{
}

SteamBoiler::SteamBoiler(QString mark,
                         double perfomance,
                         double pressure,
                         double waterVolume,
                         double steamVolume,
                         int fuel)
    : Boiler(mark, perfomance, pressure, waterVolume, fuel)
{
    m_steamVolume = steamVolume;
}

}

