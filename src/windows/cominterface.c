/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#undef UNICODE
#include <initguid.h>
#include <windows.h>
#include <objbase.h>
#include "../../include/UniversalSpeech.h"

// Universal speech library GUID
// {4EB1B5DE-592E-4CBA-8230-D21D7456C38C}
DEFINE_GUID(CLSID_TypeLib, 0x4EB1B5DE, 0x592E, 0x4CBA, 0x82, 0x30, 0xD2, 0x1D, 0x74, 0x56, 0xC3, 0x8C);

// Universal speech object GUID
// {06C98751-54F6-433D-A751-D180789A854D}
DEFINE_GUID(CLSID_UniversalSpeech, 0x06C98751, 0x54F6, 0x433D, 0xA7, 0x51, 0xD1, 0x80, 0x78, 0x9A, 0x85, 0x4D);

// Universal speech VTable GUID
// {C0BC1432-993C-4D7B-A4BB-A42646226FA1}
DEFINE_GUID(IID_UniversalSpeech, 0xC0BC1432, 0x993C, 0x4D7B, 0xA4, 0xBB, 0xA4, 0x26, 0x46, 0x22, 0x6F, 0xA1);

// Universal speech VTable
#undef  INTERFACE
#define INTERFACE IUniversalSpeech
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
	// IUnknown functions
	STDMETHOD  (QueryInterface)		(THIS_ REFIID, void **) PURE;
	STDMETHOD_ (ULONG, AddRef)		(THIS) PURE;
	STDMETHOD_ (ULONG, Release)		(THIS) PURE;
	// IDispatch functions
	STDMETHOD_ (ULONG, GetTypeInfoCount)(THIS_ UINT *) PURE;
	STDMETHOD_ (ULONG, GetTypeInfo)		(THIS_ UINT, LCID, ITypeInfo **) PURE;
	STDMETHOD_ (ULONG, GetIDsOfNames)	(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
	STDMETHOD_ (ULONG, Invoke)			(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;
	// Extra functions
	STDMETHOD  (Say)			(THIS_ BSTR, BOOL) PURE;
	STDMETHOD  (Stop)			(void*) PURE;
	STDMETHOD  (SetValue)			(THIS_ int, int) PURE;
	STDMETHOD  (GetValue)			(THIS_ int) PURE;
	STDMETHOD  (SetString)			(THIS_ int, BSTR) PURE;
	STDMETHOD_  (BSTR, GetString)			(THIS_ int) PURE;
};

// A count of how many objects our DLL has created (by some
// app calling our IClassFactory object's CreateInstance())
// which have not yet been Release()'d by the app
static DWORD		OutstandingObjects;

// A count of how many apps have locked our DLL via calling our
// IClassFactory object's LockServer()
static DWORD		LockCount;

// Where I store a pointer to my type library's TYPEINFO
static ITypeInfo	*MyTypeInfo;

// The UniversalSpeech object
//##
typedef struct {
IUniversalSpeechVtbl	*lpVtbl;
	DWORD			count;
} UniversalSpeech;

// Here are IExample2's functions.
//
// Every COM object's interface must have the 3 functions QueryInterface(),
// AddRef(), and Release().

// QueryInterface()
static HRESULT STDMETHODCALLTYPE QueryInterface(IUniversalSpeech *this, REFIID vTableGuid, void **ppv)
{
	// Check if the GUID matches IExample2 VTable's GUID. We gave the C variable name
	// IID_IExample2 to our VTable GUID. We can use an OLE function called
	// IsEqualIID to do the comparison for us. Also, if the caller passed a
	// IUnknown GUID, then we'll likewise return the IExample2, since it can
	// masquerade as an IUnknown object too. Finally, if the called passed a
	// IDispatch GUID, then we'll return the IExample2, since it can masquerade
	// as an IDispatch too
	if (!IsEqualIID(vTableGuid, &IID_IUnknown) && !IsEqualIID(vTableGuid, &IID_UniversalSpeech) && !IsEqualIID(vTableGuid, &IID_IDispatch))
	{
      // We don't recognize the GUID passed to us. Let the caller know this,
      // by clearing his handle, and returning E_NOINTERFACE.
      *ppv = 0;
      return(E_NOINTERFACE);
	}

	// Fill in the caller's handle
	*ppv = this;

	// Increment the count of callers who have an outstanding pointer to this object
	this->lpVtbl->AddRef(this);

	return(NOERROR);
}

// AddRef()
static ULONG STDMETHODCALLTYPE AddRef(IUniversalSpeech *this)
{
	// Increment reference count, and return the updated value.
	return++(((UniversalSpeech*)this)->count);
}

// Release()
static ULONG STDMETHODCALLTYPE Release(IUniversalSpeech *this)
{
	// Decrement IExample2's reference count. If 0, then we can safely free
	// this IExample2 now
if (((UniversalSpeech*)this)->count <= 0) 
	{
		GlobalFree(this);
		InterlockedDecrement(&OutstandingObjects);
		return(0);
	}
	return ((UniversalSpeech*)this)->count;
}

// ================== The standard IDispatch functions

// This is just a helper function for the IDispatch functions below
static HRESULT loadMyTypeInfo(void)
{
	register HRESULT	hr;
	LPTYPELIB			pTypeLib;

	// Load our type library and get a ptr to its TYPELIB. Note: This does an
	// implicit pTypeLib->lpVtbl->AddRef(pTypeLib)
	if (!(hr = LoadRegTypeLib(&CLSID_TypeLib, 1, 0, 0, &pTypeLib)))
	{
		// Get Microsoft's generic ITypeInfo, giving it our loaded type library. We only
		// need one of these, and we'll store it in a global Tell Microsoft this is for
		// our IExample2's VTable, by passing that VTable's GUID
		if (!(hr = pTypeLib->lpVtbl->GetTypeInfoOfGuid(pTypeLib, &IID_UniversalSpeech, &MyTypeInfo)))
		{
			// We no longer need the ptr to the TYPELIB now that we've given it
			// to Microsoft's generic ITypeInfo. Note: The generic ITypeInfo has done
			// a pTypeLib->lpVtbl->AddRef(pTypeLib), so this TYPELIB ain't going away
			// until the generic ITypeInfo does a pTypeLib->lpVtbl->Release too
			pTypeLib->lpVtbl->Release(pTypeLib);

			// Since caller wants us to return our ITypeInfo pointer,
			// we need to increment its reference count. Caller is
			// expected to Release() it when done
			MyTypeInfo->lpVtbl->AddRef(MyTypeInfo);
		}
	}

	return(hr);
}

// GetTypeInfoCount()
static ULONG STDMETHODCALLTYPE GetTypeInfoCount(IUniversalSpeech *this, UINT *pCount)
{
	*pCount = 1;
	return(S_OK);
}

// GetTypeInfo()
static ULONG STDMETHODCALLTYPE GetTypeInfo(IUniversalSpeech *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
	register HRESULT	hr;

	// Assume an error
	*pTypeInfo = 0;
	
	if (itinfo)
		hr = ResultFromScode(DISP_E_BADINDEX);

	// If our ITypeInfo is already created, just increment its ref count. NOTE: We really should
	// store the LCID of the currently created TYPEINFO and compare it to what the caller wants.
	// If no match, unloaded the currently created TYPEINFO, and create the correct one. But since
	// we support only one language in our IDL file anyway, we'll ignore this
	else if (MyTypeInfo)
	{
		MyTypeInfo->lpVtbl->AddRef(MyTypeInfo);
		hr = 0;
	}
	else
	{
		// Load our type library and get Microsoft's generic ITypeInfo object. NOTE: We really
		// should pass the LCID to match, but since we support only one language in our IDL
		// file anyway, we'll ignore this
		hr = loadMyTypeInfo();
	}

	if (!hr) *pTypeInfo = MyTypeInfo;

	return(hr);
}

// GetIDsOfNames()
static ULONG STDMETHODCALLTYPE GetIDsOfNames(IUniversalSpeech *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
	if (!MyTypeInfo)
	{
		register HRESULT	hr;

		if ((hr = loadMyTypeInfo())) return(hr);
	}
	
	// Let OLE32.DLL's DispGetIDsOfNames() do all the real work of using our type
	// library to look up the DISPID of the requested function in our object
	return(DispGetIDsOfNames(MyTypeInfo, rgszNames, cNames, rgdispid));
}

// Invoke()
static ULONG STDMETHODCALLTYPE Invoke(IUniversalSpeech *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
   // We implement only a "default" interface
   if (!IsEqualIID(riid, &IID_NULL))
      return(DISP_E_UNKNOWNINTERFACE);

	// We need our type lib's TYPEINFO (to pass to DispInvoke)
	if (!MyTypeInfo)
	{
		register HRESULT	hr;

		if ((hr = loadMyTypeInfo())) return(hr);
	}

	// Let OLE32.DLL's DispInvoke() do all the real work of calling the appropriate
	// function in our object, and massaging the passed args into the correct format
	return(DispInvoke(this, MyTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr));
}

// ================== Extra functions

static HRESULT STDMETHODCALLTYPE Say (IUniversalSpeech *this, BSTR str, BOOL interrupt)
{
return speechSay(str, interrupt);
}

static HRESULT STDMETHODCALLTYPE StopSpeech(IUniversalSpeech *this)
{
return speechStop();
}

static HRESULT STDMETHODCALLTYPE GetValue (IUniversalSpeech *this, int what)
{
return speechGetValue(what);
}

static HRESULT STDMETHODCALLTYPE SetValue (IUniversalSpeech *this, int what, int value)
{
return speechSetValue(what, value);
}

static HRESULT STDMETHODCALLTYPE SetString  (IUniversalSpeech *this, int what, BSTR value)
{
return speechSetString(what, value);
}

static BSTR STDMETHODCALLTYPE GetString (IUniversalSpeech *this, int what) 
{
const wchar_t* w = speechGetString(what);
if (!w) return NULL;
return SysAllocString(w);
}

// Here's UniversalSpeech VTable. It never changes so we can declare it
// static
// VTable begin
static const IUniversalSpeechVtbl UniversalSpeech_Vtbl = {
QueryInterface, AddRef, Release,
GetTypeInfoCount, GetTypeInfo, GetIDsOfNames, Invoke,
Say, StopSpeech,
SetValue, GetValue,
SetString, GetString
//GetEngineName
};

// The IClassFactory object 

// Since we only ever need one IClassFactory object, we declare
// it static. The only requirement is that we ensure any
// access to its members is thread-safe
static IClassFactory	MyIClassFactoryObj;

// IClassFactory's AddRef()
static ULONG STDMETHODCALLTYPE classAddRef(IClassFactory *this)
{
	// Someone is obtaining my IClassFactory, so inc the count of
	// pointers that I've returned which some app needs to Release()
	InterlockedIncrement(&OutstandingObjects);

	// Since we never actually allocate/free an IClassFactory (ie, we
	// use just 1 static one), we don't need to maintain a separate
	// reference count for our IClassFactory. We'll just tell the caller
	// that there's at least one of our IClassFactory objects in existance
	return(1);
}

// IClassFactory's QueryInterface()
static HRESULT STDMETHODCALLTYPE classQueryInterface(IClassFactory *this, REFIID factoryGuid, void **ppv)
{
	// Make sure the caller wants either an IUnknown or an IClassFactory.
	// In either case, we return the same IClassFactory pointer passed to
	// us since it can also masquerade as an IUnknown
	if (IsEqualIID(factoryGuid, &IID_IUnknown) || IsEqualIID(factoryGuid, &IID_IClassFactory))
	{
		// Call my IClassFactory's AddRef
		this->lpVtbl->AddRef(this);

		// Return (to the caller) a ptr to my IClassFactory
		*ppv = this;

		return(NOERROR);
	}

	// We don't know about any other GUIDs
	*ppv = 0;
	return(E_NOINTERFACE);
}

// IClassFactory's Release()
static ULONG STDMETHODCALLTYPE classRelease(IClassFactory *this)
{
	// One less object that an app has not yet Release()'ed
	return(InterlockedDecrement(&OutstandingObjects));
}

// IClassFactory's CreateInstance() function. It is called by
// someone who has a pointer to our IClassFactory object and now
// wants to create and retrieve a pointer to our IExample2
static HRESULT STDMETHODCALLTYPE classCreateInstance(IClassFactory *this, IUnknown *punkOuter, REFIID vTableGuid, void **objHandle)
{
	HRESULT				hr;
	register IUniversalSpeech	*thisobj;

	// Assume an error by clearing caller's handle
	*objHandle = 0;

	// We don't support aggregation in this example
	if (punkOuter)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		// Allocate our object 
		if (!(thisobj = (IUniversalSpeech *)GlobalAlloc(GMEM_FIXED, sizeof(UniversalSpeech))))
			hr = E_OUTOFMEMORY;
		else
		{
			// Store IExample2's VTable in the object
			thisobj->lpVtbl = (IUniversalSpeechVtbl *)&UniversalSpeech_Vtbl;

			// Increment the reference count so we can call Release() below and
			// it will deallocate only if there is an error with QueryInterface()
			((UniversalSpeech *)thisobj)->count = 1;

			// Fill in the caller's handle with a pointer to the UniversalSpeech we just
			// allocated above. We'll let QueryInterface do that, because
			// it also checks the GUID the caller passed, and also increments the
			// reference count (to 2) if all goes well
			hr = UniversalSpeech_Vtbl.QueryInterface(thisobj, vTableGuid, objHandle);

			// Decrement reference count. NOTE: If there was an error in QueryInterface()
			// then Release() will be decrementing the count back to 0 and will free the
			// IExample for us. One error that may occur is that the caller is asking for
			// some sort of object that we don't support (ie, it's a GUID we don't recognize)
UniversalSpeech_Vtbl.Release(thisobj);

			// If success, inc static object count to keep this DLL loaded
			if (!hr) InterlockedIncrement(&OutstandingObjects);
		}
	}

	return(hr);
}

// IClassFactory's LockServer(). It is called by someone
// who wants to lock this DLL in memory
static HRESULT STDMETHODCALLTYPE classLockServer(IClassFactory *this, BOOL flock)
{
	if (flock) InterlockedIncrement(&LockCount);
	else InterlockedDecrement(&LockCount);

	return(NOERROR);
}

// IClassFactory's VTable
static const IClassFactoryVtbl IClassFactory_Vtbl = {classQueryInterface,
classAddRef,
classRelease,
classCreateInstance,
classLockServer};

// Miscellaneous functions ///////////////////////////////////////////////////////

/************************ DllGetClassObject() ***********************
 * This is called by the OLE functions CoGetClassObject() or
 * CoCreateInstance() in order to get our DLL's IClassFactory object 
 * (and return it to someone who wants to use it to get ahold of one
 * of our IExample objects). Our IClassFactory's CreateInstance() can
 * be used to allocate/retrieve our IExample object.
 *
 * NOTE: After we return the pointer to our IClassFactory, the caller
 * will typically call its CreateInstance() function.
 */

HRESULT PASCAL DllGetClassObject(REFCLSID objGuid, REFIID factoryGuid, void **factoryHandle)
{
	register HRESULT		hr;

	// Check that the caller is passing our UniversalSpeech GUID. That's the
	// only object our DLL implements
	if (IsEqualCLSID(objGuid, &CLSID_UniversalSpeech))
	{
		// Fill in the caller's handle with a pointer to our IClassFactory object.
		// We'll let our IClassFactory's QueryInterface do that, because it also
		// checks the IClassFactory GUID and does other book-keeping
		hr = classQueryInterface(&MyIClassFactoryObj, factoryGuid, factoryHandle);
	}
	else
	{
		// We don't understand this GUID. It's obviously not for our DLL.
		// Let the caller know this by clearing his handle and returning
		// CLASS_E_CLASSNOTAVAILABLE
		*factoryHandle = 0;
		hr = CLASS_E_CLASSNOTAVAILABLE;
	}

	return(hr);
}


/************************ DllCanUnloadNow() ***********************
 * This is called by some OLE function in order to determine
 * whether it is safe to unload our DLL from memory.
 *
 * RETURNS: S_OK if safe to unload, or S_FALSE if not.
 */

HRESULT PASCAL DllCanUnloadNow(void)
{
	// If someone has retrieved pointers to any of our objects, and
	// not yet Release()'ed them, then we return S_FALSE to indicate
	// not to unload this DLL. Also, if someone has us locked, return
	// S_FALSE
	return((OutstandingObjects | LockCount) ? S_FALSE : S_OK);
}




/************************** DllMain() **************************
 * Called by OS when this DLL is loaded or unloaded.
 */

BOOL WINAPI DllMain(HINSTANCE instance, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			// No TypeInfo yet loaded
			MyTypeInfo = 0;

			// Clear global counts
			OutstandingObjects = LockCount = 0;

			// Initialize my IClassFactory with the pointer to its vtable
			MyIClassFactoryObj.lpVtbl = (IClassFactoryVtbl *)&IClassFactory_Vtbl;

			// We don't need to do any thread initialization
			DisableThreadLibraryCalls(instance);
			break;
		}

		case DLL_PROCESS_DETACH:
		{
			// Release any TYPEINFO that my IDispatch functions got
			if (MyTypeInfo) MyTypeInfo->lpVtbl->Release(MyTypeInfo);
		}
	}

	return(1);
}

static const char CLSID_FORMAT[] = "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}";
static const char PROG_ID[] = "UniversalSpeech.Interface";
static const char ObjectDescription[] = "Screen reader API COM Server";

static void stringFromCLSID(LPTSTR buffer, REFCLSID ri)
{
	sprintf(buffer, &CLSID_FORMAT[0],
		((REFCLSID)ri)->Data1, ((REFCLSID)ri)->Data2, ((REFCLSID)ri)->Data3, ((REFCLSID)ri)->Data4[0],
		((REFCLSID)ri)->Data4[1], ((REFCLSID)ri)->Data4[2], ((REFCLSID)ri)->Data4[3],
		((REFCLSID)ri)->Data4[4], ((REFCLSID)ri)->Data4[5], ((REFCLSID)ri)->Data4[6],
		((REFCLSID)ri)->Data4[7]);
}



static void registerDllCleanup (void)
{
	HKEY		rootKey;
	HKEY		hKey;
	HKEY		hKey2;
	TCHAR		buffer[39];

	stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_UniversalSpeech));

	// Open "HKEY_LOCAL_MACHINE\Software\Classes"
	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Classes", 0, KEY_WRITE, &rootKey))
	{
		// Delete ProgID subkey and everything under it
		if (!RegOpenKeyEx(rootKey, &PROG_ID[0], 0, KEY_ALL_ACCESS, &hKey))
		{
			RegDeleteKey(hKey, "CLSID");
			RegCloseKey(hKey);
			RegDeleteKey(rootKey, &PROG_ID[0]);

			// Delete our CLSID key and everything under it
			if (!RegOpenKeyEx(rootKey, "CLSID", 0, KEY_ALL_ACCESS, &hKey))
			{
				if (!RegOpenKeyEx(hKey, &buffer[0], 0, KEY_ALL_ACCESS, &hKey2))
				{
					RegDeleteKey(hKey2, "InprocServer32");
				RegDeleteKey(hKey2, &PROG_ID[0]);
					RegCloseKey(hKey2);
					RegDeleteKey(hKey, &buffer[0]);
				}

				RegCloseKey(hKey);
			}
		}

		RegCloseKey(rootKey);

		// Unregister type library
		UnRegisterTypeLib(&CLSID_TypeLib, 1, 0, LOCALE_NEUTRAL, SYS_WIN32);
	}
}

static BOOL registerDll (const char* filename) {
		HKEY		rootKey;
		HKEY		hKey;
		HKEY		hKey2;
		HKEY		hkExtra;
		TCHAR		buffer[39];
		DWORD		disposition;
DWORD result = 1;


		if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Classes", 0, KEY_WRITE, &rootKey))
		{
			// For a script engine to call the OLE function CLSIDFromProgID() (passing
			// our registered ProgID in order to get our IExample2 object's GUID), then 
			// we need to create a subkey named with our IExample2 ProgID string. We've
			// decided to use the ProgID "IExample2.object"
			if (!RegCreateKeyEx(rootKey, &PROG_ID[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey, &disposition))
			{
				// Set its default value to some "friendly" string that helps
				// a user identify what this COM DLL is for. Setting this value
				// is optional. You don't need to do it
				RegSetValueEx(hKey, 0, 0, REG_SZ, &ObjectDescription[0], sizeof(ObjectDescription));

				// Create a "CLSID" subkey whose default value is our IExample2 object's GUID (in ascii string format)
				if (!(disposition = RegCreateKeyEx(hKey, "CLSID", 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey2, &disposition)))
				{
					stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_UniversalSpeech));
					disposition = RegSetValueEx(hKey2, 0, 0, REG_SZ, (const BYTE *)&buffer[0], lstrlen(&buffer[0]) + 1);
					RegCloseKey(hKey2);
				}
				RegCloseKey(hKey);

				if (!disposition)
				{
					// Open "HKEY_LOCAL_MACHINE\Software\Classes\CLSID"
					if (!RegOpenKeyEx(rootKey, "CLSID", 0, KEY_ALL_ACCESS, &hKey))
					{
						// Create a subkey whose name is the ascii string that represents
						// our IExample2 object's GUID
						if (!RegCreateKeyEx(hKey, &buffer[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey2, &disposition))
						{
							// Set its default value to some "friendly" string that helps
							// a user identify what this COM DLL is for. Setting this value
							// is optional. You don't need to do it
							RegSetValueEx(hKey2, 0, 0, REG_SZ, (const BYTE *)&ObjectDescription[0], sizeof(ObjectDescription));

							// Create an "InprocServer32" key whose default value is the path of this DLL
							if (!RegCreateKeyEx(hKey2, "InprocServer32", 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkExtra, &disposition))
							{
								disposition = 1;
								if (!RegSetValueEx(hkExtra, 0, 0, REG_SZ, (const BYTE *)&filename[0], lstrlen(&filename[0]) + 1))
								{
									// Create a "ThreadingModel" value set to the string "both" (ie, we don't need to restrict an
									// application to calling this DLL's functions only from a single thread. We don't use global
									// data in our IExample2 functions, so we're thread-safe)
									disposition = RegSetValueEx(hkExtra, "ThreadingModel", 0, REG_SZ, (const BYTE *)"both", sizeof("both"));
								}

								RegCloseKey(hkExtra);

								// Create a "ProgID" subkey whose default value is our ProgID. This allows the app to call ProgIDFromCLSID()
								if (!disposition && !(disposition = RegCreateKeyEx(hKey2, "ProgID", 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkExtra, &disposition)))
								{
									disposition = RegSetValueEx(hkExtra, 0, 0, REG_SZ, (const BYTE *)&PROG_ID[0], sizeof(PROG_ID));
									RegCloseKey(hkExtra);
									if (!disposition) result = 0;
								}
							}

							RegCloseKey(hKey2);
						}

						RegCloseKey(hKey);
					}

					// Register the type lib (which is assumed to be a .TLB file in the same dir as this DLL)
					if (!result)
					{
						ITypeLib	*pTypeLib;
						LPTSTR		str;

						str = &filename[0] + lstrlen(&filename[0]);
						while (str > &filename[0] && *(str - 1) != '\\') --str;
						lstrcpy(str, "UniversalSpeech.tlb");

					#ifdef UNICODE
						if (!(result = LoadTypeLib(&filename[0], &pTypeLib)))
						{
							result = RegisterTypeLib(pTypeLib, &filename[0], 0);
							pTypeLib->lpVtbl->Release(pTypeLib);
						}
					#else
						{
						wchar_t		wbuffer[MAX_PATH];

						MultiByteToWideChar(CP_ACP, 0, &filename[0], -1, &wbuffer[0], MAX_PATH);
						if (!(result = LoadTypeLib(&wbuffer[0], &pTypeLib)))
						{
							result = RegisterTypeLib(pTypeLib, &wbuffer[0], 0);
							pTypeLib->lpVtbl->Release(pTypeLib);
						}
						}
					#endif
					}

					if (!result)
return TRUE;
				}
			}

			RegCloseKey(rootKey);
		}

		// If an error, make sure we clean everything up
		if (result)
		{
registerDllCleanup();
return FALSE;
		}
	}

HRESULT __stdcall DllRegisterServer(void) {
char filename[320]={0};
GetFullPathName("UniversalSpeech.dll", 319, filename, NULL);
return registerDll(filename)? S_OK : E_UNEXPECTED;
}
