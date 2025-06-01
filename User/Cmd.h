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
 * @file Cmd.h
 */

#ifndef Cmd_H
#define Cmd_H

#include "FsmBase.hpp"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */
#include "input_key_def.h"
/* USER_CODE_END */

enum class CmdStates : uint32_t {
    Running = 0,
    Pause,
    Initialize
};

enum class CmdEvents : uint32_t {
    None = 0,
    TriggerPause,
    IRQInput,
    TriggerCmdInitialization,
    TriggerResume,
    Sync
};

class Cmd : public FsmBase<CmdStates, CmdEvents> {
public:
    using States = CmdStates;
    using Events = CmdEvents;

    static Cmd& getInstance() {
        static Cmd instance;
        return instance;
    }

    Cmd(const Cmd&) = delete;
    Cmd& operator=(const Cmd&) = delete;

private:
    Cmd() : FsmBase<CmdStates, CmdEvents>() {
        currentState = initialState();
        internalEvent = initialInternalEvent();
        transitionTable = createTransitionTable();
        stateHandlersTable = createStateHandlersTable();
    }
    States initialState() const override { return States::Initialize; }
    Events initialInternalEvent() const override { return Events::None; }

    void Initialize_TriggerCmdInitialization_callback(Msg msg);
    void Running_IRQInput_callback(Msg msg);
    void Running_TriggerPause_callback(Msg msg);
    void Pause_TriggerResume_callback(Msg msg);
    void Pause_Sync_callback(Msg msg);

std::vector<Transition> createTransitionTable() override {
    return {
        {{States::Initialize, Events::TriggerCmdInitialization}, States::Running, [this](Msg msg){this->Initialize_TriggerCmdInitialization_callback(msg);}},
        {{States::Running, Events::IRQInput}, States::Running, [this](Msg msg){this->Running_IRQInput_callback(msg);}},
        {{States::Running, Events::TriggerPause}, States::Pause, [this](Msg msg){this->Running_TriggerPause_callback(msg);}},
        {{States::Pause, Events::TriggerResume}, States::Running, [this](Msg msg){this->Pause_TriggerResume_callback(msg);}},
        {{States::Pause, Events::Sync}, States::Pause, [this](Msg msg){this->Pause_Sync_callback(msg);}},
    };
}
    void en_Running();
    void du_Running();
    void ex_Running();
    void en_Pause();
    void du_Pause();
    void ex_Pause();
    void en_Initialize();
    void du_Initialize();
    void ex_Initialize();
std::vector<StateHandlers> createStateHandlersTable() override {
    return {
        {[this](){this->en_Running();}, [this](){this->du_Running();} , [this](){this->ex_Running();}},
        {[this](){this->en_Pause();}, [this](){this->du_Pause();} , [this](){this->ex_Pause();}},
        {[this](){this->en_Initialize();}, [this](){this->du_Initialize();} , [this](){this->ex_Initialize();}},
    };
}
std::string eventToString(Events event) const override {
    switch (event) {
        case Events::None: return "None";
        case Events::TriggerPause: return "TriggerPause";
        case Events::IRQInput: return "IRQInput";
        case Events::TriggerCmdInitialization: return "TriggerCmdInitialization";
        case Events::TriggerResume: return "TriggerResume";
        case Events::Sync: return "Sync";
        default: return "unknown";
    }
}
std::string stateToString(States state) const override {
    switch (state) {
        case States::Running: return "Running";
        case States::Pause: return "Pause";
        case States::Initialize: return "Initialize";
        default: return "unknown";
    }
}
    int recvMessage(Msg &msg) const override;
    void logOutput(std::string &output) const override;
};
/* USER_CODE_BEGIN CUSTOM_CLASS */

/* USER_CODE_END */

#endif // Cmd_H