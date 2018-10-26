#include "SnowflakeWorker.h"
#include <chrono>

long SnowflakeWorker::timeNow()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return ms.count();
};

long SnowflakeWorker::nextMillis(long lastTimestamp)
{
    long timestamp = timeNow();
    while (timestamp <= lastTimestamp)
    {
        timestamp = timeNow();
    }
    return timestamp;
};

long SnowflakeWorker::nextId()
{
    long timestamp = timeNow();

    if (lastTimestamp == timestamp)
    {
        sequence = (sequence + 1) & sequenceMask;
        if (sequence == 0)
        {
            timestamp = nextMillis(lastTimestamp);
        }
    }
    else
    {
        sequence = 0;
    }

    lastTimestamp = timestamp;

    return ((timestamp - epoch) << timestampShift) | (datacenterId << datacenterIdShift) | (workerId << workerIdShift) | sequence;
};

Nan::Persistent<v8::FunctionTemplate> SnowflakeWorker::constructor;

NAN_MODULE_INIT(SnowflakeWorker::Init)
{
    v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(SnowflakeWorker::New);
    constructor.Reset(ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("SnowflakeWorker").ToLocalChecked());

    Nan::SetPrototypeMethod(ctor, "id", Id);

    target->Set(Nan::New("SnowflakeWorker").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(SnowflakeWorker::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowError(Nan::New("SnowflakeWorker: Called without 'new' keyword.").ToLocalChecked());
    }

    if (!info[0]->IsNumber() || !info[1]->IsNumber())
    {
        return Nan::ThrowError(Nan::New("SnowflakeWorker: Expected WorkerID and DatacenterID to be numbers.").ToLocalChecked());
    }

    SnowflakeWorker *worker = new SnowflakeWorker();

    worker->Wrap(info.Holder());

    long workerId = info[0]->NumberValue();
    long datacenterId = info[1]->NumberValue();

    if (workerId > worker->maxWorkerId || workerId < 0)
    {
        return Nan::ThrowError(Nan::New("SnowflakeWorker: WorkerID must be between 1 and " + std::to_string(worker->maxWorkerId)).ToLocalChecked());
    };

    if (datacenterId > worker->maxDatacenterId || datacenterId < 0)
    {
        return Nan::ThrowError(Nan::New("SnowflakeWorker: Datacenter must be between 1 and " + std::to_string(worker->maxDatacenterId)).ToLocalChecked());
    };

    worker->workerId = workerId;
    worker->datacenterId = datacenterId;

    info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(SnowflakeWorker::Id)
{
    SnowflakeWorker *self = Nan::ObjectWrap::Unwrap<SnowflakeWorker>(info.This());

    std::string idString = std::to_string(self->nextId());
    while (idString.length() < 19)
    {
        idString = "0" + idString;
    }
    v8::Local<v8::String> id = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), idString.c_str());

    info.GetReturnValue().Set(id);
}