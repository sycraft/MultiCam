// Copyright John MacCormick 2012. Modified BSD license. NO WARRANTY.
#include <streams.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "MultiCamFilter.h"
#include "MultiCamOutputPin.h"
#include "jmac-vcam-guids.h"
#include "ConnectHelpers.h"
#include "LogHelpers.h"
#include "MessageBox.h"
#include "win32_exception.h"

#define hrOK VCAM_ASSERT(SUCCEEDED(hr))

// from http://msdn.microsoft.com/en-us/library/dd940435(v=VS.85).aspx
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

MultiCamOutputPin::MultiCamOutputPin(
	__in_opt LPCTSTR     pObjectName,
	__inout MultiCamFilter       *pFilter,
	__inout HRESULT      *phr,
	__in_opt LPCWSTR     pName)
	: CTransformOutputPin(pObjectName, (CTransformFilter *) pFilter, phr, pName),
	m_pParent(pFilter)
{
	win32_exception::install_handler();
	vcamOpenLog(5, "MultiCamOutputPin");
	vcamLog(10, "***********************MultiCamOutputPin::MultiCamOutputPin***********************");

	HRESULT hr = S_OK;

	m_mt.InitMediaType();

	// The following does not work. Causes crashes in MultiCamFilter::JoinTemporaryGraph()
	//hr = InitializeMediaType();	hrOK;

	vcamLog(10, "MultiCamOutputPin::MultiCamOutputPin: constructor complete");
}


MultiCamOutputPin::~MultiCamOutputPin(void)
{
	vcamLog(10, "~~~~~~~~~~~~~~~~~~~~~~~~~MultiCamOutputPin::~MultiCamOutputPin~~~~~~~~~~~~~~~~~~~~~~~~~");
	vcamCloseLog(5, "MultiCamOutputPin");

}



HRESULT STDMETHODCALLTYPE MultiCamOutputPin::Connect( 
	/* [in] */ IPin *pReceivePin,
	/* [annotation][in] */ 
	__in_opt  const AM_MEDIA_TYPE *pmt)
{
	HRESULT hr = S_OK;
	VCAM_TRY
	vcamLog(10, "MultiCamOutputPin::Connect");

	//int y = 0;
	//int x = 3 / y;
	//VCAM_ASSERT(FALSE);
	//ASSERT(FALSE);

	hr = m_pParent->ConnectUpstream();
	if (FAILED(hr)) { goto done;}

	//if(*(m_mt.FormatType()) == GUID_NULL) {
	//		hr = InitializeMediaType();
	//		hrOK;
	//}


	hr = CTransformOutputPin::Connect(pReceivePin, pmt);

done:
	vcamLog(10, "         MultiCamOutputPin::Connect returning 0x%x", hr);

	if(hr==S_OK) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning S_OK");
	}
	if(hr==VFW_E_ALREADY_CONNECTED) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning VFW_E_ALREADY_CONNECTED");
	}
	if(hr==VFW_E_NO_ACCEPTABLE_TYPES) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning VFW_E_NO_ACCEPTABLE_TYPES");
	}
	if(hr==VFW_E_NO_TRANSPORT) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning VFW_E_NO_TRANSPORT");
	}
	if(hr==VFW_E_NOT_STOPPED) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning VFW_E_NOT_STOPPED");
	}
	if(hr==VFW_E_TYPE_NOT_ACCEPTED) {
		vcamLog(10, "         MultiCamOutputPin::Connect returning VFW_E_TYPE_NOT_ACCEPTED");
	}

VCAM_CATCH
	return hr;
}

STDMETHODIMP MultiCamOutputPin::QueryInterface(REFIID riid, __deref_out void **ppv) 
{      
	HRESULT hr = S_OK;

	char guidString[GUID_STRING_MAXLEN];
	Riid2String(riid, guidString);

	hr = GetOwner()->QueryInterface(riid,ppv);

	vcamLog(10, "MultiCamOutputPin::QueryInterface, riid = %s", guidString);

	// for debugging
	CheckConnectedTo();
	
	if (hr==S_OK) {
		vcamLog(10, "       MultiCamOutputPin::QueryInterface returning S_OK");
	}
	else if (hr==E_NOINTERFACE ) {
		vcamLog(10, "       MultiCamOutputPin::QueryInterface returning E_NOINTERFACE ");
	}
	else if (hr==E_POINTER) {
		vcamLog(10, "       MultiCamOutputPin::QueryInterface returning E_POINTER");
	} else {
		vcamLog(10, "       MultiCamOutputPin::QueryInterface returning 0x%x", hr);
	}

	return hr;            
};

STDMETHODIMP MultiCamOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	HRESULT hr = S_OK;
	CheckPointer(ppv,E_POINTER);

	if (riid == IID_IKsPropertySet) {
        hr = GetInterface((IKsPropertySet *) this, ppv);
		goto done;
    } else if (riid == IID_IAMStreamConfig) {
        hr =  GetInterface((IAMStreamConfig *) this, ppv);
		goto done;
    } else {
        hr =  CTransformOutputPin::NonDelegatingQueryInterface(riid, ppv);
		goto done;
    }

done:
	char guidString[GUID_STRING_MAXLEN];
	Riid2String(riid, guidString);
	vcamLog(10, "MultiCamOutputPin::NonDelegatingQueryInterface, riid = %s", guidString);
	if (hr==NOERROR) {
		vcamLog(10, "       MultiCamOutputPin::NonDelegatingQueryInterface returning NOERROR");
	}
	else if (hr==E_NOINTERFACE ) {
		vcamLog(10, "       MultiCamOutputPin::NonDelegatingQueryInterface returning E_NOINTERFACE ");
	}
	else if (hr==E_POINTER) {
		vcamLog(10, "       MultiCamOutputPin::NonDelegatingQueryInterface returning E_POINTER");
	} else {
		vcamLog(10, "       MultiCamOutputPin::NonDelegatingQueryInterface returning 0x%x", hr);
	}

	return hr;
} // NonDelegatingQueryInterface


//////////////////////////////////////////////////////////////////////////
// IKsPropertySet
//////////////////////////////////////////////////////////////////////////


HRESULT MultiCamOutputPin::Set(REFGUID guidPropSet, DWORD dwID, void *pInstanceData, 
                        DWORD cbInstanceData, void *pPropData, DWORD cbPropData)
{// Set: Cannot set any properties.
	vcamLog(10, "MultiCamOutputPin::Set");
    return E_NOTIMPL;
}

// Get: Return the pin category (our only property). 
HRESULT MultiCamOutputPin::Get(
    REFGUID guidPropSet,   // Which property set.
    DWORD dwPropID,        // Which property in that set.
    void *pInstanceData,   // Instance data (ignore).
    DWORD cbInstanceData,  // Size of the instance data (ignore).
    void *pPropData,       // Buffer to receive the property data.
    DWORD cbPropData,      // Size of the buffer.
    DWORD *pcbReturned     // Return the size of the property.
)
{
	char guidString[GUID_STRING_MAXLEN];
	Riid2String(guidPropSet, guidString);
	vcamLog(10, "MultiCamOutputPin::Get, guidPropSet = %s, dwPropID = %d", guidString, dwPropID);

    if (guidPropSet != AMPROPSETID_Pin)             return E_PROP_SET_UNSUPPORTED;
    if (dwPropID != AMPROPERTY_PIN_CATEGORY)        return E_PROP_ID_UNSUPPORTED;
    if (pPropData == NULL && pcbReturned == NULL)   return E_POINTER;
    
    if (pcbReturned) *pcbReturned = sizeof(GUID);
    if (pPropData == NULL)          return S_OK; // Caller just wants to know the size. 
    if (cbPropData < sizeof(GUID))  return E_UNEXPECTED;// The buffer is too small.
        
    *(GUID *)pPropData = PIN_CATEGORY_CAPTURE;
    return S_OK;
}

// QuerySupported: Query whether the pin supports the specified property.
HRESULT MultiCamOutputPin::QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport)
{
	vcamLog(10, "MultiCamOutputPin::QuerySupported");
    if (guidPropSet != AMPROPSETID_Pin) return E_PROP_SET_UNSUPPORTED;
    if (dwPropID != AMPROPERTY_PIN_CATEGORY) return E_PROP_ID_UNSUPPORTED;
    // We support getting this property, but not setting it.
    if (pTypeSupport) *pTypeSupport = KSPROPERTY_SUPPORT_GET; 
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// end IKsPropertySet
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//  IAMStreamConfig
//////////////////////////////////////////////////////////////////////////



HRESULT STDMETHODCALLTYPE MultiCamOutputPin::GetFormat(AM_MEDIA_TYPE **ppmt)
{
	vcamLog(10, "MultiCamOutputPin::GetFormat");
	
	HRESULT hr = S_OK;

	if(*(m_mt.FormatType()) == GUID_NULL) {
			hr = InitializeMediaType();
			hrOK;
	}

    *ppmt = CreateMediaType(&m_mt);

	vcamLog(10, "MultiCamOutputPin::GetFormat, *ppmt = ...");
	vcamLogFormat(12, *ppmt);

    return hr;
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::GetNumberOfCapabilities(int *piCount, int *piSize)
{
	HRESULT hr = S_OK;

	*piCount = 1;
	*piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);

	vcamLog(10, "MultiCamOutputPin::GetNumberOfCapabilities, Count = %d, Size = %d", *piCount, *piSize);
	
	// for debugging
	CheckConnectedTo();

    return hr;
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC)
{
	vcamLog(10, "MultiCamOutputPin::GetStreamCaps, iIndex = %d", iIndex);
	
	// for debugging
	CheckConnectedTo();

	HRESULT hr = S_OK;

	if (iIndex < 0) {
		hr = E_INVALIDARG;
		goto done;
	}

	if (iIndex > 0) {
		hr = S_FALSE;
		goto done;
	}

	if(*(m_mt.FormatType()) == GUID_NULL) {
			hr = InitializeMediaType();
			hrOK;
	}
	*ppmt = CreateMediaType(&m_mt);

	vcamLog(10, "MultiCamOutputPin::GetStreamCaps, **ppmt = ...");
	vcamLogFormat(12, *ppmt);

done:
	vcamLog(10, "       MultiCamOutputPin::GetStreamCaps returning 0x%x", hr);

	return hr;
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::SetFormat(AM_MEDIA_TYPE *pmt)
{
	vcamLog(10, "MultiCamOutputPin::SetFormat");
	vcamLog(10, "MultiCamOutputPin::SetFormat, *pmt = ...");
	vcamLogFormat(12, pmt);

	HRESULT hr = S_OK;
	IPin* pin = NULL; 

	CMediaType new_mt = CMediaType(*pmt, &hr);
	hrOK;
	if (new_mt == m_mt) {
		vcamLog(10, "MultiCamOutputPin::SetFormat: new media type is the same as old media type");
	} 
	else {
		vcamLog(10, "MultiCamOutputPin::SetFormat: new media type differs from old media type");		
	}

    m_mt = *pmt;

	hr = ConnectedTo(&pin);
	VCAM_ASSERT(SUCCEEDED(hr) || hr == VFW_E_NOT_CONNECTED);
    if(pin)
    {
		IFilterGraph *pGraph = m_pParent->m_pGraph;
		// jmac -- temporary hope-for-best experiment
		// Original code was the Reconnect(), but I don't how why -- Removing it doesn't seem to break anything
        //pGraph->Reconnect(this);

		// New experiment was this Disconnect(), but that didn't fix anything
		// this->Disconnect();
		// end jmac temporary hope-for-best experiment

		vcamLog(10, "   MultiCamOutputPin::SetFormat returning S_OK (pin was connected)");
    } else {
		vcamLog(10, "   MultiCamOutputPin::SetFormat returning S_OK (pin was not connected)");
	}
	SafeRelease(&pin);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//  end IAMStreamConfig
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Helper methods
//////////////////////////////////////////////////////////////////////////


HRESULT MultiCamOutputPin::InitializeMediaType()
{
	vcamLog(10, "MultiCamOutputPin::InitializeMediaType");

	HRESULT hr = S_OK;
	AM_MEDIA_TYPE mt;
	bool use_temp_graph = false;
	//VCAM_ASSERT(m_pParent != NULL);
	//VCAM_ASSERT(m_pParent->m_pUpstreamConfig != NULL);
	//hr = m_pParent->m_pUpstreamConfig->GetFormat(&pmt);
	LONG max_width = 0;
	LONG max_height = 0;

	if(m_pFilter->GetFilterGraph()==NULL) {
		use_temp_graph = true;
		hr = m_pParent->JoinTemporaryGraph();
		hrOK;
//		VCAM_ASSERT(m_pParent->m_pInput->IsConnected());
	}

	hr = m_pParent->ConnectUpstream();
	hrOK;

	hr = m_pParent->ConnectionMediaType(&mt);
	hrOK;

	vcamLog(10, "MultiCamOutputPin::InitializeMediaType: got media type:");
	vcamLogFormat(12, &mt);

	hr = m_pParent->GetLargestInputDetails(max_width, max_height);
	hrOK;

	VCAM_ASSERT(mt.formattype == FORMAT_VideoInfo || mt.formattype == FORMAT_VideoInfo2);
	BITMAPINFOHEADER *pbmi = GetBITMAPINFOHEADER(&mt);
	pbmi->biWidth = max_width;
	pbmi->biHeight = max_height;
	ULONG size = (ULONG) max_width * max_height * pbmi->biBitCount / 8;
	pbmi->biSizeImage = (DWORD) size;
	mt.lSampleSize = size;


	m_mt.Set(mt);
	FreeMediaType(mt);

//done:
	if (use_temp_graph) 
	{
		hr = m_pParent->DestroyTemporaryGraph();
		hrOK;
	}

	vcamLog(10, "MultiCamOutputPin::InitializeMediaType, new media type is:");
	vcamLogFormat(12, &m_mt);
	vcamLog(10, "    MultiCamOutputPin::InitializeMediaType, max_width = %ld, max_height = %ld", max_width, max_height);
	vcamLog(10, "    MultiCamOutputPin::InitializeMediaType, returning 0x%x", hr);
    return hr;
		
}

//////////////////////////////////////////////////////////////////////////
// end helper methods
//////////////////////////////////////////////////////////////////////////


HRESULT MultiCamOutputPin::GetMediaType(int iPosition, CMediaType *pMediaType)
{
	vcamLog(10, "MultiCamOutputPin::GetMediaType, iPosition = %d", iPosition);
	
	HRESULT hr = S_OK;
	if(*(m_mt.FormatType()) == GUID_NULL) {
			hr = InitializeMediaType();
			hrOK;
			vcamLog(1, "    MultiCamOutputPin::GetMediaType: InitializeMediaType() succeeded");
			hr = pMediaType->Set(m_mt);
			hrOK;
			vcamLog(1, "    MultiCamOutputPin::GetMediaType: pMediaType->Set(m_mt) succeeded");
			goto done;
	}

	if(!m_pTransformFilter->m_pInput->IsConnected()) {
		vcamLog(1, "    MultiCamOutputPin::GetMediaType: Input isn't connected !!**");
		// hope for the best??? -- nope, crashes as m_pGraph!=NULL
		//hr = m_pParent->ConnectUpstream();
		//vcamLog(1, "    MultiCamOutputPin::GetMediaType: ConnectUpstream returned 0x%x !!**", hr);
	}

	hr = CTransformOutputPin::GetMediaType(iPosition, pMediaType);

done:
	char* hr_string = "unknown";
	if (hr == S_OK) {
		hr_string = "S_OK";
	} else if (hr == VFW_S_NO_MORE_ITEMS) {
		hr_string = "VFW_S_NO_MORE_ITEMS";
	} else if (hr == E_INVALIDARG) {
		hr_string = "E_INVALIDARG";
	} else if (hr == E_UNEXPECTED) {
		hr_string = "E_UNEXPECTED";
	} 

	vcamLog(10, "    MultiCamOutputPin::GetMediaType, returning 0x%x (%s)", hr, hr_string);
    return hr;
}

HRESULT MultiCamOutputPin::CheckMediaType(const CMediaType *mt)
{
	vcamLog(10, "MultiCamOutputPin::CheckMediaType");
	
	HRESULT hr = S_OK;
	if(*(m_mt.FormatType()) == GUID_NULL) {
			hr = InitializeMediaType();
			hrOK;
	}
	hr = CTransformOutputPin::CheckMediaType(mt);

	vcamLog(10, "    MultiCamOutputPin::CheckMediaType, returning 0x%x", hr);
    return hr;		
}

HRESULT MultiCamOutputPin::AttemptConnection(
        IPin* pReceivePin,      // connect to this pin
        const CMediaType* pmt   // using this type
    )
{
	vcamLog(10, "MultiCamOutputPin::AttemptConnection");
	
	HRESULT hr = S_OK;

	hr = CTransformOutputPin::AttemptConnection(pReceivePin, pmt);

	vcamLog(10, "    MultiCamOutputPin::AttemptConnection, returning 0x%x", hr);
    return hr;		
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::QueryPinInfo( __out  PIN_INFO *pInfo) {
	vcamLog(50, "MultiCamOutputPin::QueryPinInfo");
	return CTransformOutputPin::QueryPinInfo(pInfo);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::ReceiveConnection( 
		/* [in] */ IPin *pConnector,
		/* [in] */ const AM_MEDIA_TYPE *pmt) {
	vcamLog(50, "MultiCamOutputPin::ReceiveConnection");
	return CTransformOutputPin::ReceiveConnection(pConnector, pmt);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::Disconnect( void) {
	vcamLog(50, "MultiCamOutputPin::Disconnect");
	return CTransformOutputPin::Disconnect();
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::ConnectedTo( 
		/* [annotation][out] */ 
		__out  IPin **pPin) {
	vcamLog(50, "MultiCamOutputPin::ConnectedTo");
	HRESULT hr = CTransformOutputPin::ConnectedTo(pPin);
	if (hr == S_OK) {
		vcamLog(50, "   MultiCamOutputPin::ConnectedTo: returning S_OK");
	} else if (hr == VFW_E_NOT_CONNECTED){
		vcamLog(50, "   MultiCamOutputPin::ConnectedTo: returning VFW_E_NOT_CONNECTED");
	} else {
		vcamLog(50, "   MultiCamOutputPin::ConnectedTo: returning 0x%x", hr);
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::ConnectionMediaType( 
		/* [annotation][out] */ 
		__out  AM_MEDIA_TYPE *pmt) {
	vcamLog(50, "MultiCamOutputPin::ConnectionMediaType");
	return CTransformOutputPin::ConnectionMediaType(pmt);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::QueryDirection( 
		/* [annotation][out] */ 
		__out  PIN_DIRECTION *pPinDir) {
	vcamLog(50, "MultiCamOutputPin::QueryDirection");

	// for debugging
	CheckConnectedTo();
	
	return CTransformOutputPin::QueryDirection(pPinDir);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::QueryId( 
		/* [annotation][out] */ 
		__out  LPWSTR *Id) {
	vcamLog(50, "MultiCamOutputPin::QueryId");
	return CTransformOutputPin::QueryId(Id);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::QueryAccept( 
		/* [in] */ const AM_MEDIA_TYPE *pmt) {
	vcamLog(50, "MultiCamOutputPin::QueryAccept");
	return CTransformOutputPin::QueryAccept(pmt);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::EnumMediaTypes( 
		/* [annotation][out] */ 
		__out  IEnumMediaTypes **ppEnum) {
	vcamLog(50, "MultiCamOutputPin::EnumMediaTypes");
	return CTransformOutputPin::EnumMediaTypes(ppEnum);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::QueryInternalConnections( 
		/* [annotation][out] */ 
		__out_ecount_part_opt(*nPin, *nPin)  IPin **apPin,
		/* [out][in] */ ULONG *nPin) {
	vcamLog(50, "MultiCamOutputPin::QueryInternalConnections");
	return CTransformOutputPin::QueryInternalConnections(apPin, nPin);
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::EndOfStream( void) {
	vcamLog(50, "MultiCamOutputPin::EndOfStream");
	return CTransformOutputPin::EndOfStream();
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::BeginFlush( void) {
	vcamLog(50, "MultiCamOutputPin::BeginFlush");
	return CTransformOutputPin::BeginFlush();
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::EndFlush( void) {
	vcamLog(50, "MultiCamOutputPin::EndFlush");
	return CTransformOutputPin::EndFlush();
}

HRESULT STDMETHODCALLTYPE MultiCamOutputPin::NewSegment( 
		/* [in] */ REFERENCE_TIME tStart,
		/* [in] */ REFERENCE_TIME tStop,
		/* [in] */ double dRate) {
	vcamLog(50, "MultiCamOutputPin::NewSegment");
	return CTransformOutputPin::NewSegment(tStart, tStop, dRate);
}

//////////////////////////////////////////////////////////////////////////
//  CBasePin
//////////////////////////////////////////////////////////////////////////
HRESULT MultiCamOutputPin::Active(void) {
	vcamLog(90, "MultiCamOutputPin::Active");
	return CTransformOutputPin::Active();
}

HRESULT MultiCamOutputPin::AgreeMediaType(IPin *pReceivePin, const CMediaType *pmt) {
	vcamLog(90, "MultiCamOutputPin::AgreeMediaType");
	return CTransformOutputPin::AgreeMediaType(pReceivePin, pmt);
}

HRESULT MultiCamOutputPin::BreakConnect() {
	vcamLog(90, "MultiCamOutputPin::BreakConnect");
	return CTransformOutputPin::BreakConnect();
}

bool MultiCamOutputPin::CanReconnectWhenActive() {
	vcamLog(90, "MultiCamOutputPin::CanReconnectWhenActive");
	return CTransformOutputPin::CanReconnectWhenActive();
}

HRESULT MultiCamOutputPin::CheckConnect(IPin *p) {
	vcamLog(90, "MultiCamOutputPin::CheckConnect");
	return CTransformOutputPin::CheckConnect(p);
}

HRESULT MultiCamOutputPin::CompleteConnect(IPin *pReceivePin) {
	vcamLog(90, "MultiCamOutputPin::CompleteConnect");
	return CTransformOutputPin::CompleteConnect(pReceivePin);
}

double MultiCamOutputPin::CurrentRate() {
	vcamLog(90, "MultiCamOutputPin::CurrentRate");
	return CTransformOutputPin::CurrentRate();
}

REFERENCE_TIME MultiCamOutputPin::CurrentStopTime() {
	vcamLog(90, "MultiCamOutputPin::CurrentStopTime");
	return CTransformOutputPin::CurrentStopTime();
}

REFERENCE_TIME MultiCamOutputPin::CurrentStartTime() {
	vcamLog(90, "MultiCamOutputPin::CurrentStartTime");
	return CTransformOutputPin::CurrentStartTime();
}

STDMETHODIMP MultiCamOutputPin::DisconnectInternal() {
	vcamLog(90, "MultiCamOutputPin::DisconnectInternal");
	return CTransformOutputPin::DisconnectInternal();
}

void MultiCamOutputPin::DisplayPinInfo(IPin *pReceivePin) {
	vcamLog(90, "MultiCamOutputPin::DisplayPinInfo");
	return CTransformOutputPin::DisplayPinInfo(pReceivePin);
}

void MultiCamOutputPin::DisplayTypeInfo(IPin *pPin, const CMediaType *pmt) {
	vcamLog(90, "MultiCamOutputPin::DisplayTypeInfo");
	return CTransformOutputPin::DisplayTypeInfo(pPin, pmt);
}

IPin * MultiCamOutputPin::GetConnected() {
	vcamLog(90, "MultiCamOutputPin::GetConnected");
	return CTransformOutputPin::GetConnected();
}

LONG MultiCamOutputPin::GetMediaTypeVersion() {
	vcamLog(90, "MultiCamOutputPin::GetMediaTypeVersion");
	return CTransformOutputPin::GetMediaTypeVersion();
}

HRESULT MultiCamOutputPin::Inactive(void) {
	vcamLog(90, "MultiCamOutputPin::Inactive");
	return CTransformOutputPin::Inactive();
}

void MultiCamOutputPin::IncrementTypeVersion() {
	vcamLog(90, "MultiCamOutputPin::IncrementTypeVersion");
	return CTransformOutputPin::IncrementTypeVersion();
}

BOOL MultiCamOutputPin::IsConnected(void) {
	vcamLog(90, "MultiCamOutputPin::IsConnected");
	return CTransformOutputPin::IsConnected();
}

BOOL MultiCamOutputPin::IsStopped() {
	vcamLog(90, "MultiCamOutputPin::IsStopped");
	return CTransformOutputPin::IsStopped();
}

LPWSTR MultiCamOutputPin::Name() {
	vcamLog(90, "MultiCamOutputPin::Name");
	return CTransformOutputPin::Name();
}

STDMETHODIMP MultiCamOutputPin::Notify(IBaseFilter * pSender, Quality q) {
	vcamLog(90, "MultiCamOutputPin::Notify");
	return CTransformOutputPin::Notify(pSender, q);
}

HRESULT MultiCamOutputPin::Run(REFERENCE_TIME tStart) {
	vcamLog(90, "MultiCamOutputPin::Run");
	return CTransformOutputPin::Run(tStart);
}

HRESULT MultiCamOutputPin::SetMediaType(const CMediaType *p) {
	vcamLog(90, "MultiCamOutputPin::SetMediaType");
	return CTransformOutputPin::SetMediaType(p);
}

void MultiCamOutputPin::SetReconnectWhenActive(bool bCanReconnect) {
	vcamLog(90, "MultiCamOutputPin::SetReconnectWhenActive");
	return CTransformOutputPin::SetReconnectWhenActive(bCanReconnect);
}

STDMETHODIMP MultiCamOutputPin::SetSink(IQualityControl * piqc) {
	vcamLog(90, "MultiCamOutputPin::SetSink");
	return CTransformOutputPin::SetSink(piqc);
}

HRESULT MultiCamOutputPin::TryMediaTypes(IPin *pReceivePin,__in_opt const CMediaType *pmt, IEnumMediaTypes *pEnum) {
	vcamLog(90, "MultiCamOutputPin::TryMediaTypes");
	return CTransformOutputPin::TryMediaTypes(pReceivePin, pmt, pEnum);
}

HRESULT MultiCamOutputPin::DecideAllocator(IMemInputPin * pPin, __deref_out IMemAllocator ** pAlloc) {
	vcamLog(90, "MultiCamOutputPin::DecideAllocator");
	return CTransformOutputPin::DecideAllocator(pPin, pAlloc);
}

HRESULT MultiCamOutputPin::DecideBufferSize(IMemAllocator * pAlloc, __inout ALLOCATOR_PROPERTIES * ppropInputRequest) {
	vcamLog(90, "MultiCamOutputPin::DecideBufferSize");
	return CTransformOutputPin::DecideBufferSize(pAlloc, ppropInputRequest);
}

HRESULT MultiCamOutputPin::GetDeliveryBuffer(__deref_out IMediaSample ** ppSample, __in_opt REFERENCE_TIME * pStartTime, __in_opt REFERENCE_TIME * pEndTime, DWORD dwFlags) {
	vcamLog(90, "MultiCamOutputPin::GetDeliveryBuffer");
	return CTransformOutputPin::GetDeliveryBuffer(ppSample, pStartTime, pEndTime, dwFlags);
}

HRESULT MultiCamOutputPin::Deliver(IMediaSample *s) {
	vcamLog(90, "MultiCamOutputPin::Deliver");
	return CTransformOutputPin::Deliver(s);
}

HRESULT MultiCamOutputPin::InitAllocator(__deref_out IMemAllocator **ppAlloc) {
	vcamLog(90, "MultiCamOutputPin::InitAllocator");
	return CTransformOutputPin::InitAllocator(ppAlloc);
}

HRESULT MultiCamOutputPin::DeliverEndOfStream(void) {
	vcamLog(90, "MultiCamOutputPin::DeliverEndOfStream");
	return CTransformOutputPin::DeliverEndOfStream();
}

HRESULT MultiCamOutputPin::DeliverBeginFlush(void) {
	vcamLog(90, "MultiCamOutputPin::DeliverBeginFlush");
	return CTransformOutputPin::DeliverBeginFlush();
}

HRESULT MultiCamOutputPin::DeliverEndFlush(void) {
	vcamLog(90, "MultiCamOutputPin::DeliverEndFlush");
	return CTransformOutputPin::DeliverEndFlush();
}

HRESULT MultiCamOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate) {
	vcamLog(90, "MultiCamOutputPin::DeliverNewSegment");
	return CTransformOutputPin::DeliverNewSegment(tStart, tStop, dRate);
}

// For debugging
void MultiCamOutputPin::CheckConnectedTo() {
	HRESULT hr = S_OK;
	IPin* pin = NULL; 
	hr = ConnectedTo(&pin);
	VCAM_ASSERT(SUCCEEDED(hr) || hr == VFW_E_NOT_CONNECTED);
	if(pin)
	{
		vcamLog(95, "   MultiCamOutputPin::CheckConnectedTo: got downstream pin");
	} else {
		vcamLog(95, "   MultiCamOutputPin::CheckConnectedTo: no downstream pin found");
	}
	SafeRelease(&pin);
}