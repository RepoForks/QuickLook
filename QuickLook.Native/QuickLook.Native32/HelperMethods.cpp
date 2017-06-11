#include "stdafx.h"
#include "HelperMethods.h"

void HelperMethods::GetSelectedInternal(CComQIPtr<IWebBrowserApp> pwba, PWCHAR buffer)
{
	CComQIPtr<IServiceProvider> psp;
	if (FAILED(pwba->QueryInterface(IID_IServiceProvider, reinterpret_cast<void**>(&psp))))
		return;

	CComPtr<IShellBrowser> psb;
	if (FAILED(psp->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, reinterpret_cast<LPVOID*>(&psb))))
		return;

	CComPtr<IShellView> psv;
	if (FAILED(psb->QueryActiveShellView(&psv)))
		return;

	CComPtr<IDataObject> dao;
	if (FAILED(psv->GetItemObject(SVGIO_SELECTION, IID_IDataObject, reinterpret_cast<void**>(&dao))))
		return;

	return ObtainFirstItem(dao, buffer);
}

void HelperMethods::ObtainFirstItem(CComPtr<IDataObject> dao, PWCHAR buffer)
{
	FORMATETC formatetc;
	STGMEDIUM medium;

	formatetc.cfFormat = CF_HDROP;
	formatetc.ptd = nullptr;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex = -1;
	formatetc.tymed = TYMED_HGLOBAL;

	medium.tymed = TYMED_HGLOBAL;

	if (FAILED(dao->GetData(&formatetc, &medium)))
		return;

	int n = DragQueryFile(HDROP(medium.hGlobal), 0xFFFFFFFF, nullptr, 0);

	if (n < 1)
		return;

	DragQueryFile(HDROP(medium.hGlobal), 0, buffer, MAX_PATH - 1);
}

bool HelperMethods::IsCursorActivated(HWND hwnd)
{
	auto tId = GetWindowThreadProcessId(hwnd, nullptr);

	GUITHREADINFO gui = {sizeof gui};
	GetGUIThreadInfo(tId, &gui);
	return gui.flags || gui.hwndCaret;
}