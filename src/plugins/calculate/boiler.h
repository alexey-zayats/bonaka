#ifndef BOILER_H
#define BOILER_H

#include <QString>

namespace Calculate {


class Boiler
{
    enum FuelType {
        coal = 2,   // уголь
        gas = 4,    // газ
        masut = 8,  // мазут
        liquid = 16 // жидкое
    };

public:
    Boiler(QString mark,
           double perfomance,
           double pressure,
           double waterVolume,
           int fuel);

    QString mark() { return m_mark; }
    double perfomance() { return m_perfomance; }
    double pressure() { return m_pressure; }
    double waterVolume() { return m_waterVolume; }
    int fuel() { return m_fuel; }

    static QStringList fuelTitle(int num);

private:
    QString m_mark;
    double m_perfomance;
    double m_pressure;
    double m_waterVolume;
    int m_fuel;

};

class WaterBoiler : public Boiler
{
public:
    WaterBoiler(QString mark,
                double perfomance,
                double pressure,
                double waterVolume,
                int fuel);
};

class SteamBoiler : public Boiler
{
public:
    SteamBoiler(QString mark,
                double perfomance,
                double pressure,
                double waterVolume,
                double steamVolume,
                int fuel);

    double steamVolume() { return m_steamVolume; }

private:
    double m_steamVolume;

};

}

#endif // BOILER_H
