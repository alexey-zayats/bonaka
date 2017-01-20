#ifndef BOILERWIDGET_H
#define BOILERWIDGET_H

#include <QWidget>
#include <QModelIndex>

class QVBoxLayout;
class QCompleter;
class QStandardItemModel;
class QComboBox;
class QTreeView;
class QStackedWidget;
class QLineEdit;
class QAbstractItemModel;
class QRadioButton;
class QGridLayout;
class QPushButton;

namespace Calculate {

class Scum;
class Boiler;
class WaterBoiler;
class SteamBoiler;
class TreeModelCompleter;

class BoilerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoilerWidget(QWidget *parent = 0);
    ~BoilerWidget();

signals:

public slots:
    void boilerTypeChanged(int);
    void boilerKindChanged(int);
    void setYearDays(const QString &text);
    void setScumSpace(const int idx);
    void claculateClicked();

private:
    void initCompleters();
    void initSource();
    void initMathView();
    void initSteamBoiler();
    void initWaterBoiler();
    void initScumRef();

    double asDouble(const QString &p);

    QStringList fuelTitle( int num );
    void setTreeHeader(QTreeView *treeView);

    QAbstractItemModel *treeModelFromFile(const QString& fileName, TreeModelCompleter*);

    QHash<QString,WaterBoiler*> *waterBoilers;
    QHash<QString,SteamBoiler*> *steamBoilers;
    QHash<QString,Scum*> *scumHash;

    QCompleter *waterCompleter;
    QCompleter *steamCompleter;
    QCompleter *scumCompleter;

    QStandardItemModel *waterModel;
    QStandardItemModel *steamModel;
    QStandardItemModel *hpModel;
    QStandardItemModel *scumModel;

    QComboBox *boilerCombo;
    QComboBox *fuelCombo;
    QComboBox *scumCombo;

    int boilerType;
    int boilerKind;

    TreeModelCompleter *hpCompleter;

    QRadioButton *waterRadio;
    QRadioButton *steamRadio;

    int baseLayoutRow;
    QGridLayout *baseLayout;

    QPushButton *calculateButton;

    QWidget *selectorView, *mathWidget;

};

}

#endif // BOILERWIDGET_H
