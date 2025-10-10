#include "SoundManager.h"

#include "AK/AkFilePackageLowLevelIODeferred.h"
#include <cassert>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#define AKSOUNDENGINE_DLL

#ifndef AK_OPTIMIZED
    #include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

// Bank file names
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_MAIN L"MainSoundBank.bnk"

const AkGameObjectID DEFAULT_LISTENER = 0;
CAkFilePackageLowLevelIODeferred g_lowLevelIO;

bool SoundManager::initialize()
{
    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);
 
    if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success )
    {
        assert( ! "Could not create the memory manager." );
        return false;
    }


    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings( stmSettings );
    
    // Customize the Stream Manager settings here.
    
    if ( !AK::StreamMgr::Create( stmSettings ) )
    {
        assert( ! "Could not create the Streaming Manager" );
        return false;
    }


    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );
    
    // Customize the streaming device settings here.
    
    // CAkFilePackageLowLevelIODeferred::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    if ( g_lowLevelIO.Init( deviceSettings ) != AK_Success )
    {
        assert( ! "Could not create the streaming device and Low-Level I/O system" );
        return false;
    }

    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings( initSettings );
    AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );
 
    if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
    {
        assert( ! "Could not initialize the Sound Engine." );
        return false;
    }

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings( musicInit );
        
    if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
    {
        assert( ! "Could not initialize the Music Engine." );
        return false;
    }

#ifndef AK_OPTIMIZED
    //
    // Initialize communications (not in release build!)
    //
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings( commSettings );
    if ( AK::Comm::Init( commSettings ) != AK_Success )
    {
        assert( ! "Could not initialize communication." );
        return false;
    }
#endif // AK_OPTIMIZED

    AK::SoundEngine::RegisterGameObj(DEFAULT_LISTENER, "Default Listener");
    AK::SoundEngine::SetDefaultListeners(&DEFAULT_LISTENER, 1);

    return true;
}

void SoundManager::loadBanks()
{
    g_lowLevelIO.SetBasePath( AKTEXT("Wwise/GeneratedSoundBanks/Windows/") );
    AK::StreamMgr::SetCurrentLanguage( AKTEXT("English(US)") );

    AkBankID bankID; // Not used. These banks can be unloaded with their file name.
    AKRESULT eResult = AK::SoundEngine::LoadBank( BANKNAME_INIT, bankID );
    assert( eResult == AK_Success );
    eResult = AK::SoundEngine::LoadBank( BANKNAME_MAIN, bankID );
    assert( eResult == AK_Success );
}

void SoundManager::processAudio()
{
    AK::SoundEngine::RenderAudio();
}

void SoundManager::close()
{
#ifndef AK_OPTIMIZED
    AK::Comm::Term();
#endif // AK_OPTIMIZED
    
    AK::MusicEngine::Term();
    AK::SoundEngine::Term();
    
    g_lowLevelIO.Term();
    
    if ( AK::IAkStreamMgr::Get() )
        AK::IAkStreamMgr::Get()->Destroy();

    AK::MemoryMgr::Term();
}
