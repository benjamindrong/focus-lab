//
// Created by Benjamin Drong on 5/10/26.
//

#ifndef FOCUSLAB_INPUT_H
#define FOCUSLAB_INPUT_H



enum class InputType {
    KeyPress,
    KeyRelease
};

struct InputEvent {
    InputType type;
    int key; // abstract key code
};



#endif //FOCUSLAB_INPUT_H
