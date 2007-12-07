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


///////////////////////////////////////////////
// CWindow.h: interface for the CWindow class.
//

#ifndef __CWINDOW_H__
#define __CWINDOW_H__

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CWindow  
{
protected:

    HWND m_hWnd;
    bool m_Active;

protected:
    
    virtual void OnCreate (HWND hwnd, WPARAM wParam, LPARAM lParam);        // WM_CREATE
    virtual void OnInitDialog (WPARAM wParam, LPARAM lParam);               // WM_INITDIALOG
    virtual void OnActivateApp (WPARAM wParam, LPARAM lParam);              // WM_ACTIVATEAPP
    virtual void OnSize (WPARAM wParam, LPARAM lParam);                     // WM_SIZE
    virtual void OnMove (WPARAM wParam, LPARAM lParam);                     // WM_MOVE
    virtual void OnPaint (WPARAM wParam, LPARAM lParam);                    // WM_PAINT
    virtual void OnKeyDown (WPARAM wParam, LPARAM lParam);                  // WM_KEYDOWN
    virtual void OnKeyUp (WPARAM wParam, LPARAM lParam);                    // WM_KEYUP
    virtual void OnTimer (WPARAM wParam, LPARAM lParam);                    // WM_TIMER
    virtual void OnCommand (WPARAM wParam, LPARAM lParam);                  // WM_COMMAND
    virtual bool OnSysCommand (WPARAM wParam, LPARAM lParam);               // WM_SYSCOMMAND
    virtual void OnClose (WPARAM wParam, LPARAM lParam);                    // WM_CLOSE
    virtual void OnDestroy (WPARAM wParam, LPARAM lParam);                  // WM_DESTROY
    virtual void OnWindowActive (void);

public:

    CWindow (HINSTANCE hInstance, const char *pWindowTitle, int IconResourceID = -1);
    virtual ~CWindow (void);
    void SetClientSize (int ClientWidth, int ClientHeight);
    void ShowWindow (void);
    void MessagePump (void);
    LRESULT CALLBACK WinProc (unsigned int msg, WPARAM wParam, LPARAM lParam);
};  

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


#endif