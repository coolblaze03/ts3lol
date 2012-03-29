/**********************************************************************

Filename    :   LobbyDataProvider.h
Content     :   Data provider base used by the Lobby application
Created     :   7/31/2009
Authors     :   Prasad Silva
Copyright   :   (c) 2009 Scaleform Corp. All Rights Reserved.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_LobbyDataProvider_H
#define INC_LobbyDataProvider_H

#include <time.h>

#include "GThreads.h"

#include "LobbyTypes.h"


// The data provider is the interface between the lobby application and
// the lobby data. This data can exist online (ex: GameSpy central servers)
// or offline, and the data provider is intended to encapsulate this
// heterogeneity.

// Commands are passed from the application to the data provider to
// process data or time intensive tasks, and results are in turn passed
// back to the application when the tasks have been completed. Results
// are also generated for events or messages that arrive from the data 
// store without initiation from the application.


// Define the following to disable Online mode (removes dependencies to
// networking libraries) and only enable Offline mode.
//
// #define LOBBY_NO_ONLINE
//
// Note: Some solutions may have this define included by default in the 
//       project properties.


//////////////////////////////////////////////////////////////////////////
// Result type generated by the data provider. The Lobby application
// listens to these results and notifies the UI appropriately.
//
struct LDPResult : public GNewOverrideBase<GStat_Default_Mem>, 
                   public GListNode<LDPResult>
{
    enum ResultType
    {
        R_UserLoggedIn,
        R_UserLoggedOut,

        R_BuddyArrived,
        R_BuddyStatusChanged,
        R_BuddyRemoved,
        R_BuddyRequestReceived,

        R_ChatMessageArrived,

        R_ServerAdded,
        R_ServerUpdated,
        R_ServerUpdateFailed,
        R_ServerListUpdateStarted,
        R_ServerListUpdateStopped,
        R_ServerListUpdateCompleted,
        R_ServerListCleared,

        R_Error,
        R_LoginErrorReceived,
        R_ChatErrorReceived,
        R_BuddyErrorReceived,
        R_ServerErrorReceived,
    };

    LobbyError                  Error;
    GString                     UserName;
    GString                     Message;
    GPtr<LobbyBuddy>            pBuddy;
    GPtr<LobbyServer>           pServer;
    ResultType                  Type;
    int                         ProfileID;            
    time_t                      TimeStamp;        
    LobbyServer::NetworkType    Network;    

    LDPResult(ResultType type) : Type(type) {}
};


//////////////////////////////////////////////////////////////////////////
// Commands generated by the lobby application. These commands are 
// processed by the data provider implementation.
//
struct LDPCommand : public GNewOverrideBase<GStat_Default_Mem>, 
                    public GListNode<LDPCommand>
{
    enum CommandType
    {
        C_Login,
        C_CreateAccount,
        C_Logout,
        C_GetBuddyInfo,
        C_SendChatMessage,
        C_RemoveBuddy,
        C_AddBuddy,
        C_AcceptBuddyRequest,
        C_DenyBuddyRequest,
        C_PopulateServerList,
        C_RefreshServer,
        C_StopServerUpdating,

        // Special value to indicate a command subclass
        ExtendedSet,
    };

    GString                     UserName;
    GString                     Password;
    GString                     Email;
    GString                     Message;
    GPtr<LobbyServer>           pServer;
    CommandType                 Type;
    int                         ProfileID;
    LobbyServer::NetworkType    Network;
    bool                        bClearServers;


    LDPCommand(CommandType type) : Type(type) {}
};


//////////////////////////////////////////////////////////////////////////
// Thread safe queue used to store results and commands as they are 
// generated and processed in different threads.
//
template <class T> class LDPQueue
{
public:
    typedef T ValueType;

    class Condition
    {
    public:
        virtual bool operator () (ValueType* t) const = 0;
    };

    LDPQueue() {}

    GINLINE void        Lock()      { Mutex.Lock(); }
    GINLINE void        Unlock()    { Mutex.Unlock(); }
    
    GINLINE GMutex&     GetLock()   { return Mutex; }

    GINLINE bool        IsEmpty()           { return Container.IsEmpty(); }
    GINLINE ValueType*  PopFront()               
    { 
        ValueType* p = Container.GetFirst(); 
        Container.Remove(p); 
        return p;
    }
    GINLINE void        PushBack(ValueType* v)  { Container.PushBack(v); }
    GINLINE void        Erase(const Condition& c)
    {
        ValueType* data = Container.GetFirst();
        while (!Container.IsNull(data))
        {
            ValueType* temp = data;
            data = Container.GetNext(data);
            if (c(temp))
            {
                Container.Remove(temp);
                delete temp;
            }
        }        
    }

private:
    GList<ValueType>    Container;
    GMutex              Mutex;
};


//////////////////////////////////////////////////////////////////////////
// Listener interface for data provider results. The lobby application
// (LobbyController) implements this interface.
//
class LDPResultListener
{
public:
    virtual void    OnDataProviderResult(LDPResult* r) = 0;
};


//////////////////////////////////////////////////////////////////////////
// Data provider interface
//
// The data provider provides the data necessary for the Lobby application 
// to function. It is also intended to wrap any third party SDKs, such as 
// GameSpy, to provide a common interface for data retrieval. 
//
// Data intensive tasks and network access is intended to be handled in
// the worker thread that should be spawned on initialization. The
// Run() method should call the Think() method to process any
// commands (asynchronous if in multi-threaded mode). 
//
class LobbyDataProvider : public GThread
{
public:
    // The connection state of the data provider to the network
    enum ConnectionStateType
    {
        CS_Opening  = 0x01,
        CS_Opened,
        CS_Closed,
        CS_Closing,
        CS_Failed   = 0x10,
    };

    LobbyDataProvider();
    virtual             ~LobbyDataProvider();

    // **** Custom data interface. These methods correspond to actions 
    //      performed in the user interface that affect the application.
    //      The base implementation of these methods add corresponding
    //      command objects to the command queue.

    // Connect to the account servers
    virtual void        ConnectNick(const char *uniqueNick, 
                                    const char *password);
    // Create a new account and connect to the account servers
    virtual void        ConnectNewUser(const char *uniqueNick, 
                                       const char *password,
                                       const char* Email);
    // Logout from the main servers
    virtual void        Logout();

    // Send a chat message to a specific buddy
    virtual void        SendChatMessage(const LobbyBuddy* pbuddy, 
                                        const char* msg);

    // Add a user to the buddy list
    virtual void        AddBuddy(const char* buddyName, 
                                 const char* message);

    // Remove a buddy from the buddy list
    virtual void        RemoveBuddy(const LobbyBuddy* pbuddy);

    // Accept a buddy request from a remote user
	// KevinJ: Modified to also take buddyName as an input parameters, so this can be returned to the user with LDPCommand::C_AcceptBuddyRequest
    virtual void        AcceptBuddyRequest(int id, const char* buddyName);

    // Deny a buddy request from a remote user
	// KevinJ: Modified to also take buddyName as an input parameters, so this can be returned to the user with LDPCommand::C_DenyBuddyRequest
    virtual void        DenyBuddyRequest(int id, const char* buddyName);

	// KevinJ: This was added because "CANNOT NOTIFY SINCE BUDDY PANEL IS INACTIVE" is returned if you update the buddy panel too early after logging in
	// buddyPanelIsAvailable is true when the buddy panel becomes active
	virtual void		OnBuddyPanelStateChange(bool buddyPanelIsAvailable) {}

    // Retrieve a new server list
    virtual void        PopulateNewServerList(LobbyServer::NetworkType net);
    
    // Stop retrieving the server list
    virtual void        StopUpdating(LobbyServer::NetworkType net, 
                                     bool clearList = true);

    // Refresh a specific server to retrieve its latest information
    virtual void        RefreshServer(LobbyServer::NetworkType net, 
                                      LobbyServer* pserver);

    // Get the total number of servers to be retrieved. Subclasses
    // must implement this method since the base does not include any
    // information about server counts.
    virtual UPInt       GetTotalServerCount(LobbyServer::NetworkType net)   = 0;

    // Return data limits
    virtual UInt        GetMaxUniqueNickLength()            { return 20; }
    virtual UInt        GetMaxPasswordLength()              { return 20; }
    virtual UInt        GetMaxStatusStringLength()          { return 35; }
    virtual UInt        GetMaxBuddyRequestReasonLength()    { return 128; }
    virtual UInt        GetMaxErrorStringLength()           { return 128; }

protected:

    // Entry point for threaded mode
    virtual SInt        Run()                                               = 0;
    // Process a specific command. This virtual method allows data provider 
    // implementations to handle commands differently.
    virtual void        ProcessCommand(LDPCommand* cmd)                     = 0;

    // ****

public:
    void                Stop();
    void                ExecuteSynchronousTasks();
    void                AddResultListener(LDPResultListener* l);

protected:
    // True if the worker thread is alive
    volatile bool       bAlive;
    // True if the application wants the worker thread to die
    volatile bool       bStopRequested;

    // Current connection state
    ConnectionStateType             ConnectionState;

    GArray<LDPResultListener*>      ResultListeners;

    // *** Result and command queues
    LDPQueue<LDPCommand>            Commands;
    LDPQueue<LDPResult>             HighPriorityResults;
    LDPQueue<LDPResult>             LowPriorityResults;
    // Temporary storage to offload results from the worker threads
    // when processing results from the results queues. This frees
    // up the queues from the main thread and allows the worker
    // to continue processing.
    GList<LDPResult>                ResultsToProcess;

    // *** Helpers to queue results and commands
    GINLINE void        QueueHighPriorityResult(LDPResult* result)
    {
        GMutex::Locker Locker(&HighPriorityResults.GetLock());
        HighPriorityResults.PushBack(result);
    }
    GINLINE void        QueueLowPriorityResult(LDPResult* result)
    {
        GMutex::Locker Locker(&LowPriorityResults.GetLock());
        LowPriorityResults.PushBack(result);
    }
    GINLINE void        QueueCommand(LDPCommand* command)
    {
        GMutex::Locker Locker(&Commands.GetLock());
        Commands.PushBack(command);
    }

    // Process the results and command queues
    void                ProcessResults();
    void                ProcessCommands();

    // Broadcast a new result to all registered result listeners
    void                BroadcastResult(LDPResult* r);
};

#endif  // INC_LobbyDataProvider_H

