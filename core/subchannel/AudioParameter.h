#ifndef AUDIOPARAMETER_H
#define AUDIOPARAMETER_H


class AudioParameter
{
    public:
        AudioParameter();
        ~AudioParameter();

    private:
        float m_volumeMain;
        float m_volumePre;
        bool  m_playDirectionFwd;
        bool  m_isMuted;
};

#endif // AUDIOPARAMETER_H
