#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "core.h"

enum class EventType{
    TranslateCamera,
    TranslateModel,
    RotateModel,
    DebugMode,
    WireframeMode,
    EmissiveMode,
    LightMode,
    SceneUpdate,
    BoxMode,
    NormalMode,
    CameraMode,
    ClippingMode,
    TextureMode
};

enum class MoveDirection{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACKWARD
};

enum class RotateDirection{
    ROLL,
    YAW,
    PITCH
};

enum class Mode{
    OFF,
    ON,
    SWITCH
};


class EventData{
    public:
    MoveDirection direction;
    RotateDirection rotateDiraction;
    Mode mode;
    float amount;
    EventData(MoveDirection direction): direction(direction){}
    EventData(Mode mode): mode(mode){}
    EventData(RotateDirection rotateDiraction, float amount):
        rotateDiraction(rotateDiraction), amount(amount){}
};


class Event
{
    EventType type;
    EventData data;

public:
    Event(EventType type, EventData data): type(type), data(data){}
    EventType getType(){ return type; }
    EventData getData(){ return data; }
    bool handled = false;
};

class Listener
{
public:
    Listener(){}
    virtual void processEvent(Event& event){}
    virtual void processEvent(Event* event){}
};

class EventManager
{
    std::vector<Event> events;
    std::vector<Listener*> listeners;

public:
    EventManager();

    void requestUpdate();
    void dispatch(Event event);
    void process();
    void addListener(Listener* id);
};



#endif // EVENTMANAGER_H
