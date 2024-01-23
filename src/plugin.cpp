#include <wizard/cpp_plugin.h>
#include <plugin_export.h>
#include <dynohook/manager.h>

using namespace dyno;

#if DYNO_PLATFORM_WINDOWS
#include <dynohook/conventions/x64_windows_call.h>
#define DEFAULT_CALLCONV x64WindowsCall
#else
#include <dynohook/conventions/x64_systemV_call.h>
#define DEFAULT_CALLCONV x64SystemVcall
#endif

class DynoHookPlugin : public wizard::IPluginEntry {
	void OnPluginStart() override {
		HookManager::Get(); // init singleton
	}

	void OnPluginEnd() override {
		HookManager::Get().unhookAll();
	}
} g_dynoPlugin;

EXPOSE_PLUGIN(PLUGIN_API, &g_dynoPlugin)

extern "C"
PLUGIN_API HookManager* Dyno_GetManager() {
	return &HookManager::Get();
}

extern "C"
PLUGIN_API IHook* Dyno_HookDetour(void* pFunc, DataObject* arguments, int size, DataObject returnType) {
	return HookManager::Get().hookDetour(pFunc, [=]() { return new DEFAULT_CALLCONV(std::vector(arguments, arguments + size), returnType); }).get();
}

extern "C"
PLUGIN_API IHook* Dyno_HookVirtual(void* pClass, size_t index, DataObject* arguments, int size, DataObject returnType) {
	return HookManager::Get().hookVirtual(pClass, index, [=]() { return new DEFAULT_CALLCONV(std::vector(arguments, arguments + size), returnType); }).get();
}

extern "C"
PLUGIN_API bool Dyno_UnhookDetour(void* pFunc) {
	return HookManager::Get().unhookDetour(pFunc);
}

extern "C"
PLUGIN_API bool Dyno_UnhookVirtual(void* pClass, size_t index) {
	return HookManager::Get().unhookVirtual(pClass, index);
}

extern "C"
PLUGIN_API IHook* Dyno_FindDetour(void* pFunc) {
	return HookManager::Get().findDetour(pFunc).get();
}

extern "C"
PLUGIN_API IHook* Dyno_FindVirtual(void* pClass, size_t index){
	return HookManager::Get().findVirtual(pClass, index).get();
}

extern "C"
PLUGIN_API bool Dyno_AddCallback(IHook* pHook, bool type, CallbackHandler handler){
	return pHook->addCallback(static_cast<CallbackType>(type), handler);
}

extern "C"
PLUGIN_API bool Dyno_RemoveCallback(IHook* pHook, bool type, CallbackHandler handler) {
	return pHook->addCallback(static_cast<CallbackType>(type), handler);
}

extern "C"
PLUGIN_API bool Dyno_IsCallbackRegistered(IHook* pHook, bool type, CallbackHandler handler) {
	return pHook->isCallbackRegistered(static_cast<CallbackType>(type), handler);
}

extern "C"
PLUGIN_API bool Dyno_AreCallbacksRegistered(IHook* pHook) {
	return pHook->areCallbacksRegistered();
}

extern "C"
PLUGIN_API bool Dyno_GetArgumentBool(IHook* pHook, size_t index) { return pHook->getArgument<bool>(index); }
extern "C"
PLUGIN_API int8_t Dyno_GetArgumentInt8(IHook* pHook, size_t index) { return pHook->getArgument<int8_t>(index); }
extern "C"
PLUGIN_API uint8_t Dyno_GetArgumentUInt8(IHook* pHook, size_t index) { return pHook->getArgument<uint8_t>(index); }
extern "C"
PLUGIN_API int16_t Dyno_GetArgumentInt16(IHook* pHook, size_t index) { return pHook->getArgument<int16_t>(index); }
extern "C"
PLUGIN_API uint16_t Dyno_GetArgumentUInt16(IHook* pHook, size_t index) { return pHook->getArgument<uint16_t>(index); }
extern "C"
PLUGIN_API int32_t Dyno_GetArgumentInt32(IHook* pHook, size_t index) { return pHook->getArgument<int32_t>(index); }
extern "C"
PLUGIN_API uint32_t Dyno_GetArgumentUInt32(IHook* pHook, size_t index) { return pHook->getArgument<uint32_t>(index); }
extern "C"
PLUGIN_API int64_t Dyno_GetArgumentInt64(IHook* pHook, size_t index) { return pHook->getArgument<int64_t>(index); }
extern "C"
PLUGIN_API uint64_t Dyno_GetArgumentUInt64(IHook* pHook, size_t index) { return pHook->getArgument<uint64_t>(index); }
extern "C"
PLUGIN_API float Dyno_GetArgumentFloat(IHook* pHook, size_t index) { return pHook->getArgument<float>(index); }
extern "C"
PLUGIN_API double Dyno_GetArgumentDouble(IHook* pHook, size_t index) { return pHook->getArgument<double>(index); }
extern "C"
PLUGIN_API void* Dyno_GetArgumentPointer(IHook* pHook, size_t index) { return pHook->getArgument<void*>(index); }
extern "C"
PLUGIN_API const char* Dyno_GetArgumentString(IHook* pHook, size_t index) { return pHook->getArgument<const char*>(index); }

extern "C"
PLUGIN_API void Dyno_SetArgumentBool(IHook* pHook, size_t index, bool value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentInt8(IHook* pHook, size_t index, int8_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentUInt8(IHook* pHook, size_t index, uint8_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentInt16(IHook* pHook, size_t index, int16_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentUInt16(IHook* pHook, size_t index, uint16_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentInt32(IHook* pHook, size_t index, int32_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentUInt32(IHook* pHook, size_t index, uint32_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentInt64(IHook* pHook, size_t index, int64_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentUInt64(IHook* pHook, size_t index, uint64_t value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentFloat(IHook* pHook, size_t index, float value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentDouble(IHook* pHook, size_t index, double value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentPointer(IHook* pHook, size_t index, void* value) { return pHook->setArgument(index, value); }
extern "C"
PLUGIN_API void Dyno_SetArgumentString(IHook* pHook, size_t index, const char* value) { return pHook->setArgument(index, value); }

extern "C"
PLUGIN_API bool Dyno_GetReturnBool(IHook* pHook) { return pHook->getReturn<bool>(); }
extern "C"
PLUGIN_API int8_t Dyno_GetReturnInt8(IHook* pHook) { return pHook->getReturn<int8_t>(); }
extern "C"
PLUGIN_API uint8_t Dyno_GetReturnUInt8(IHook* pHook) { return pHook->getReturn<uint8_t>(); }
extern "C"
PLUGIN_API int16_t Dyno_GetReturnInt16(IHook* pHook) { return pHook->getReturn<int16_t>(); }
extern "C"
PLUGIN_API uint16_t Dyno_GetReturnUInt16(IHook* pHook) { return pHook->getReturn<uint16_t>(); }
extern "C"
PLUGIN_API int32_t Dyno_GetReturnInt32(IHook* pHook) { return pHook->getReturn<int32_t>(); }
extern "C"
PLUGIN_API uint32_t Dyno_GetReturnUInt32(IHook* pHook) { return pHook->getReturn<uint32_t>(); }
extern "C"
PLUGIN_API int64_t Dyno_GetReturnInt64(IHook* pHook) { return pHook->getReturn<int64_t>(); }
extern "C"
PLUGIN_API uint64_t Dyno_GetReturnUInt64(IHook* pHook) { return pHook->getReturn<uint64_t>(); }
extern "C"
PLUGIN_API float Dyno_GetReturnFloat(IHook* pHook) { return pHook->getReturn<float>(); }
extern "C"
PLUGIN_API double Dyno_GetReturnDouble(IHook* pHook) { return pHook->getReturn<double>(); }
extern "C"
PLUGIN_API void* Dyno_GetReturnPointer(IHook* pHook) { return pHook->getReturn<void*>(); }
extern "C"
PLUGIN_API const char* Dyno_GetReturnString(IHook* pHook) { return pHook->getReturn<const char*>(); }

extern "C"
PLUGIN_API void Dyno_SetReturnBool(IHook* pHook, bool value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnInt8(IHook* pHook, int8_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnUInt8(IHook* pHook, uint8_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnInt16(IHook* pHook, int16_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnUInt16(IHook* pHook, uint16_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnInt32(IHook* pHook, int32_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnUInt32(IHook* pHook, uint32_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnInt64(IHook* pHook, int64_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnUInt64(IHook* pHook, uint64_t value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnFloat(IHook* pHook, float value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnDouble(IHook* pHook, double value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnPointer(IHook* pHook, void* value) { return pHook->setReturn(value); }
extern "C"
PLUGIN_API void Dyno_SetReturnString(IHook* pHook, const char* value) { return pHook->setReturn(value); }