#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <memory>
#include "programdata.h"
#include "thirdparty.h"
#include "utils.h"

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

    void on_outputFolderSelect_clicked();

    void on_pngToStlSelect_clicked();

    void on_stlToDaeSelect_clicked();

    void on_textureImageSelect_clicked();

    void on_smoothingOptionCheckbox_clicked();

    void on_sharpnessOptionCheckbox_clicked();

    void on_colorInverseOptionCheckbox_clicked();

    void on_grayscaleOptionCheckbox_clicked();


    //define the callbacks to handle process states (Start, Finish and ReadyStandartOutput)

    void onProcessFinished(int);

    void onProcessStarted();

    void onReadOutput();

    void onSmoothingSliderValueChanged(int);

    void onSmoothingSpinBoxValueChanged(double);

    void onSharpnessSliderValueChanged(int);

    void onSharpnessSpinBoxValueChanged(double);

    void onSharpnessSliderReleased();


private:

    typedef enum _INPUT_FILE_TYPE
    {
        STL, IMAGE
    } INPUT_FILE_TYPE;

    Ui::MainWindow *ui;
    bool isZFieldReadOnly = true;
    bool isDimensionFieldReadOnly = true;
    bool isXRotateFieldReadOnly = true;
    bool isWindowResized = false;
    bool isSmoothingOption = false;
    bool isSharpnessOption = false;
    bool isColorInverseOption = false;
    bool isGrayscaleOption = false;
    double sharpnessValue, smoothingValue;
    INPUT_FILE_TYPE inputFileType = INPUT_FILE_TYPE::IMAGE;
    QProcess* pExecutedProcess = nullptr;
    QString buffer;
    QImage readImage, changedAfterSmoothingImage, changedAfterSharpnessImage;
    std::shared_ptr<PROGRAM_DATA> pProgramData;
    QString inputDataFilename = "", outputModelFilename, outputFolderPath, inputTextureImageFilename;
    QString imageFileName;
    //Magick::Image inputImageManipulatorPtr;
    //Magick::Image testImage;
    //Magick::Blob testBlob;
    //std::shared_ptr<Magick::Image> inputImageManipulator;

    // define the methods to perform some acions
    void expandWindowSize();
    void progressBarThread();
    void startProcess(QString app, QStringList args);

    void fillProgramDataStructure();
    void showParameters();
    QStringList createArgs();
    bool isRequiredFieldsFilled();
    void setSmoothing(double value);
    void setSharpness(double value);
    void updateImageView();
    //void sharpenImage(QImage& image, double sigma, double alpha);
    //void secondDerivImage(QImage& image, double sigma);
    //void separableGaussianBlurImage(QImage& image, double sigma);
};

#endif // MAINWINDOW_H
