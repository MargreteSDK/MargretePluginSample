#pragma once
#include <MargretePlugin.h>
class CMargretePluginCommandImpl : public IMargretePluginCommand {
public:
	virtual MpInteger addRef() {
		return ++m_refCount;
	}

	virtual MpInteger release() {
		--m_refCount;
		if (m_refCount <= 0) {
			delete this;
			return 0;
		}
		return m_refCount;
	}
	virtual MpBoolean queryInterface(const MpGuid& iid, void** ppobj);

	virtual MpBoolean getCommandName(wchar_t* text, MpInteger textLength) const;
	virtual MpBoolean invoke(IMargretePluginContext* ctx);

private:
	MpInteger m_refCount = 0;
};

