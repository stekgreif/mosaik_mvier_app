#include "AlsaPcm.h"
#include <QDebug>
//#include "mosaik.h"
#include <math.h>
#include <QWidget>
#include "core/subchannel/SubchannelManager.h"
#include "core/subchannel/Sample.h"

/*
aplay- l

hw:X,Y
X: card number
Y: device number
*/

/// @todo 66 - add function for scanning and selecting audio cards
/// @todo 73 - make this class a singleton pattern

#define PI  3.141592654
#define TAU 6.283185307


AlsaPcm::AlsaPcm()
    : m_sampleRate(44100)
    , m_frameCnt(0)
    , m_barCnt(0)
    , m_pendingBpmValue(110)
    , m_framesPerStep(6000) // defines the bp: 12000=60
    , m_stepCnt(-1)  // so the first step is 0
{
    //m_sampleRate = 48000;


    /** set mosaik_hw_params **/

//if((err = snd_pcm_open (&m_pcmHandle, PCM_DEVICE , SND_PCM_STREAM_PLAYBACK, 0)) < 0 )
#if 0 // 2015-05-11 original -> works
    if((err = snd_pcm_open (&m_pcmHandle, PCM_DEVICE , SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) < 0 )
    {
        fprintf(stderr, "Cannot open audio device %s (%s).\n", PCM_DEVICE, snd_strerror(err));
        exit(1);
    }
#endif

    int err2;
    if((err = snd_pcm_open (&m_pcmHandle, PCM_DEVICE_3 , SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) < 0 )
    {
        if((err2 = snd_pcm_open (&m_pcmHandle, PCM_DEVICE_2 , SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC)) < 0 )
        {
            fprintf(stderr, "Cannot open audio device %s (%s) nor %s (%s).\n",
                    PCM_DEVICE_2, snd_strerror(err), PCM_DEVICE_3, snd_strerror(err2));
            exit(1);
        }
    }


    if((err = snd_pcm_hw_params_malloc (&m_hwParams)) < 0)
    {
        fprintf(stderr, "Cannot allocate hardware parameter structure (%s).\n", snd_strerror(err));
        exit(1);
    }

    if((err = snd_pcm_hw_params_any (m_pcmHandle, m_hwParams)) < 0)
    {
        fprintf(stderr, "Cannot initialize hardware parameter structure (%s).\n", snd_strerror(err));
        exit(1);
    }

    if((err = snd_pcm_hw_params_set_access (m_pcmHandle, m_hwParams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        fprintf(stderr, "Cannot set access type (%s).\n", snd_strerror(err));
        exit(1);
    }

    if((err = snd_pcm_hw_params_set_format (m_pcmHandle, m_hwParams, SND_PCM_FORMAT_FLOAT )) < 0)
    {
        fprintf(stderr, "Cannot set sample format (%s).\n", snd_strerror(err));
        exit(1);
    }

    if ((err = snd_pcm_hw_params_set_rate_near (m_pcmHandle, m_hwParams, &m_sampleRate, 0)) < 0)
    {
        fprintf (stderr, "Cannot set sample rate (%s)\n.", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_channels (m_pcmHandle, m_hwParams, CHANNELS)) < 0)
    {
        fprintf (stderr, "Cannot set channel count (%s).\n", snd_strerror (err));
        exit (1);
    }

    if (snd_pcm_hw_params_set_periods(m_pcmHandle, m_hwParams, NUM_OF_PERIODS, 0) < 0)
    {
        fprintf(stderr, "Error setting periods.\n");
        exit (1);
    }

    if (snd_pcm_hw_params_set_buffer_size(m_pcmHandle, m_hwParams, (FRAMES * NUM_OF_PERIODS)) < 0)
    {
        fprintf(stderr, "Error setting buffersize.\n");
        exit (1);
    }

    if ((err = snd_pcm_hw_params (m_pcmHandle, m_hwParams)) < 0)
    {
        fprintf (stderr, "XXX Cannot set parameters (%s).\n", snd_strerror (err));
        exit (1);
    }


    /** check hw param settings **/
    snd_pcm_uframes_t bufferSize;
    snd_pcm_hw_params_get_buffer_size(m_hwParams, &bufferSize);
    qDebug() <<Q_FUNC_INFO <<"bufferSize:" <<bufferSize;

    snd_pcm_uframes_t periodSize;
    snd_pcm_hw_params_get_period_size(m_hwParams, &periodSize, 0);
    qDebug() <<Q_FUNC_INFO <<"periodSize:" <<periodSize;

    unsigned int periods;
    snd_pcm_hw_params_get_periods(m_hwParams, &periods, 0);
    qDebug() <<Q_FUNC_INFO <<"periods:" <<periods;

    snd_pcm_hw_params_free(m_hwParams);

    qDebug() <<Q_FUNC_INFO <<"Hardware parameters initialized.";



    /** set sw_params **/
    if ((err = snd_pcm_sw_params_malloc (&m_swParams)) < 0)
    {
        fprintf (stderr, "Cannot allocate software parameters structure (%s).\n", snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_current (m_pcmHandle, m_swParams)) < 0)
    {
        fprintf (stderr, "Cannot initialize software parameters structure (%s).\n", snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_set_avail_min (m_pcmHandle, m_swParams, FRAMES)) < 0)
    {
        fprintf (stderr, "Cannot set minimum available count (%s).\n", snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_set_start_threshold (m_pcmHandle, m_swParams, 0U)) < 0)
    {
        fprintf (stderr, "Cannot set start mode (%s).\n", snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params (m_pcmHandle, m_swParams)) < 0)
    {
        fprintf (stderr, "Cannot set software parameters (%s).\n", snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_prepare (m_pcmHandle)) < 0)
    {
        fprintf (stderr, "Cannot prepare audio interface for use (%s).\n", snd_strerror (err));
        exit (1);
    }

    qDebug() << "Software parameters initialized.";

    /* start thread now! */
    terminateRequest = true;

#if 1
    /* init square buffer */
    int i = 0;
    for(i = 0; i < FRAMES/2; i++)
    {
        m_squareBuffer[i] = 1;
    }
    for(i = FRAMES/2 + 1; i < FRAMES; i++)
    {
        m_squareBuffer[i] = -1;
    }
#endif
}



AlsaPcm::~AlsaPcm()
{
    terminateRequest = false;
    //snd_pcm_drop(m_pcmHandle);
    snd_pcm_drain(m_pcmHandle);
    snd_pcm_close(m_pcmHandle);
    qDebug() <<"~" <<Q_FUNC_INFO;
}

quint32 AlsaPcm::getStepCounter() const
{
    return m_stepCnt;
}

int AlsaPcm::getNumberOfFrames() const
{
    return FRAMES;
}

void AlsaPcm::slot_bpmChanged(float bpm)
{
    m_pendingBpmValue = bpm;
    m_newBpmValue = true;
}





/** 4 CHANNEL AUDIO CALLBACK **/

int AlsaPcm::playbackCallback(snd_pcm_sframes_t nFrames)
{
    if( m_newBpmValue )
    {
        m_framesPerStep = (60 * m_sampleRate) / (4 * m_pendingBpmValue);
        m_newBpmValue = false;
    }
    /* collect all audio frames */
    for(int n = 0; n < FRAMES; n++)
    {
        if( (m_frameCnt % m_framesPerStep) == 0 )
        {
            m_stepCnt++;
            //qDebug() <<Q_FUNC_INFO <<"m_stepCnt" <<m_stepCnt;
            subchannelManager().setStepSequencerCounter( m_stepCnt );
        }

#if 0 // test function: sine
        float signalSine = 1. * sin( (TAU*1000.0* (float)m_frameCnt) / ((float)m_sampleRate) );
        m_pcmBuffer[n*4    ] = signalSine;
        m_pcmBuffer[n*4 + 1] = signalSine;
        m_pcmBuffer[n*4 + 2] = signalSine;
        m_pcmBuffer[n*4 + 3] = signalSine;
#endif

#if 1 // subchannel player
        FourChannelFrame_t fourChFrame;
        //fourChFrame = subchannelManager().getFourChannelAudioTestFrame();
        fourChFrame = subchannelManager().getFourChannelAudioFrame();

        m_pcmBuffer[n*4    ] = fourChFrame.mainLeft ;
        m_pcmBuffer[n*4 + 1] = fourChFrame.mainRight;
        m_pcmBuffer[n*4 + 2] = fourChFrame.preLeft  ;
        m_pcmBuffer[n*4 + 3] = fourChFrame.preRight ;
#endif
        m_frameCnt++;
    }

    /* send the actual audio data */
    if ((err = snd_pcm_writei(m_pcmHandle, m_pcmBuffer, nFrames)) < 0)
    {
        fprintf (stderr, "Write failes (%s).\n", snd_strerror (err));
    }

    return err;
}



void AlsaPcm::run()
{
    qDebug() << Q_FUNC_INFO <<"Audio is running now.";

    //snd_pcm_prepare(m_pcmHandle);

    while( terminateRequest )
    {
#if 1
        /* wait till the interface is ready for data, or 1 second has elapsed. */
        if ((err = snd_pcm_wait (m_pcmHandle, 1000)) < 0)
        {
            fprintf (stderr, "Poll failed (%s).\n", strerror (errno));
            break;
        }
#endif

        /* find out how much space is available for playback data */
        if ((m_framesToDeliver = snd_pcm_avail_update (m_pcmHandle)) < 0)
        {
            printf("Frames to deliver: %d \n", (int)m_framesToDeliver);
            if (m_framesToDeliver == -EPIPE)
            {
                fprintf (stderr, "An xrun occured.\n");
                break;
            }
            else
            {
                fprintf (stderr, "Unknown ALSA avail update return value (%d).\n", (int)m_framesToDeliver);
                break;
            }
        }

        // frames_to_deliver = frames_to_deliver > (BUFFER_SIZE/CHANNELS) ? (BUFFER_SIZE/CHANNELS) : frames_to_deliver;

        if (m_framesToDeliver > FRAMES)
        {
            m_framesToDeliver = FRAMES;
        }
        else
        {
            m_framesToDeliver = m_framesToDeliver;
        }

        /* deliver the data */
        if (playbackCallback (m_framesToDeliver) != m_framesToDeliver)
        {
            fprintf (stderr, "Playback callback failed.\n");
            break;
        }

    }
}
