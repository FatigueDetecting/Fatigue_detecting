#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
using namespace std;
using namespace cv;
#ifndef ns1970
#define ms1970 chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count()
#define us1970 chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count()
#define ns1970 chrono::time_point_cast<chrono::nanoseconds>(chrono::system_clock::now()).time_since_epoch().count()
#endif

template<class Object, int nbuf0 = 9, int nchn0 = 9, int ndim0 = 6> class CirArr
{
public:
    static CirArr& GetMe(int dim = 0) { static CirArr us[ndim0]; return us[dim]; }
    void help()
    {
        spdlog::info("1.Read faster than write: read waits until timeout");
        spdlog::info("2.Read slower than write: read skips intermediary buffers and goes directly to the latest buffer(bufCount should be enough large in case writePos returns to readPos)");
    }

public:
    enum State
    {
        Deinited,
        Inited
    };

protected:
    State state = Deinited;
    const int nbuf = nbuf0;
    const int nchn = nchn0;
    const int ndim = ndim0;

protected:
    Object objects[nbuf0];
    int64 readPos[nchn0] = { 0 };
    int64 writePos = 0;

public:
    State getState() { return state; }
    int getBuf() { return nbuf; }
    int getChn() { return nchn; }
    int getDim() { return ndim; }

public:
    virtual bool init()
    {
        if (state == Inited) { spdlog::info("zero operation"); return true; }
        memset(objects, 0, sizeof(objects));
        memset(readPos, 0, sizeof(readPos));
        memset(&writePos, 0, sizeof(writePos));
        state = Inited;
        return true;
    }
    virtual bool deinit()
    {
        if (state == Deinited) { spdlog::info("zero operation"); return true; }
        memset(objects, 0, sizeof(objects));
        memset(readPos, 0, sizeof(readPos));
        memset(&writePos, 0, sizeof(writePos));
        state = Deinited;
        return true;
    }

public:
    bool getLatest(Object** object, int chnId, int msTimeout = 1000, int msSleep = 2)
    {
        if (state != Inited) { spdlog::critical("wrong state"); return false; }
        for (int64 t0 = ms1970; ms1970 - t0 < msTimeout;)
        {
            int64 availablePos = writePos;
            if (availablePos > readPos[chnId])
            {
                int64 relativePos = availablePos % nbuf;
                *object = objects + relativePos;
                readPos[chnId] = availablePos;
                return true;
            }
            this_thread::sleep_for(chrono::milliseconds(msSleep));
        }
        return false;
    }
    int64 lockWritten(Object** object)
    {
        if (state != Inited) { spdlog::critical("wrong state"); return -1; }
        int64 absolutePos = writePos;
        int64 relativePos = ++absolutePos % nbuf;
        *object = objects + relativePos;
        return absolutePos;
    }
    int64 unlockWritten(int64 absolutePos)
    {
        if (state != Inited) { spdlog::critical("wrong state"); return -1; }
        return (writePos = absolutePos);
    }
};


template<class tp, int nbuf0 = 9, int nchn0 = 9, int ndim0 = 6> class CirMat : public CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>
{
public://Subclass cannot access parent members without following code in GCC
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::state;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::nbuf;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::nchn;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::ndim;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::objects;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::readPos;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::writePos;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::Deinited;
    using CirArr<Mat_<tp>, nbuf0, nchn0, ndim0>::Inited;

public:
    static CirMat& GetMe(int dim = 0) { static CirMat us[ndim0]; return us[dim]; }

public:
    bool init() { spdlog::critical("please call the other one"); return false; };
    bool init(int rows, int cols)
    {
        if (state == Inited) { spdlog::info("zero operation"); return true; }
        for (int k = 0; k < nbuf; ++k) { objects[k].create(rows, cols); objects[k] = 0; }
        memset(readPos, 0, sizeof(readPos));
        memset(&writePos, 0, sizeof(writePos));
        state = Inited;
        return true;
    }
    bool deinit()
    {
        if (state == Deinited) { spdlog::info("zero operation"); return true; }
        for (int k = 0; k < nbuf; ++k) objects[k].release();
        memset(readPos, 0, sizeof(readPos));
        memset(&writePos, 0, sizeof(writePos));
        state = Deinited;
        return true;
    }
};


class CirMatDemo
{
public:
    static void TestMe(int argc = 0, char** argv = 0)
    {
        //1.StartControl
        if (ControlThread::GetMe().start() != 0) spdlog::critical("ControlThread::GetMe().start failed");
        else spdlog::info("ControlThread::GetMe().start succeeded");

        //2.StopControl
        if (ControlThread::GetMe().stop() != 0) spdlog::critical("ControlThread::GetMe().stop failed");
        else spdlog::info("ControlThread::GetMe().stop succeeded");
    }

public:
    using CirMatUint8 = CirMat<uchar, 7>; //write is about ten times faster than read; set nbuf as 4/6/8/10/12/14 for showing different effect

public:
    class WriteThread
    {
    public:
        static WriteThread& GetMe() { static WriteThread me; return me; }

    public:
        enum State
        {
            Stopped,
            Started
        };

    protected:
        State state = Stopped;

    public:
        State getState() { return state; }

    public:
        bool start()
        {
            if (state == Started) { spdlog::info("zero operation"); return true; }
            if (CirMatUint8::GetMe().getState() != CirMatUint8::Inited) { spdlog::critical("CirMatUint8 not Inited"); return false; }
            state = Started;
            std::thread([this]()->void
                {
                    int k = 0;
                    while (1)
                    {
                        //Exit
                        if (state == Stopped) break;

                        //GetPointer
                        Mat_<uchar>* mat;
                        int64 absolutePos = CirMatUint8::GetMe().lockWritten(&mat);
                        if (absolutePos < 0) { spdlog::critical("CirMatUint8::GetMe().lockWritten failed"); continue; }

                        //OperateData
                        *mat = 0;
                        char num[20]; sprintf(num, "WritePos: %d", ++k);
                        putText(*mat, num, Point(mat->cols / 5, mat->rows >> 1), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 2, LINE_8, false);

                        //ReleasePointer
                        if (CirMatUint8::GetMe().unlockWritten(absolutePos) < 0) spdlog::critical("CirMatUint8::GetMe().unlockWritten failed");

                        //ControlFramerate
                        this_thread::sleep_for(50ms);
                    }
                }).detach();

                return true;
        }
        bool stop()
        {
            if (state == Stopped) { spdlog::info("zero operation"); return true; }
            state = Stopped;
            return true;
        }
    };


    class ReadThread
    {
    public:
        static ReadThread& GetMe() { static ReadThread me; return me; }

    public:
        enum State
        {
            Stopped,
            Started
        };

    protected:
        State state = Stopped;

    public:
        State getState() { return state; }

    public:
        bool start()
        {
            if (state == Started) { spdlog::info("zero operation"); return true; }
            if (CirMatUint8::GetMe().getState() != CirMatUint8::Inited) { spdlog::critical("CirMatUint8 not Inited"); return false; }
            state = Started;
            std::thread([this]()->void
                {
                    int k = 0;
                    while (1)
                    {
                        //Exit
                        if (state == Stopped) break;

                        //GetPointer
                        Mat_<uchar>* mat;
                        if (!CirMatUint8::GetMe().getLatest(&mat, 0, 100)) { spdlog::info("CirMatUint8::GetMe().getLatest failed"); continue; }

                        //OperateData
                        mat->rowRange(0, mat->rows / 3) = 255;
                        mat->rowRange(mat->rows * 2 / 3, mat->rows) = 255;
                        char num[20]; sprintf(num, "ReadPos: %d", ++k);
                        putText(*mat, num, Point(mat->cols * 0.6, mat->rows >> 1), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 2, LINE_8, false);

                        //ControlFramerate
                        this_thread::sleep_for(400ms);
                        cv::imshow("effect", *mat);
                        cv::waitKey(100);
                    }
                }).detach();

                return true;
        }
        bool stop()
        {
            if (state == Stopped) { spdlog::info("zero operation"); return true; }
            state = Stopped;
            return true;
        }
    };


    class ControlThread
    {
    public:
        static ControlThread& GetMe() { static ControlThread me; return me; }

    public:
        enum State
        {
            Stopped,
            BufferStarted,
            WriteStarted,
            ReadStarted,
            Started
        };

    protected:
        State state = Stopped;

    public:
        State getState() { return state; }

    public:
        bool start()
        {
            if (state > Stopped) { spdlog::info("zero operation"); return true; }

            //1.StartBuffer
            if (CirMatUint8::GetMe().init(720, 1280) == false)
            {
                spdlog::critical("CirMatUint8::GetMe().init failed");
                stop(); return false;
            }
            else state = State(state + 1);

            //2.StartWrite
            if (WriteThread::GetMe().start() == false)
            {
                spdlog::critical("WriteThread::GetMe().start failed");
                stop(); return false;
            }
            else state = State(state + 1);

            //3.StartRead
            if (ReadThread::GetMe().start() == false)
            {
                spdlog::critical("ReadThread::GetMe().start failed");
                stop(); return false;
            }
            else state = State(state + 1);

            //4.MainThread
            while (1)
            {
                static int sec = 60;
                if (sec == 0) break;
                spdlog::info("This is the main thread and finish in {}s", --sec);
                this_thread::sleep_for(1000ms);
            }

            return true;
        }
        bool stop()
        {
            bool ret = true;

            //StopRead
            if (state > ReadStarted)
            {
                if (ReadThread::GetMe().stop() != 0)
                {
                    spdlog::critical("ReadThread::GetMe().stop failed");
                    ret = false;
                }
                state = State(state - 1);
            }

            //StopRead
            if (state > WriteStarted)
            {
                if (WriteThread::GetMe().stop() != 0)
                {
                    spdlog::critical("WriteThread::GetMe().stop failed");
                    ret = false;
                }
                state = State(state - 1);
            }

            //StopBuffer
            if (state > BufferStarted)
            {
                if (CirMatUint8::GetMe().deinit() != 0)
                {
                    spdlog::critical("CirMatUint8::GetMe().deinit failed");
                    ret = false;
                }
                state = State(state - 1);
            }

            return ret;
        }
    };
};

int main(int argc = 0, char** argv = 0) { CirMatDemo::TestMe(argc, argv); return 0; }