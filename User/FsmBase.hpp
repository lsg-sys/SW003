#ifndef FSM_BASE_HPP_
#define FSM_BASE_HPP_

#include <vector>
#include <string>
#include <functional>

/* User include */
#include <cstring>

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "usart.h"

template <typename State, typename Event>
class FsmBase {

public:

    struct s_Message {
        Event event;
        uint32_t arg;
    };
    using Msg = struct s_Message;

    /* 
     * Each state transition has a handler to call.
     */
    struct Transition {
    	std::pair<State, Event> from;
        State to;
        std::function<void(Msg)> callback;
    };
    /* 
     * Every state has three handlers: enter, duration and exit.
     */
    struct StateHandlers {
        std::function<void()> enter;
        std::function<void()> duration;
        std::function<void()> exit;
    };

    void processEvent(Msg msg) {
        for (const auto& transition : transitionTable) {
            /* Check if the transition is valid, otherwise skip */
            if (transition.from == std::make_pair(currentState, msg.event)) {
                
                std::string logStr;
                logStr = stateToString(currentState) + " ---(" + eventToString(msg.event) + ")---> " + stateToString(transition.to) + "\n"; logOutput(logStr);

                bool isNewState = currentState != transition.to;

                /* Exit the current state */
                if (isNewState && stateHandlersTable[static_cast<unsigned int>(currentState)].exit) {
                    logStr = stateToString(currentState) + " exit\n"; logOutput(logStr);
                    stateHandlersTable[static_cast<unsigned int>(currentState)].exit();
                }
                /* call the transition callback */
                if (transition.callback) {
                    logStr = "callback\n"; logOutput(logStr);
                    transition.callback(msg);
                }
                /* Enter the next state */
                if (isNewState && stateHandlersTable[static_cast<unsigned int>(transition.to)].enter) {
                    logStr = stateToString(transition.to) + " enter\n"; logOutput(logStr);
                    stateHandlersTable[static_cast<unsigned int>(transition.to)].enter();
                }
                /* Set the new state */
                currentState = transition.to;
                break;
            }
        }
    }

    State getCurrentState() const { return currentState; }
    bool internalEventsExist() const { return internalEvent != Event::None; }
    Event getInternalEvent() const { return internalEvent; }
    void setInternalEvent(Event event) { internalEvent = event; }
    void clearInternalEvents() { internalEvent = Event::None; }

    void run() {
        while(true) {
            /*  Only one internal event occurs at a time. */
            if (internalEventsExist()) {
                /* Events are fetched and then cleared to avoid conflicts with 
                 * new internal events that may arise later.
                 */
                Msg msg = {getInternalEvent(), 0};
                clearInternalEvents();
                processEvent(msg);
            } else {
                /* Handle external event */
                Msg msg;
                if (0 == recvMessage(msg))
                    processEvent(msg);
            }

            if (stateHandlersTable[static_cast<unsigned int>(getCurrentState())].duration)
                stateHandlersTable[static_cast<unsigned int>(getCurrentState())].duration();
        }
    }

protected:
    virtual State initialState() const = 0;
    virtual Event initialInternalEvent() const = 0;
    virtual std::vector<Transition> createTransitionTable() = 0;
    virtual std::vector<StateHandlers> createStateHandlersTable() = 0;

    virtual int recvMessage(Msg& msg) const = 0;
    virtual std::string stateToString(State state) const = 0;
    virtual std::string eventToString(Event event) const = 0;
    virtual void logOutput(std::string& output) const = 0;

    State currentState;
    Event internalEvent;
    std::vector<Transition> transitionTable;
    std::vector<struct StateHandlers> stateHandlersTable;
};

#endif /* FSM_BASE_HPP_ */
