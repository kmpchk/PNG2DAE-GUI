#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QProgressBar>
#include <QProgressDialog>
#include <memory>
#include "programdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // callbacks to serve view's clicking

    void on_convertStartButton_clicked();

    void on_selectImageButton_clicked();

    void on_showImageWindowCheckbox_clicked();

    void on_pngToDaeSelect_clicked();

    void on_zAxisScaleLabel_clicked();

    void on_dimensionScaleLabel_clicked();

    void on_xAxisRotateLabel_clicked();


    //define the callbacks to handle process states (Start, Finish and ReadyStandartOutput)

    void onProcessFinished(int);

    void onProcessStarted();

    void onReadOutput();

    void on_outputFolderSelect_clicked();

    void on_pngToStlSelect_clicked();

    void on_stlToDaeSelect_clicked();

    void on_textureImageSelect_clicked();

private:

    Ui::MainWindow *ui;
    bool isZFieldReadOnly = true;
    bool isDimensionFieldReadOnly = true;
    bool isXRotateFieldReadOnly = true;
    bool isWindowResized = false;
    int defWindowWidth = 281;
    int defWindowHeight = 448;
    int resizedWindowWidth = 500;
    QProcess* pExecutedProcess;
    QString buffer;
    std::shared_ptr<PROGRAM_DATA> pProgramData;
    QString inputImageFilename, outputModelFilename, outputFolderPath, inputTextureImageFilename;

    // define the methods to perform some acions
    void expandWindowSize();
    void progressBarThread();
    void startProcess(QString app, QStringList args);

    void fillProgramDataStructure();
    void showParameters();
};

#endif // MAINWINDOW_H
