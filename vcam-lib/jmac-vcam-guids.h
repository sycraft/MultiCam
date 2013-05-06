// Copyright John MacCormick 2012. Modified BSD license. NO WARRANTY.
#pragma once

    
        //#define VCAM_ASSERT(_x_) if (!(_x_))         \
        //    MessageBox(NULL, TEXT(#_x_), NULL, NULL)
    

// {76B223FC-E5B5-401B-AD80-2D3BEB5E1029}
DEFINE_GUID(CLSID_Multicam, 
0x76b223fc, 0xe5b5, 0x401b, 0xad, 0x80, 0x2d, 0x3b, 0xeb, 0x5e, 0x10, 0x29);


// {4673A4DE-E590-4A5D-85D7-E2396BFAF1B3}
DEFINE_GUID(CLSID_VirtualCamDoubler, 
0x4673a4de, 0xe590, 0x4a5d, 0x85, 0xd7, 0xe2, 0x39, 0x6b, 0xfa, 0xf1, 0xb3);


// {D485091B-5B60-4959-A227-419852C72BA5}
DEFINE_GUID(CLSID_VirtualCamClone, 
0xd485091b, 0x5b60, 0x4959, 0xa2, 0x27, 0x41, 0x98, 0x52, 0xc7, 0x2b, 0xa5);

// {80BBFC14-7054-4520-B552-F079D23ED452}
DEFINE_GUID(CLSID_VirtualCam3, 
0x80bbfc14, 0x7054, 0x4520, 0xb5, 0x52, 0xf0, 0x79, 0xd2, 0x3e, 0xd4, 0x52);

// {F90E74CE-7B8A-45c2-8C41-4D8F63347E42}
DEFINE_GUID(CLSID_VirtualCam2, 
0xf90e74ce, 0x7b8a, 0x45c2, 0x8c, 0x41, 0x4d, 0x8f, 0x63, 0x34, 0x7e, 0x42);

// {8E14549A-DB61-4309-AFA1-3578E927E933}
DEFINE_GUID(CLSID_VirtualCam,
            0x8e14549a, 0xdb61, 0x4309, 0xaf, 0xa1, 0x35, 0x78, 0xe9, 0x27, 0xe9, 0x33);

// {80FDF9B0-32FD-457b-8BE7-D367F3854959}
DEFINE_GUID(CLSID_GoogleAdapter0,
            0x80FDF9B0, 0x32FD, 0x457b, 0x8b, 0xe7, 0xd3, 0x67, 0xf3, 0x85, 0x49, 0x59);

// {80FDF9B1-32FD-457b-8BE7-D367F3854959}
DEFINE_GUID(CLSID_GoogleAdapter1,
            0x80FDF9B1, 0x32FD, 0x457b, 0x8b, 0xe7, 0xd3, 0x67, 0xf3, 0x85, 0x49, 0x59);

// {17CCA71B-ECD7-11D0-B908-00A0C9223196}
DEFINE_GUID(CLSID_Proxy,
            0x17CCA71B, 0xECD7, 0x11D0, 0xb9, 0x08, 0x00, 0xa0, 0xc9, 0x22, 0x31, 0x96);

// 30323449-0000-0010-8000-00AA00389B71  'I420' ==  MEDIASUBTYPE_I420 
DEFINE_GUID(CLSID_I420, 
			0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); 

// {CA120208-830C-41EC-A5F0-69C21F0E88CA}
DEFINE_GUID(CLSID_VHScrCap,
            0xCA120208, 0x830C, 0x41EC, 0xa5, 0xf0, 0x69, 0xc2, 0x1f, 0x0e, 0x88, 0xca);

// {1B544C22-FD0B-11CE-8C63-00AA0044B51E}
DEFINE_GUID(CLSID_Softcam,
            0x1B544C22, 0xFD0B, 0x11CE, 0x8c, 0x63, 0x00, 0xaa, 0x00, 0x44, 0xb5, 0x1e);

// {00CADAC6-7EA1-418B-8DDD-DF8510070101}
DEFINE_GUID(CLSID_FakeWebcamSource,
            0x00CADAC6, 0x7EA1, 0x418B, 0x8d, 0xdd, 0xdf, 0x85, 0x10, 0x07, 0x01, 0x01);

// {C3DE648B-FF20-474F-99E1-C60522200CF5}
DEFINE_GUID(CLSID_SplitCam,
            0xC3DE648B, 0xFF20, 0x474F, 0x99, 0xe1, 0xc6, 0x05, 0x22, 0x20, 0x0c, 0xf5);

// {3755D805-2E56-4A74-BD0F-13C7493A35C8}
DEFINE_GUID(CLSID_UScreenCapture,
            0x3755D805, 0x2E56, 0x4A74, 0xBD, 0x0F, 0x13, 0xc7, 0x49, 0x3a, 0x35, 0xc8);


// {508822D9-2B73-4D3D-B651-32083C75885B}
DEFINE_GUID(CLSID_VHMultiCam,
            0x508822D9, 0x2B73, 0x4D3D, 0xB6, 0x51, 0x32, 0x08, 0x3c, 0x75, 0x88, 0x5b);

// {}
//DEFINE_GUID(CLSID_,
//            0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x);
