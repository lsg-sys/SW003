/**
 * 
 *      ___                                  
 *     /   ""=-,_                   ,  _.    
 *     |         .E=-,_  __          \\/     
 *      \        \  ``, '7 "-,     //' \\    
 *       \        \    ``+ 1  "=,//'         
 *        \        \\     ",.||/ \           
 *          \        ",     ,^:'"; \\        
 *           \\        "-//    ', "-_l       
 *             "1      //"\_     '"_-.       
 *               ""_//`     "+,    '='\      
 *                //\:         `";_  `=|     
 *              //'   `\_          `""`=,    
 *           //'         "=_            ^\   
 *         //'              "=_          '\  
 *       //'                   ""=_      '|  
 *                                 '""'==*'  
 * 
 * @author lsg
 * @file SmartWatches.h
 */

#ifndef SmartWatches_H
#define SmartWatches_H

#include "FsmBase.hpp"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */

/* USER_CODE_END */

enum class SmartWatchesStates : uint32_t {
    Sleep = 0,
    Running,
    Initialize
};

enum class SmartWatchesEvents : uint32_t {
    None = 0,
    KeyboardInitializationIsComplete,
    PanelInitializationIsComplete,
    TriggerSleep,
    CmdInitializationIsComplete,
    PowerOn,
    CmdInput,
    MemsInitializationIsComplete,
    Timeout,
    Sync,
    MemsInput,
    KeyboardInput
};

class SmartWatches : public FsmBase<SmartWatchesStates, SmartWatchesEvents> {
public:
    using States = SmartWatchesStates;
    using Events = SmartWatchesEvents;

    static SmartWatches& getInstance() {
        static SmartWatches instance;
        return instance;
    }

    SmartWatches(const SmartWatches&) = delete;
    SmartWatches& operator=(const SmartWatches&) = delete;

private:
    SmartWatches() : FsmBase<SmartWatchesStates, SmartWatchesEvents>() {
        currentState = initialState();
        internalEvent = initialInternalEvent();
        transitionTable = createTransitionTable();
        stateHandlersTable = createStateHandlersTable();
    }
    States initialState() const override { return States::Initialize; }
    Events initialInternalEvent() const override { return Events::PowerOn; }

    void Initialize_PowerOn_callback(Msg msg);
    void Initialize_CmdInitializationIsComplete_callback(Msg msg);
    void Initialize_PanelInitializationIsComplete_callback(Msg msg);
    void Initialize_MemsInitializationIsComplete_callback(Msg msg);
    void Initialize_KeyboardInitializationIsComplete_callback(Msg msg);
    void Running_KeyboardInput_callback(Msg msg);
    void Running_MemsInput_callback(Msg msg);
    void Running_CmdInput_callback(Msg msg);
    void Running_TriggerSleep_callback(Msg msg);
    void Running_Timeout_callback(Msg msg);
    void Sleep_KeyboardInput_callback(Msg msg);
    void Sleep_MemsInput_callback(Msg msg);
    void Sleep_CmdInput_callback(Msg msg);
    void Sleep_Sync_callback(Msg msg);

std::vector<Transition> createTransitionTable() override {
    return {
        {{States::Initialize, Events::PowerOn}, States::Initialize, [this](Msg msg){this->Initialize_PowerOn_callback(msg);}},
        {{States::Initialize, Events::CmdInitializationIsComplete}, States::Initialize, [this](Msg msg){this->Initialize_CmdInitializationIsComplete_callback(msg);}},
        {{States::Initialize, Events::PanelInitializationIsComplete}, States::Initialize, [this](Msg msg){this->Initialize_PanelInitializationIsComplete_callback(msg);}},
        {{States::Initialize, Events::MemsInitializationIsComplete}, States::Initialize, [this](Msg msg){this->Initialize_MemsInitializationIsComplete_callback(msg);}},
        {{States::Initialize, Events::KeyboardInitializationIsComplete}, States::Running, [this](Msg msg){this->Initialize_KeyboardInitializationIsComplete_callback(msg);}},
        {{States::Running, Events::KeyboardInput}, States::Running, [this](Msg msg){this->Running_KeyboardInput_callback(msg);}},
        {{States::Running, Events::MemsInput}, States::Running, [this](Msg msg){this->Running_MemsInput_callback(msg);}},
        {{States::Running, Events::CmdInput}, States::Running, [this](Msg msg){this->Running_CmdInput_callback(msg);}},
        {{States::Running, Events::TriggerSleep}, States::Sleep, [this](Msg msg){this->Running_TriggerSleep_callback(msg);}},
        {{States::Running, Events::Timeout}, States::Sleep, [this](Msg msg){this->Running_Timeout_callback(msg);}},
        {{States::Sleep, Events::KeyboardInput}, States::Running, [this](Msg msg){this->Sleep_KeyboardInput_callback(msg);}},
        {{States::Sleep, Events::MemsInput}, States::Running, [this](Msg msg){this->Sleep_MemsInput_callback(msg);}},
        {{States::Sleep, Events::CmdInput}, States::Running, [this](Msg msg){this->Sleep_CmdInput_callback(msg);}},
        {{States::Sleep, Events::Sync}, States::Sleep, [this](Msg msg){this->Sleep_Sync_callback(msg);}},
    };
}
    void en_Sleep();
    void du_Sleep();
    void ex_Sleep();
    void en_Running();
    void du_Running();
    void ex_Running();
    void en_Initialize();
    void du_Initialize();
    void ex_Initialize();
std::vector<StateHandlers> createStateHandlersTable() override {
    return {
        {[this](){this->en_Sleep();}, [this](){this->du_Sleep();} , [this](){this->ex_Sleep();}},
        {[this](){this->en_Running();}, [this](){this->du_Running();} , [this](){this->ex_Running();}},
        {[this](){this->en_Initialize();}, [this](){this->du_Initialize();} , [this](){this->ex_Initialize();}},
    };
}
std::string eventToString(Events event) const override {
    switch (event) {
        case Events::None: return "None";
        case Events::KeyboardInitializationIsComplete: return "KeyboardInitializationIsComplete";
        case Events::PanelInitializationIsComplete: return "PanelInitializationIsComplete";
        case Events::TriggerSleep: return "TriggerSleep";
        case Events::CmdInitializationIsComplete: return "CmdInitializationIsComplete";
        case Events::PowerOn: return "PowerOn";
        case Events::CmdInput: return "CmdInput";
        case Events::MemsInitializationIsComplete: return "MemsInitializationIsComplete";
        case Events::Timeout: return "Timeout";
        case Events::Sync: return "Sync";
        case Events::MemsInput: return "MemsInput";
        case Events::KeyboardInput: return "KeyboardInput";
        default: return "unknown";
    }
}
std::string stateToString(States state) const override {
    switch (state) {
        case States::Sleep: return "Sleep";
        case States::Running: return "Running";
        case States::Initialize: return "Initialize";
        default: return "unknown";
    }
}
    int recvMessage(Msg &msg) const override;
    void logOutput(std::string &output) const override;
};
/* USER_CODE_BEGIN CUSTOM_CLASS */

/* USER_CODE_END */

#endif // SmartWatches_H