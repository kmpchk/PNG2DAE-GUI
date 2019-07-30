#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include <thread>
#include <QSysInfo>

void prepareProgramData(const std::shared_ptr<PROGRAM_DATA>& programData);

Q_GUI_EXPORT void qt_blurImage(QImage& blurImage, qreal radius, bool quality, int transposed = 0);

using namespace png2dae;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->showImageWindowCheckbox->setHidden(true);

    pProgramData = std::make_shared<PROGRAM_DATA>();

    //inputImageManipulatorPtr = new Magick::Image();

    //prepareProgramData(pProgramData);

    /*auto b = QSysInfo::buildCpuArchitecture();
    QMessageBox::information(this, "..", b);*/

    //this->setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );

    //setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowMinimizeButtonHint);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    ui->smoothingSlider->setEnabled(false);
    ui->sharpnessSlider->setEnabled(false);
    ui->zAxisScaleEdit->setValidator(new QDoubleValidator(ZET_SCALE_COEFF_BOTTOM, ZET_SCALE_COEFF_TOP, 1, this));
    ui->dimensionScaleEdit->setValidator(new QDoubleValidator(DIMENSION_SCALE_COEFF_BOTTOM, DIMENSION_SCALE_COEFF_TOP, 1, this));
    ui->xAxisRotateEdit->setValidator(new QDoubleValidator(X_ROTATE_ANGLE_BOTTOM, X_ROTATE_ANGLE_TOP, 1, this));

    connect(ui->smoothingSlider, SIGNAL(valueChanged(int)), SLOT(onSmoothingSliderValueChanged(int)));
    connect(ui->smoothingSpinBox, SIGNAL(valueChanged(double)), SLOT(onSmoothingSpinBoxValueChanged(double)));
    connect(ui->sharpnessSlider, SIGNAL(valueChanged(int)), SLOT(onSharpnessSliderValueChanged(int)));
    connect(ui->sharpnessSpinBox, SIGNAL(valueChanged(double)), SLOT(onSharpnessSpinBoxValueChanged(double)));
    connect(ui->sharpnessSlider, SIGNAL(sliderReleased()), SLOT(onSharpnessSliderReleased()));

    //progressBar.setRange(0, 0);
    //progressBar.hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(pExecutedProcess)
        delete pExecutedProcess;
}

void prepareProgramData(const std::shared_ptr<PROGRAM_DATA>& programData)
{
    programData->bGrayscaleOption = false;
    programData->bColorInverseOption = false;
    programData->bSharpnessOption = false;
    programData->bSmoothingOption = false;
}

void MainWindow::expandWindowSize()
{
    if(isWindowResized)
    {
        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    else
    {
        resize(RESIZED_WINDOW_WIDTH, WINDOW_HEIGHT);
        setFixedSize(RESIZED_WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    isWindowResized = !isWindowResized;
}

void MainWindow::updateImageView()
{
    /*Magick::Blob blob;
    inputImageManipulatorPtr.write(&blob);
    const QByteArray imageData((char*)blob.data(), blob.length());
    QPixmap pix;
    pix.loadFromData(imageData);
    ui->imageViewLabel->setPixmap(pix.scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT));*/
}

void MainWindow::setSmoothing(double value)
{
    //inputImageManipulatorPtr.blur(value);
    if(ui->smoothingSlider->value() == 0.0 && ui->sharpnessSlider->value() == 0.0)
    {
        changedAfterSharpnessImage = readImage;
        changedAfterSmoothingImage = readImage;
    }

    changedAfterSmoothingImage = changedAfterSharpnessImage;
    qt_blurImage(changedAfterSmoothingImage, value, true);
    ui->imageViewLabel->setPixmap(QPixmap::fromImage(changedAfterSmoothingImage).scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT));

}

void MainWindow::setSharpness(double value)
{
    //inputImageManipulatorPtr.sharpen(value);
    if(ui->smoothingSlider->value() == 0.0 && ui->sharpnessSlider->value() == 0.0)
    {
        changedAfterSharpnessImage = readImage;
        changedAfterSmoothingImage = readImage;
    }
    sharpnessValue = value;
    changedAfterSharpnessImage = changedAfterSmoothingImage;
    sharpenImage(changedAfterSharpnessImage, 1.0, sharpnessValue);
    ui->imageViewLabel->setPixmap(QPixmap::fromImage(changedAfterSharpnessImage).scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT));

}

void MainWindow::onSmoothingSliderValueChanged(int value)
{
    /*ui->smoothingSpinBox->setValue(value);
    setSmoothing(value);
    updateImageView();*/
    ui->smoothingSpinBox->setValue(value);
    setSmoothing(value);
}

void MainWindow::onSharpnessSliderReleased()
{
    /*changedAfterSharpnessImage = changedAfterSmoothingImage;
    sharpenImage(changedAfterSharpnessImage, 1.0, sharpnessValue);
    ui->imageViewLabel->setPixmap(QPixmap::fromImage(changedAfterSmoothingImage).scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT));
    */

    /*
    QByteArray arr = QByteArray::fromRawData((const char*)changedImage.bits(), changedImage.byteCount());
    testBlob = Magick::Blob(arr.data(), arr.length());
    inputImageManipulatorPtr.type(Magick::ImageType::TrueColorMatteType);
    inputImageManipulatorPtr.read(testBlob);
    inputImageManipulatorPtr.sharpen(sharpnessValue);

    Magick::Blob blob;
    //inputImageManipulatorPtr.type(Magick::ImageType::TrueColorMatteType);
    inputImageManipulatorPtr.write(&blob);
    //testImage = inputImageManipulatorPtr;
    //testBlob = blob;
    const QByteArray imageData((char*)blob.data(), blob.length());

    QPixmap pix;
    pix.loadFromData(imageData);
    ui->imageViewLabel->setPixmap(pix.scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT)); */
}

void MainWindow::onSharpnessSliderValueChanged(int value)
{
    /*ui->sharpnessSpinBox->setValue(value);
    setSharpness(value);
    updateImageView();*/
    ui->sharpnessSpinBox->setValue(value);
    setSharpness(value);
}

void MainWindow::onSharpnessSpinBoxValueChanged(double value)
{
    ui->sharpnessSlider->setValue(value);
    setSharpness(value);
}

void MainWindow::onSmoothingSpinBoxValueChanged(double value)
{
    ui->smoothingSlider->setValue(value);
    setSmoothing(value);
    //QMessageBox::information(this, "..", QString::number(value));
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
    qDebug() << "Grayscale option is presented: " << pProgramData->bGrayscaleOption;
    qDebug() << "Inverse-color option is presented: " << pProgramData->bColorInverseOption;
    qDebug() << "Smoothing option is presented: " << pProgramData->bSmoothingOption;
    qDebug() << "Smoothing option's coefficient: " << pProgramData->dSmoothingCoeff;
    qDebug() << "Sharpness option is presented: " << pProgramData->bSharpnessOption;
    qDebug() << "Sharpness option's coefficient: " << pProgramData->dSharpnessCoeff;
    qDebug() << "Zet axis coeff: " << pProgramData->dZetAxisCoeff;
    qDebug() << "X axis angle: " << pProgramData->dXAxisRotateAngle;
    qDebug() << "Dimension coeff: " << pProgramData->dDimensionCoeff;
}

bool MainWindow::isRequiredFieldsFilled()
{
    bool isInputImageSelected = !inputImageFilename.isEmpty();
    bool isOutputModelFilenamePresented = !outputModelFilename.isEmpty();
    bool isOutputFolderPathPresented = !outputFolderPath.isEmpty();

    return isInputImageSelected && isOutputFolderPathPresented && isOutputModelFilenamePresented;
}

void MainWindow::fillProgramDataStructure()
{
    outputModelFilename = ui->outputFilenameEdit->text();
    if(outputModelFilename.isNull() || outputModelFilename.isEmpty())
        return;
    pProgramData->inputImageFilename = QFileInfo(INPUT_IMAGE_PREPARED_NAME).absoluteFilePath();
    pProgramData->outputFolderPath = outputFolderPath;
    pProgramData->outputModelFilename = outputModelFilename;
    pProgramData->bSmoothingOption = isSmoothingOption;
    if(ui->smoothingOptionCheckbox->isChecked())
        pProgramData->dSmoothingCoeff = ui->smoothingSpinBox->value();
    pProgramData->bSharpnessOption = isSharpnessOption;
    if(ui->sharpnessOptionCheckbox->isChecked())
        pProgramData->dSharpnessCoeff = ui->sharpnessSpinBox->value();
    pProgramData->bGrayscaleOption = isGrayscaleOption;
    pProgramData->bColorInverseOption = isColorInverseOption;
    if(ui->zAxisScaleLabel->isChecked() && !ui->zAxisScaleEdit->text().isEmpty())
        pProgramData->dZetAxisCoeff = ui->zAxisScaleEdit->text().toDouble();
    if(ui->xAxisRotateLabel->isChecked() && !ui->xAxisRotateEdit->text().isEmpty())
        pProgramData->dXAxisRotateAngle = ui->xAxisRotateEdit->text().toDouble();
    if(ui->dimensionScaleLabel->isChecked() && !ui->dimensionScaleEdit->text().isEmpty())
        pProgramData->dDimensionCoeff = ui->dimensionScaleEdit->text().toDouble();
}

void MainWindow::on_convertStartButton_clicked()
{
    if(!isRequiredFieldsFilled())
    {
        QMessageBox::critical(this, "A error message", "Not all required fields are filled!");
        return;
    }

    sharpnessValue = ui->sharpnessSpinBox->value();
    smoothingValue = ui->smoothingSpinBox->value();

    QImage image(readImage);
    qt_blurImage(image, smoothingValue, true);
    sharpenImage(image, 1.0, sharpnessValue);
    png2dae::saveImage(image, INPUT_IMAGE_PREPARED_NAME);

    fillProgramDataStructure();
    showParameters();

    /*QStringList args;
    args << "";
    startProcess("testApp", args);*/
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

void MainWindow::on_textureImageSelect_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a texture file", "/home");
    if(fileName.isNull() || fileName.isEmpty())
        return;

    inputTextureImageFilename = fileName;

    QMessageBox::information(this, "..", inputTextureImageFilename);
}

void MainWindow::on_selectImageButton_clicked()
{
    QString fileNamePath = QFileDialog::getOpenFileName(this, "Open a input file", "/home");
    if(fileNamePath.isNull() || fileNamePath.isEmpty())
        return;

    readImage.load(fileNamePath);
    /*inputImageManipulatorPtr.read(fileNamePath.toStdString());
    if(!inputImageManipulatorPtr.isValid())
    {
        QMessageBox::critical(this, "An error", "Invalid file read");
        return;
    }*/
    changedAfterSmoothingImage = readImage;
    changedAfterSharpnessImage = readImage;
    QFileInfo fi(fileNamePath);
    QString fileName = fi.fileName();
    QString fileNameWithoutExtension = fileName.left(fileName.lastIndexOf("."));
    ui->outputFilenameEdit->setText(fileNameWithoutExtension);
    outputModelFilename = fileNameWithoutExtension;

    inputImageFilename = fileNamePath;
    imageFileName = fileName;

    //QImage image(fileName);

    //ui->imageViewLabel->setFixedSize(readImage.width(), readImage.height());

    QMessageBox::information(this, "..", fileNamePath);
    QPixmap pix(fileNamePath);
    ui->imageViewLabel->setPixmap(pix.scaled(INPUT_IMAGE_VIEW_WIDTH, INPUT_IMAGE_VIEW_HEIGHT));

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

void MainWindow::on_smoothingOptionCheckbox_clicked()
{
    isSmoothingOption = !isSmoothingOption;
    ui->smoothingSlider->setEnabled(isSmoothingOption);
    ui->smoothingSpinBox->setReadOnly(!isSmoothingOption);
}

void MainWindow::on_sharpnessOptionCheckbox_clicked()
{
    isSharpnessOption = !isSharpnessOption;
    ui->sharpnessSlider->setEnabled(isSharpnessOption);
    ui->sharpnessSpinBox->setReadOnly(!isSharpnessOption);
}

void MainWindow::on_colorInverseOptionCheckbox_clicked()
{
    isGrayscaleOption = !isGrayscaleOption;
}

void MainWindow::on_grayscaleOptionCheckbox_clicked()
{
    isColorInverseOption = !isColorInverseOption;
}
