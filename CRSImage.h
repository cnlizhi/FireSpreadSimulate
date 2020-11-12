//CRSImage.h

#ifndef _CRSIMAGE_H
#define _CRSIMAGE_H

#include <QImage>
#include <QString>

typedef unsigned char DN;
enum INTERLEAVE { BSQ, BIP, BIL };

class CRSImage
{
public:
    CRSImage();
    CRSImage(const CRSImage& rImage);
    ~CRSImage();
    bool OpenImage(QString path);
    void Information();
    void CloseImage();
    void Statistics();
    void Covariance();
    bool SaveAsFile(QString path);
    DN*** GetDN() const;
    int GetBands() const;
    int GetLines() const;
    int GetSamples() const;
protected:
    bool ReadMetaData(const char* c_path);
    bool InitBuffer();
    bool ReadImgData(QString path);
    double GetAverage(DN** band) const;
    double GetVariance(DN** band) const;
    double GetCovariance(DN** band_x, DN** band_y) const;
    int GetMaximum(DN** band) const;
    int GetMinimum(DN** band) const;
private:
    DN*** m_pppDN;
    int m_samples;
    int m_lines;
    int m_bands;
    INTERLEAVE m_interleave;
    short m_datatype;
};

#endif //_CREIMAGE_H
