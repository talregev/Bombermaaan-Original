/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer

    This file is part of Bombermaaan.

    Bombermaaan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bombermaaan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bombermaaan.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************************/


//////////////////////
// CMainInput.h

#ifndef __CMAININPUT_H__
#define __CMAININPUT_H__

#include "CDirectInput.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define NUMBER_OF_MENU_CONTROLS               6       //!< Number of menu controls

#define MAX_PLAYER_INPUT_NAME_LENGTH    16      //!< Maximum length of a string containing a player input name

#define MENU_UP         0       //!< Index of the menu "up" control in the menu controls array
#define MENU_DOWN       1       //!< Index of the menu "down" control in the menu controls array
#define MENU_LEFT       2       //!< Index of the menu "left" control in the menu controls array
#define MENU_RIGHT      3       //!< Index of the menu "right" control in the menu controls array
#define MENU_PREVIOUS   4       //!< Index of the menu "previous" control in the menu controls array
#define MENU_NEXT       5       //!< Index of the menu "next" control in the menu controls array

#define NUMBER_OF_SYSTEM_CONTROLS         2

#define SYSTEM_PAUSE    0
#define SYSTEM_BREAK    1

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SMenuControl
{
    int     Key;            //!< Index of the control on the device
    float   PressTime;      //!< Time (in seconds) that elapsed since the control is pressed
    float   RepeatTime;     //!< Time (in seconds) that elapsed since the control last repeated
    bool    Active;         //!< Is the control active? (taking control repetition into account)
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SSystemControl
{
    int     Key;            //!< Index of the control on the device
    bool    State;
    bool    Pressing;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CMainInput
{
private:

    CTimer*         m_pTimer;
    CDirectInput*   m_pDirectInput;
    SMenuControl    m_MenuControls [NUMBER_OF_MENU_CONTROLS];
    SSystemControl  m_SystemControls [NUMBER_OF_SYSTEM_CONTROLS];

public:

                    CMainInput (void);
                    ~CMainInput (void);
    inline void     SetDirectInput (CDirectInput* pDirectInput);
    inline void     SetTimer (CTimer* pTimer);
    void            Create (void);
    void            Destroy (void);
    void            Open (void);
    bool            IsOpened (void);
    void            Close (void);
    void            Update (void); 
    inline bool     TestUp (void);
    inline bool     TestDown (void);
    inline bool     TestLeft (void);
    inline bool     TestRight (void);
    inline bool     TestPrevious (void);
    inline bool     TestNext (void);
    inline bool     TestPause (void);
    inline bool     TestBreak (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMainInput::SetDirectInput (CDirectInput* pDirectInput)
{
    m_pDirectInput = pDirectInput;
}

inline void CMainInput::SetTimer (CTimer* pTimer)
{
    m_pTimer = pTimer;
}

inline bool CMainInput::TestUp (void)
{
    return m_MenuControls[MENU_UP].Active;
}

inline bool CMainInput::TestDown (void)
{
    return m_MenuControls[MENU_DOWN].Active;
}

inline bool CMainInput::TestLeft (void)
{
    return m_MenuControls[MENU_LEFT].Active;
}

inline bool CMainInput::TestRight (void)
{
    return m_MenuControls[MENU_RIGHT].Active;
}

inline bool CMainInput::TestPrevious (void)
{
    return m_MenuControls[MENU_PREVIOUS].Active;
}

inline bool CMainInput::TestNext (void)
{   
    return m_MenuControls[MENU_NEXT].Active;
}

inline bool CMainInput::TestPause (void)
{
    return m_SystemControls[SYSTEM_PAUSE].State;
}

inline bool CMainInput::TestBreak (void)
{
    return m_SystemControls[SYSTEM_BREAK].State;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CMAININPUT_H__