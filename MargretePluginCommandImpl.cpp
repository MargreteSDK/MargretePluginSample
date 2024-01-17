#include <Windows.h>
#include "MargretePlugin.h"
#include "MargretePluginCommandImpl.h"

MpBoolean CMargretePluginCommandImpl::queryInterface(const MpGuid& iid, void** ppobj) {
	if (!ppobj)
		return MP_FALSE;

	if (iid == IID_IMargretePluginBase || iid == IID_IMargretePluginCommand) {
		*ppobj = this;
		addRef();
		return MP_TRUE;
	}
	return MP_FALSE;
}

MpBoolean CMargretePluginCommandImpl::getCommandName(wchar_t* text, MpInteger textLength) const {
	wcsncpy_s(text, textLength, L"MargretePluginSample", textLength);
	return MP_TRUE;
}

MpBoolean CMargretePluginCommandImpl::invoke(IMargretePluginContext* ctx) {
	MargreteComPtr<IMargretePluginDocument> doc;
	MargreteComPtr<IMargretePluginChart> chart;
	MargreteComPtr<IMargretePluginUndoBuffer> undoBuffer;

	ctx->getDocument(doc.put());
	doc->getChart(chart.put());
	doc->getUndoBuffer(undoBuffer.put());

	MpInteger currentTick = ctx->getCurrentTick();

	undoBuffer->beginRecording();

	// insert slide with air
	{
		MP_NOTEINFO noteInfo;
		memset(&noteInfo, 0, sizeof(noteInfo));

		MargreteComPtr<IMargretePluginNote> slideBegin, slideStep, slideEnd, air;
		if (chart->createNote(slideBegin.put()) &&
			chart->createNote(slideStep.put()) &&
			chart->createNote(slideEnd.put()) &&
			chart->createNote(air.put())) {

			noteInfo.type = MP_NOTETYPE_SLIDE;
			noteInfo.longAttr = MP_NOTELONGATTR_BEGIN;
			noteInfo.x = 4;
			noteInfo.width = 4;
			noteInfo.tick = currentTick;
			noteInfo.timelineId = 0;
			slideBegin->setInfo(&noteInfo);

			noteInfo.longAttr = MP_NOTELONGATTR_STEP;
			noteInfo.x = 8;
			noteInfo.tick = currentTick + 480;
			slideStep->setInfo(&noteInfo);

			noteInfo.longAttr = MP_NOTELONGATTR_END;
			noteInfo.x = 6;
			noteInfo.tick = currentTick + 480 * 2;
			slideEnd->setInfo(&noteInfo);

			noteInfo.type = MP_NOTETYPE_AIR;
			noteInfo.direction = MP_NOTEDIR_UPLEFT;
			air->setInfo(&noteInfo);

			// put air into slideEnd
			slideEnd->appendChild(air.get());

			// put slide children into slideBegin
			slideBegin->appendChild(slideStep.get());
			slideBegin->appendChild(slideEnd.get());

			// put slideBegin into the chart
			chart->appendNote(slideBegin.get());
		}
	}

	// insert bpm event
	{
		MP_EVENT_BPMINFO bpmInfo;
		memset(&bpmInfo, 0, sizeof(bpmInfo));

		bpmInfo.tick = currentTick;
		bpmInfo.bpm = 240.0;

		MargreteComPtr<IMargretePluginEventBpm> bpm;
		if (chart->findEventBpm(bpmInfo.tick, bpm.putVoid())) { // if a bpm event exists on the tick we want to put
			bpm->setInfo(&bpmInfo);
		}
		else if (chart->createEvent(IID_IMargretePluginEventBpm, bpm.putVoid())) { // otherwise create
			bpm->setInfo(&bpmInfo);
			chart->appendEvent(bpm.get());
		}
	}

	undoBuffer->commitRecording();
	
	ctx->update();

	MessageBoxW((HWND)ctx->getMainWindowHandle(), L"Inserted some objects!", L"MargretePluginSample", MB_ICONINFORMATION);

	return MP_TRUE;
}

