
#include <nan.h>
#include "SnowflakeWorker.h"

NAN_MODULE_INIT(InitModule)
{
    SnowflakeWorker::Init(target);
}

NODE_MODULE(myModule, InitModule);