#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QProcess>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include <thread>
#include <QtConcurrent/QtConcurrentRun>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showImageWindowCheckbox->setHidden(true);

    pProgramData = std::make_shared<PROGRAM_DATA>();

    this->setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );

    //setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowMinimizeButtonHint);

    setFixedSize(defWindowWidth, defWindowHeight);

    //progressBar.setRange(0, 0);
    //progressBar.hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(pExecutedProcess)
        delete pExecutedProcess;
}

void MainWindow::expandWindowSize()
{
    if(isWindowResized)
    {
        resize(defWindowWidth, defWindowHeight);
        setFixedSize(defWindowWidth, defWindowHeight);
    }
    else
    {
        resize(resizedWindowWidth, defWindowHeight);
        setFixedSize(resizedWindowWidth, defWindowHeight);
    }
    isWindowResized = !isWindowResized;
}

void MainWindow::onProcessFinished(int code)
{
    buffer = pExecutedProcess->readAllStandardOutput();
    qDebug() << buffer;

    ui->convertStatusProgressBar->setMaximum(1);
    ui->convertStatusProgressBar->setMinimum(0);

    ui->convertStartButton->setEnabled(true);

    pExecutedProcess->close();
    delete pExecutedProcess;
}

void MainWindow::onProcessStarted()
{
    //QMessageBox::information(this, "..", "App started");

    ui->convertStartButton->setEnabled(false);
}

void MainWindow::onReadOutput()
{
    //QString appStandartOutput = process.readAllStandardOutput();
    //QMessageBox::information(this, "..", appStandartOutput);

    while(pExecutedProcess->canReadLine())
    {
        qDebug() << pExecutedProcess->readLine();
    }
}

void MainWindow::startProcess(QString app, QStringList args)
{
    pExecutedProcess = new QProcess();

    ui->convertStatusProgressBar->setMaximum(0);
    ui->convertStatusProgressBar->setMinimum(0);

    connect(pExecutedProcess, SIGNAL(readyReadStandardOutput()), SLOT(onReadOutput()));
    connect(pExecutedProcess, SIGNAL(finished(int)), SLOT(onProcessFinished(int)));
    connect(pExecutedProcess, SIGNAL(started()), SLOT(onProcessStarted()));

    pExecutedProcess->start(app, args);
    qDebug() << "Process started.";
}

void MainWindow::showParameters()
{
    qDebug() << "Selected convert mode: " << pProgramData->convertMode;
    qDebug() << "Input image filename: " << pProgramData->inputImageFilename;
    qDebug() << "Output model filename: " << pProgramData->outputModelFilename;
    qDebug() << "Output folder path: " << pProgramData->outputFolderPath;
    qDebug() << "Grayscale option is presented: " << pProgramData->bGreyscaleOption;
    qDebug() << "Inverse-color is option presented: " << pProgramData->bColorInverseOption;
    qDebug() << "Zet axis coeff: " << pProgramData->dZetAxisCoeff;
    qDebug() << "X axis angle: " << pProgramData->dXAxisRotateAngle;
    qDebug() << "Dimension coeff: " << pProgramData->dDimensionCoeff;
}

void MainWindow::fillProgramDataStructure()
{
    outputModelFilename = ui->outputFilenameEdit->text();
    if(outputModelFilename.isNull() || outputModelFilename.isEmpty())
        return;
    pProgramData->inputImageFilename = inputImageFilename;
    pProgramData->outputFolderPath = outputFolderPath;
    pProgramData->outputModelFilename = outputModelFilename;
    if(ui->grayscaleOptionCheckbox->isChecked())
        pProgramData->bGreyscaleOption = true;
    if(ui->colorInverseOptionCheckbox->isChecked())
        pProgramData->bColorInverseOption = true;
    if(ui->zAxisScaleLabel->isChecked() && !ui->zAxisScaleEdit->text().isEmpty())
        pProgramData->dZetAxisCoeff = ui->zAxisScaleEdit->text().toDouble();
    if(ui->xAxisRotateLabel->isChecked() && !ui->xAxisRotateEdit->text().isEmpty())
        pProgramData->dXAxisRotateAngle = ui->xAxisRotateEdit->text().toDouble();
    if(ui->dimensionScaleLabel->isChecked() && !ui->dimensionScaleEdit->text().isEmpty())
        pProgramData->dDimensionCoeff = ui->dimensionScaleEdit->text().toDouble();
}

void MainWindow::on_convertStartButton_clicked()
{
    fillProgramDataStructure();
    showParameters();

    QStringList args;
    args << "";
    startProcess("testApp", args);
}

void test()
{
    QStringList list;
    list << "check";
    list << "the";
    list << "mond";

    foreach(const auto& str, list)
    {
        qDebug() << str << "\n";
    }
}

void MainWindow::on_selectImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "/home");
    if(fileName.isNull() || fileName.isEmpty())
        return;

    inputImageFilename = fileName;

    //QImage image(fileName);

    QMessageBox::information(this, "..", fileName);
    QPixmap pix(fileName);
    ui->imageViewLabel->setPixmap(pix.scaled(211, 221));

    if(!isWindowResized)
    {
        ui->showImageWindowCheckbox->setHidden(false);
        ui->showImageWindowCheckbox->click();
    }
}

void MainWindow::on_showImageWindowCheckbox_clicked()
{
    expandWindowSize();
}

void MainWindow::on_zAxisScaleLabel_clicked()
{
    isZFieldReadOnly = !isZFieldReadOnly;
    ui->zAxisScaleEdit->setReadOnly(isZFieldReadOnly);
}

void MainWindow::on_dimensionScaleLabel_clicked()
{
    isDimensionFieldReadOnly = !isDimensionFieldReadOnly;
    ui->dimensionScaleEdit->setReadOnly(isDimensionFieldReadOnly);
}

void MainWindow::on_xAxisRotateLabel_clicked()
{
    isXRotateFieldReadOnly = !isXRotateFieldReadOnly;
    ui->xAxisRotateEdit->setReadOnly(isXRotateFieldReadOnly);
}

void MainWindow::on_outputFolderSelect_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open a directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    //QMessageBox::information(this, "..", dir);
    if(dir.isNull() || dir.isEmpty())
        return;
    outputFolderPath = dir;

    ui->outputFolderPathEdit->setText(dir);
}

void MainWindow::on_pngToDaeSelect_clicked()
{
    pProgramData->convertMode = 0;
}

void MainWindow::on_pngToStlSelect_clicked()
{
    pProgramData->convertMode = 1;
}

void MainWindow::on_stlToDaeSelect_clicked()
{
    pProgramData->convertMode = 2;
}
