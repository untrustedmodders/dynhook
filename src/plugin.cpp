#include <plugify/cpp_plugin.h>
#include <plugin_export.h>
#include <dynohook/imanager.h>

using namespace dyno;

#if DYNO_PLATFORM_WINDOWS
#include <dynohook/conventions/x64_windows_call.h>
#define DEFAULT_CALLCONV x64WindowsCall
#else
#include <dynohook/conventions/x64_systemV_call.h>
#define DEFAULT_CALLCONV x64SystemVcall
#endif

class DynoHookPlugin : public plugify::IPluginEntry {
	void OnPluginStart() override {
		//Log::registerLogger(std::make_shared<ErrorLogger>());
		IHookManager::Get(); // init singleton
	}

	void OnPluginEnd() override {
          IHookManager::Get().unhookAll();
	}
} g_dynoHookPlugin;

EXPOSE_PLUGIN(PLUGIN_API, &g_dynoHookPlugin)

extern "C"
PLUGIN_API IHookManager* GetManager() {
	return &IHookManager::Get();
}

extern "C"
PLUGIN_API IHook* HookDetour(void* pFunc, const std::vector<DataObject>& arguments, DataObject returnType) {
	return IHookManager::Get().hookDetour(pFunc, [&]() { return new DEFAULT_CALLCONV(arguments, returnType); }).get();
}

extern "C"
PLUGIN_API IHook* HookVirtual(void* pClass, int index, const std::vector<DataObject>& arguments, DataObject returnType) {
	return IHookManager::Get().hookVirtual(pClass, index, [&]() { return new DEFAULT_CALLCONV(arguments, returnType); }).get();
}

extern "C"
PLUGIN_API IHook* HookVirtualByFunc(void* pClass, void* pFunc, const std::vector<DataObject>& arguments, DataObject returnType) {
	return IHookManager::Get().hookVirtual(pClass, pFunc, [&]() { return new DEFAULT_CALLCONV(arguments, returnType); }).get();
}

extern "C"
PLUGIN_API bool UnhookDetour(void* pFunc) {
	return IHookManager::Get().unhookDetour(pFunc);
}

extern "C"
PLUGIN_API bool UnhookVirtual(void* pClass, int index) {
	return IHookManager::Get().unhookVirtual(pClass, index);
}

extern "C"
PLUGIN_API bool UnhookVirtualByFunc(void* pClass, void* pFunc) {
	return IHookManager::Get().unhookVirtual(pClass, pFunc);
}

extern "C"
PLUGIN_API IHook* FindDetour(void* pFunc) {
	return IHookManager::Get().findDetour(pFunc).get();
}

extern "C"
PLUGIN_API IHook* FindVirtual(void* pClass, int index) {
	return IHookManager::Get().findVirtual(pClass, index).get();
}

extern "C"
PLUGIN_API IHook* FindVirtualByFunc(void* pClass, void* pFunc) {
	return IHookManager::Get().findVirtual(pClass, pFunc).get();
}

extern "C"
PLUGIN_API void UnhookAll() {
	return IHookManager::Get().unhookAll();
}

extern "C"
PLUGIN_API void HookAllVirtual(void* pClass) {
	IHookManager::Get().unhookAllVirtual(pClass);
}

extern "C"
PLUGIN_API bool AddCallback(IHook* pHook, CallbackType type, CallbackHandler handler) {
	return pHook->addCallback(type, handler);
}

extern "C"
PLUGIN_API bool RemoveCallback(IHook* pHook, CallbackType type, CallbackHandler handler) {
	return pHook->addCallback(type, handler);
}

extern "C"
PLUGIN_API bool IsCallbackRegistered(IHook* pHook, CallbackType type, CallbackHandler handler) {
	return pHook->isCallbackRegistered(type, handler);
}

extern "C"
PLUGIN_API bool AreCallbacksRegistered(IHook* pHook) {
	return pHook->areCallbacksRegistered();
}

extern "C"
PLUGIN_API bool GetArgumentBool(IHook* pHook, size_t index) { return pHook->getArgument<bool>(index); }
extern "C"
PLUGIN_API int8_t GetArgumentInt8(IHook* pHook, size_t index) { return pHook->getArgument<int8_t>(index); }
extern "C"
PLUGIN_API uint8_t GetArgumentUInt8(IHook* pHook, size_t index) { return pHook->getArgument<uint8_t>(index); }
extern "C"
PLUGIN_API int16_t GetArgumentInt16(IHook* pHook, size_t index) { return pHook->getArgument<int16_t>(index); }
extern "C"
PLUGIN_API uint16_t GetArgumentUInt16(IHook* pHook, size_t index) { return pHook->getArgument<uint16_t>(index); }
extern "C"
PLUGIN_API int32_t GetArgumentInt32(IHook* pHook, size_t index) { return pHook->getArgument<int32_t>(index); }
extern "C"
PLUGIN_API uint32_t GetArgumentUInt32(IHook* pHook, size_t index) { return pHook->getArgument<uint32_t>(index); }
extern "C"
PLUGIN_API int64_t GetArgumentInt64(IHook* pHook, size_t index) { return pHook->getArgument<int64_t>(index); }
extern "C"
PLUGIN_API uint64_t GetArgumentUInt64(IHook* pHook, size_t index) { return pHook->getArgument<uint64_t>(index); }
extern "C"
PLUGIN_API float GetArgumentFloat(IHook* pHook, size_t index) { return pHook->getArgument<float>(index); }
extern "C"
PLUGIN_API double GetArgumentDouble(IHook* pHook, size_t index) { return pHook->getArgument<double>(index); }
extern "C"
PLUGIN_API void* GetArgumentPointer(IHook* pHook, size_t index) { return pHook->getArgument<void*>(index); }
extern "C"
PLUGIN_API const char* GetArgumentString(IHook* pHook, size_t index) { return pHook->getArgument<const char*>(index); }
extern "C"
PLUGIN_API const wchar_t* GetArgumentWString(IHook* pHook, size_t index) { return pHook->getArgument<const wchar_t*>(index); }

extern "C"
PLUGIN_API void SetArgumentBool(IHook* pHook, size_t index, bool value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentInt8(IHook* pHook, size_t index, int8_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentUInt8(IHook* pHook, size_t index, uint8_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentInt16(IHook* pHook, size_t index, int16_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentUInt16(IHook* pHook, size_t index, uint16_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentInt32(IHook* pHook, size_t index, int32_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentUInt32(IHook* pHook, size_t index, uint32_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentInt64(IHook* pHook, size_t index, int64_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentUInt64(IHook* pHook, size_t index, uint64_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentFloat(IHook* pHook, size_t index, float value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentDouble(IHook* pHook, size_t index, double value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentPointer(IHook* pHook, size_t index, void* value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentString(IHook* pHook, size_t index, const char* value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void SetArgumentWString(IHook* pHook, size_t index, const wchar_t* value) { return pHook->setArgument(index, value); }

extern "C"
PLUGIN_API bool GetReturnBool(IHook* pHook) { return pHook->getReturn<bool>(); }
extern "C"
PLUGIN_API int8_t GetReturnInt8(IHook* pHook) { return pHook->getReturn<int8_t>(); }
extern "C"
PLUGIN_API uint8_t GetReturnUInt8(IHook* pHook) { return pHook->getReturn<uint8_t>(); }
extern "C"
PLUGIN_API int16_t GetReturnInt16(IHook* pHook) { return pHook->getReturn<int16_t>(); }
extern "C"
PLUGIN_API uint16_t GetReturnUInt16(IHook* pHook) { return pHook->getReturn<uint16_t>(); }
extern "C"
PLUGIN_API int32_t GetReturnInt32(IHook* pHook) { return pHook->getReturn<int32_t>(); }
extern "C"
PLUGIN_API uint32_t GetReturnUInt32(IHook* pHook) { return pHook->getReturn<uint32_t>(); }
extern "C"
PLUGIN_API int64_t GetReturnInt64(IHook* pHook) { return pHook->getReturn<int64_t>(); }
extern "C"
PLUGIN_API uint64_t GetReturnUInt64(IHook* pHook) { return pHook->getReturn<uint64_t>(); }
extern "C"
PLUGIN_API float GetReturnFloat(IHook* pHook) { return pHook->getReturn<float>(); }
extern "C"
PLUGIN_API double GetReturnDouble(IHook* pHook) { return pHook->getReturn<double>(); }
extern "C"
PLUGIN_API void* GetReturnPointer(IHook* pHook) { return pHook->getReturn<void*>(); }
extern "C"
PLUGIN_API const char* GetReturnString(IHook* pHook) { return pHook->getReturn<const char*>(); }
extern "C"
PLUGIN_API const wchar_t* GetReturnWString(IHook* pHook) { return pHook->getReturn<const wchar_t*>(); }

extern "C"
PLUGIN_API void SetReturnBool(IHook* pHook, bool value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnInt8(IHook* pHook, int8_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnUInt8(IHook* pHook, uint8_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnInt16(IHook* pHook, int16_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnUInt16(IHook* pHook, uint16_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnInt32(IHook* pHook, int32_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnUInt32(IHook* pHook, uint32_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnInt64(IHook* pHook, int64_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnUInt64(IHook* pHook, uint64_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnFloat(IHook* pHook, float value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnDouble(IHook* pHook, double value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnPointer(IHook* pHook, void* value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnString(IHook* pHook, const char* value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void SetReturnWString(IHook* pHook, const wchar_t* value) { return pHook->setReturn(value); }

extern "C"
PLUGIN_API DataObject CreateDataObject(DataType type, RegisterType reg, uint16_t size) { return DataObject(type, reg, size); }