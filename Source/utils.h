#ifndef UTILS_H
#define UTILS_H

#include <QImage>

namespace png2dae
{
    void separableGaussianBlurImage(QImage& image, double sigma);
    void secondDerivImage(QImage& image, double sigma);
    void sharpenImage(QImage &image, double sigma, double alpha);

    void saveImage(const QImage& image, const QString& fileName);
    void readImage(QImage& image);
}

#endif // UTILS_H
