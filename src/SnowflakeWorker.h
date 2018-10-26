#include <nan.h>

class SnowflakeWorker : public Nan::ObjectWrap
{
  private:
    long workerIdBits = 5L;
    long datacenterIdBits = 5L;
    long maxWorkerId = -1L ^ (-1L << workerIdBits);
    long maxDatacenterId = -1L ^ (-1L << datacenterIdBits);
    long sequenceBits = 12L;

    long workerIdShift = sequenceBits;
    long datacenterIdShift = sequenceBits + workerIdBits;
    long timestampShift = sequenceBits + workerIdBits + datacenterIdBits;
    long sequenceMask = -1L ^ (-1 << sequenceBits);

    long lastTimestamp = -1L;

    long epoch = 1540136120000L;

    long timeNow();
    long nextMillis(long lastTimestamp);
    long nextId();

  public:
    long workerId;
    long datacenterId;
    long sequence;

    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(New);

    static NAN_METHOD(Id);

    static Nan::Persistent<v8::FunctionTemplate> constructor;
};