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
 * @file Panel.h
 */

#ifndef Panel_H
#define Panel_H

#include "FsmBase.hpp"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */

/* USER_CODE_END */

enum class PanelStates : uint32_t {
    Sleep = 0,
    Running,
    Initialize
};

enum class PanelEvents : uint32_t {
    None = 0,
    TriggerSleep,
    TriggerPanelInitialization,
    TriggerWakeup,
    Sync,
    KeyInput
};

class Panel : public FsmBase<PanelStates, PanelEvents> {
public:
    using States = PanelStates;
    using Events = PanelEvents;

    static Panel& getInstance() {
        static Panel instance;
        return instance;
    }

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;

private:
    Panel() : FsmBase<PanelStates, PanelEvents>() {
        currentState = initialState();
        internalEvent = initialInternalEvent();
        transitionTable = createTransitionTable();
        stateHandlersTable = createStateHandlersTable();
    }
    States initialState() const override { return States::Initialize; }
    Events initialInternalEvent() const override { return Events::None; }

    void Initialize_TriggerPanelInitialization_callback(Msg msg);
    void Running_KeyInput_callback(Msg msg);
    void Running_TriggerSleep_callback(Msg msg);
    void Sleep_TriggerWakeup_callback(Msg msg);
    void Sleep_Sync_callback(Msg msg);

std::vector<Transition> createTransitionTable() override {
    return {
        {{States::Initialize, Events::TriggerPanelInitialization}, States::Running, [this](Msg msg){this->Initialize_TriggerPanelInitialization_callback(msg);}},
        {{States::Running, Events::KeyInput}, States::Running, [this](Msg msg){this->Running_KeyInput_callback(msg);}},
        {{States::Running, Events::TriggerSleep}, States::Sleep, [this](Msg msg){this->Running_TriggerSleep_callback(msg);}},
        {{States::Sleep, Events::TriggerWakeup}, States::Running, [this](Msg msg){this->Sleep_TriggerWakeup_callback(msg);}},
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
        case Events::TriggerSleep: return "TriggerSleep";
        case Events::TriggerPanelInitialization: return "TriggerPanelInitialization";
        case Events::TriggerWakeup: return "TriggerWakeup";
        case Events::Sync: return "Sync";
        case Events::KeyInput: return "KeyInput";
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

#endif // Panel_H