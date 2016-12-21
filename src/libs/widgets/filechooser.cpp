#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QGridLayout>
#include <QDir>
#include <QFile>
#include <QApplication>

#include "filechooser.h"

namespace Widgets {

FileChooser::FileChooser(QWidget *parent) :
    QWidget(parent)
{
   init();
}

FileChooser::FileChooser(QWidget *parent,
                         const QString &caption,
                         const QString &directory,
                         const QString &filter) :
    QWidget(parent)
{
    init();

    setNameFilter(filter);
    setCaption(caption);
    setDirectory(directory);
}


void FileChooser::init()
{
    m_nameFilter = tr("*");
    m_caption = tr("Select file");
    m_directory = qApp->applicationDirPath();

    m_lineEdit = new QLineEdit();
    m_lineEdit->setDisabled(true);

    m_chooseFile = new QToolButton();
    m_chooseFile->setText(QLatin1String("..."));

    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_lineEdit, 0, 0);
    m_layout->addWidget(m_chooseFile, 0, 1);

    connect(m_chooseFile, SIGNAL(clicked(bool)), this, SLOT(chooseFileClicked()));

    setLayout(m_layout);
}

void FileChooser::chooseFileClicked()
{
    QFileDialog dialog(this, m_caption, m_directory, m_nameFilter);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if ( QDialog::Accepted == dialog.exec() ) {
        QStringList selectedFiles = dialog.selectedFiles();
        if (selectedFiles.size() == 1) {
            QFileInfo fileInfo(selectedFiles.at(0));
            if ( fileInfo.isReadable() ) {
                m_choosedFile = fileInfo.absoluteFilePath();
                m_lineEdit->setText(m_choosedFile);
                emit fileChoosed(m_choosedFile);
            }
        }
    }
}

void FileChooser::setCaption ( const QString &caption )
{
    if (!caption.isEmpty())
        m_caption = caption;
}

void FileChooser::setNameFilter ( const QString &nameFilter )
{
    if (!nameFilter.isEmpty())
        m_nameFilter = nameFilter;
}

void FileChooser::setDirectory ( const QString &directory )
{
    if(!directory.isEmpty() ) {
        QDir dir(directory);
        if (dir.isReadable() ) {
            m_directory = directory;
        }
    }
}

const QString FileChooser::file()
{
    return m_choosedFile;
}

}
