#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>
#include <QPixmap>

#include </usr/include/sndfile.h>


typedef struct{
    QString  pathAndName;
    QString  path;
    QString  name;
    QPixmap  *pixmap;
    QPixmap  *miniWaveform;

    SF_INFO  sndInfo;
    SNDFILE *sndFile;
    float 	*frameBuffer;
    float   *viewFrameBuffer;
    float    lengthInMs;
    bool     isLoaded;
} Sample_t;





typedef struct{
    float left;
    float right;
} TwoChannelFrame_t;


typedef struct{
    float preLeft;
    float preRight;
    float mainLeft;
    float mainRight;
} FourChannelFrame_t;




class Sample
{
    public:
        Sample(QString samplePathAndName);
        ~Sample();

        Sample_t *getSampleStructPointer(void);

    private:
        Sample_t *m_sample;
        float m_peak;
        QString   m_samplePathAndName;
        float getMaxSampleValue();
        float getMaxSumValue();
};

#endif // SAMPLE_H
