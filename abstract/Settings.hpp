#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

// Include system libraries
#include <INIReader.h>

// Settings file abstract class
class Settings
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Settings() = 0; // Declare as pure virtual to prevent instantiation

    // Derived class does not need to have an implementation
    virtual void parseSettings(INIReader &){};
};

// Declare inline destructor to prevent linkage errors
inline Settings::~Settings(){};

#endif