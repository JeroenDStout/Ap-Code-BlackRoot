/*
 *
 *  � Stout Games 2019
 *
 */

#pragma once

#include <sstream>

#include "BlackRoot/Pubc/Exception.h"

namespace BlackRoot
{
namespace Debug
{

	#ifdef _DEBUG

		#define DBGFLAG_ASSERT
        #define DBGFLAG_MEMORY_PARANOIA
	
	#endif
	
	#ifdef _RELEASE

		#undef DBGFLAG_ASSERT
	
	#endif

	#define _DbAssertFatalMsg(_tst, _msg)										\
				do {															\
				if (!(_tst))													\
				{	std::stringstream ss;                                       \
                    throw new BlackRoot::Debug::Exception((std::stringstream("Fatal assertion failed: �") << #_tst << "�" << std::endl << "  (" << #_tst << ")").str()); \
				}																\
				} while (0)

	#define _DbAssertFatal(_tst)										        \
				do {															\
				if (!(_tst))													\
				{	std::stringstream ss;                                       \
                    throw new BlackRoot::Debug::Exception((std::stringstream("Fatal assertion failed: �") << #_tst << "�").str()); \
				}																\
				} while (0)

	#define	DbAssertFatal(x)			_DbAssertFatal(x)
	#define	DbAssertMsgFatal(x, msg)	_DbAssertFatalMsg(x, msg)

	#ifdef DBGFLAG_ASSERT

		#define	DbAssert(x)				_DbAssertFatal(x)
		#define	DbAssertMsg(x, msg)		_DbAssertFatalMsg(x, msg)
		
	#else

		#define	DbAssert(x)				(void(0))
		#define	DbAssertMsg(x, msg)		(void(0))

	#endif

#ifdef _VISUAL_STUDIO

	#define DbFunctionTryEx(act) \
		const char *__DEBUGTextA = "BR Exception while " act " in " __FUNCTION__; \
		const char *__DEBUGTextB = "STD Exception while " act L" in " __FUNCTION__ ": " std::endl "  "; \
		const char *__DEBUGTextC = "Unknown throw in " act L" in " __FUNCTION__; \
		try {
	
	#define DbFunctionTry \
		const char *__DEBUGTextA = "Exception in " __FUNCTION__; \
		const char *__DEBUGTextB = "STD Exception in " __FUNCTION__ ": "; \
		const char *__DEBUGTextC = "Unknown throw in " __FUNCTION__; \
		try {

	#define DbFunctionCatch \
		catch (BlackRoot::Debug::Exception *e) { \
			throw new BlackRoot::Debug::Exception(__DEBUGTextA, e); \
		} \
		catch (std::exception & ex) { \
			throw new BlackRoot::Debug::Exception((std::stringstream(__DEBUGTextB) << ex.what()).str()); \
		} \
		catch (...) { \
			throw new BlackRoot::Debug::Exception(__DEBUGTextC); \
		} }

	#define DbFunctionCatchEx(extra) \
		catch (BlackRoot::Debug::Exception *e) { \
			throw new BlackRoot::Debug::Exception((std::stringstream(__DEBUGTextA) << std::endl << "  " << extra, e); \
		} \
		catch (std::exception & ex) { \
			throw new BlackRoot::Debug::Exception((std::stringstream(__DEBUGTextB) << ex.what() << std::endl << "  " << extra).str()); \
		} \
		catch (...) { \
			throw new BlackRoot::Debug::Exception(__DEBUGTextC, GenDebugInfo); \
		} }

#else

	#error No code for build

#endif

#ifdef DBGFLAG_MEMORY_PARANOIA
#ifdef _WIN32

	#define DbCheckMemory(x) \
        AssertMsg(_CrtCheckMemory(), L"Memory validity check failed in " x ".");

#endif
#endif

};
};