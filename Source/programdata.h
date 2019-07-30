#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H

#include <stdint.h>
#include <QString>
#include <QMessageBox>

typedef struct _PROGRAM_DATA
{
    QString inputImageFilename;
    QString outputModelFilename;
    QString outputFolderPath = ".";
    QString inputTextureImageFilename = ".";
    uint8_t convertMode;
    bool bColorInverseOption = false;
    bool bGrayscaleOption = false;
    double dSmoothingCoeff = 0.0;
    bool bSmoothingOption = false;
    double dSharpnessCoeff = 0.0;
    bool bSharpnessOption = false;
    double dZetAxisCoeff = 0.25;
    double dDimensionCoeff = 0.05;
    double dXAxisRotateAngle = -90.0;

} PROGRAM_DATA, *PPROGRAM_DATA;

#endif // PROGRAMDATA_H
