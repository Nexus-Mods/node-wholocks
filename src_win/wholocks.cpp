#include "impl.h"

#include "string_cast.h"

#include <nan.h>

using namespace Nan;
using namespace v8;

Local<Object> convert(const Local<Context> &context, const Process &proc) {
  Local<Object> result = New<Object>();
  result->Set(context, New<String>("appName").ToLocalChecked(),
              New<String>(toMB(proc.appName.c_str(), CodePage::UTF8, proc.appName.length()).c_str()).ToLocalChecked());
  result->Set(context, Nan::New<v8::String>("pid").ToLocalChecked(), New<Number>(proc.pid));
  return result;
}

NAN_METHOD(wholocks) {
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Expected 1 arguments: The file path to check");
    return;
  }

  Local<Context> context = Nan::GetCurrentContext();
  Isolate *isolate = context->GetIsolate();

  String::Utf8Value path(isolate, info[0]);

  try {
    std::vector<Process> processes = WhoIsLocking(toWC(*path, CodePage::UTF8, path.length()));
    v8::Local<v8::Array> retValue = Nan::New<v8::Array>();
    for (int i = 0; i < processes.size(); ++i) {
      retValue->Set(context, i, convert(context, processes[i]));
    }
    info.GetReturnValue().Set(retValue);
  }
  catch (const std::exception &err) {
    Nan::ThrowError(err.what());
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("wholocks").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(wholocks)).ToLocalChecked());
}

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(wholocks, Init)
#else
NODE_MODULE(wholocks, Init)
#endif
