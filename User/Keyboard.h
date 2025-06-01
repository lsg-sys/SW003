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
 * @file Keyboard.h
 */

#ifndef Keyboard_H
#define Keyboard_H

#include "FsmBase.hpp"

/* USER_CODE_BEGIN CUSTOM_INCLUDES */

/* USER_CODE_END */

enum class KeyboardStates : uint32_t {
    Running = 0,
    Pause,
    Initialize
};

enum class KeyboardEvents : uint32_t {
    None = 0,
    TriggerPause,
    TriggerResume,
    Sync,
    TriggerKeyboardInitialization
};

class Keyboard : public FsmBase<KeyboardStates, KeyboardEvents> {
public:
    using States = KeyboardStates;
    using Events = KeyboardEvents;

    static Keyboard& getInstance() {
        static Keyboard instance;
        return instance;
    }

    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

private:
    Keyboard() : FsmBase<KeyboardStates, KeyboardEvents>() {
        currentState = initialState();
        internalEvent = initialInternalEvent();
        transitionTable = createTransitionTable();
        stateHandlersTable = createStateHandlersTable();
    }
    States initialState() const override { return States::Initialize; }
    Events initialInternalEvent() const override { return Events::None; }

    void Initialize_TriggerKeyboardInitialization_callback(Msg msg);
    void Running_TriggerPause_callback(Msg msg);
    void Pause_TriggerResume_callback(Msg msg);
    void Pause_Sync_callback(Msg msg);

std::vector<Transition> createTransitionTable() override {
    return {
        {{States::Initialize, Events::TriggerKeyboardInitialization}, States::Running, [this](Msg msg){this->Initialize_TriggerKeyboardInitialization_callback(msg);}},
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
        case Events::TriggerResume: return "TriggerResume";
        case Events::Sync: return "Sync";
        case Events::TriggerKeyboardInitialization: return "TriggerKeyboardInitialization";
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

#endif // Keyboard_H