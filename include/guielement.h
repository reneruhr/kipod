#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <string>
#include "eventmanager.h"

class GUIelement
{
    static int id(){
        static int id = 0;
        return id++;
    }
protected:
    const std::string IMGUIname;

public:
    GUIelement(std::string name): IMGUIname("##" + name + std::to_string( id() )){}
};

class ButtonLRarrow : public GUIelement
{
    int counter = 0;
    std::string description;
    float spacing = 0;
public:
    ButtonLRarrow(std::string description);

    void left(EventManager *eventmanager, Event event);
    void right(EventManager *eventmanager, Event event);
};


class MatrixView : public GUIelement
{

public:
    MatrixView(std::string description) : GUIelement(description){}
    void showMatrix(mat4& matrix);
};










#endif // GUIELEMENT_H
