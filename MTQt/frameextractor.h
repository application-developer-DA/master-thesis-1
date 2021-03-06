#pragma once

#include "mainwidget.h"
#include "libav.h"

#include <QString>
#include <QTimerEvent>

#include <queue>

class FrameExtractor : public QObject {
    Q_OBJECT

    struct FrameCallbackHandler {
        bool stopped;
        std::queue<const libav::AVFrame*> videoFrames;

        bool operator()(const libav::AVFrame& videoFrame, int /*index*/) {
            videoFrames.push(&videoFrame);
            return true;
        }

        bool operator()(const libav::AVSamples& /*samples*/ , int /*index*/) { return true; } // ignore audio samples
    };

public:
    FrameExtractor(MainWidget& frameReceiver, const char* filename)
        : m_frameReceiver(frameReceiver)
        , m_inputFile(filename)
        , m_fileStream(m_inputFile)
        , m_timerId(startTimer(1000 / 30))
    {
    }

    ~FrameExtractor()
    {
        killTimer(m_timerId); // Precaution
    }

private:
    virtual void timerEvent(QTimerEvent* timerEvent) {
        if (timerEvent->timerId() == m_timerId) {
            if (ProceedDecoding()) {
                m_frameReceiver.FeedFrame(m_callback.videoFrames.front());
                m_callback.videoFrames.pop();
            }
        }
    }

    bool ProceedDecoding() {
        bool decoded = true;
        while (m_callback.videoFrames.empty() && decoded)
            decoded = m_fileStream.Decode(m_callback, true);

        if (! decoded || m_callback.stopped) {
            killTimer(m_timerId);
            m_timerId = 0;
        }
        return decoded;
    }

private:
    MainWidget&           m_frameReceiver;
    libav::AVInputFile    m_inputFile;
    libav::AVStream       m_fileStream;
    FrameCallbackHandler  m_callback;
    int                   m_timerId;
};
