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


/////////////////////////////////////////
// CLog.cpp
//
// by Thibaut "Fury" Tollemer
// 
// Most of the code was taken in the
// Log.cpp/Log.h files of HaCKeR source,
// by Micha�l Schoonbrood :
//      - MadButch@OneCoolDude.Com
//      - http://play.as/madbutch
//

#include "stdafx.h"
#include "CLog.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CLog::CLog()
{
    // Log file is not open yet
    m_bOpen = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CLog::~CLog()
{
    // Close the log file
    if( m_bOpen )
    {
        Close();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Get an instance of CLog (singleton)
CLog& CLog::GetLog()
{
   static CLog rLog;    // r = reference

   return rLog;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Open the Log
bool CLog::Open( const char *pFilename )
{
    // Check if there already is an opened file
    if( m_bOpen )
    {
        // Close it first
        Close();
    }

    // Make sure the file is NOT read-only
    SetFileAttributes( pFilename, FILE_ATTRIBUTE_NORMAL );

    // Open the Log
    m_theLog.open( pFilename );
    if( !m_theLog.fail() )
    {
        // Set indicator bOpen to true
        m_bOpen = true;

        // Get current time
        SYSTEMTIME LocalTime;
        GetLocalTime (&LocalTime);
        
        // Store first log entry
        char FirstLogEntry [128];
        sprintf( FirstLogEntry,                                              // String where to write
                 "==> Log started on %02d/%02d/%02d at %02d:%02d:%02d.\n\n", // Format to use
                 LocalTime.wDay, LocalTime.wMonth, LocalTime.wYear,          // Time numbers to use
                 LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond );
        
        // Write first log entry
        m_theLog.write( FirstLogEntry, strlen( FirstLogEntry ) );
    }
    else
    {
        // Set indicator bOpen to false
        m_bOpen = false;
    }

    return m_bOpen;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Close the Log
bool CLog::Close()
{
    // Close the Log
    if( m_bOpen )
    {
        // Write a blank line
        m_theLog.write( "\n", 1 );
        
        // Get current time
        SYSTEMTIME LocalTime;
        GetLocalTime (&LocalTime);
        
        // Store last log entry
        char LastLogEntry [128];
        sprintf( LastLogEntry,                                             // String where to write
                 "==> Log ended on %02d/%02d/%02d at %02d:%02d:%02d.\n\n", // Format to use
                 LocalTime.wDay, LocalTime.wMonth, LocalTime.wYear,        // Time numbers to use
                 LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond );
        
        // Write last log entry
        m_theLog.write( LastLogEntry, strlen( LastLogEntry ) );

        // Close the file
        m_theLog.close();
    }

    m_bOpen = false;

    return !m_bOpen;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Log the last error, which can be found by calling GetLastError.
void CLog::LogLastError()
{
    // Declare variables
    LPVOID lpMsgBuf;

    //char sCurrentPath[ _MAX_PATH ];
    FormatMessage(  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                    (char *) &lpMsgBuf,
                    0,
                    NULL );

    // Log the error
    WriteLine( (const char *) lpMsgBuf );

    // Free the buffer.
    LocalFree( lpMsgBuf );
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

long CLog::Write( const char *pMessage, ... )
{
    // Format the given string using the given arguments ("..." parameter)
    char Message [2048];
    va_list argList;
    va_start( argList, pMessage );
	vsprintf(Message, pMessage, argList);
    va_end (argList);

    // If the log is open
    if( m_bOpen )
    {
        // If the message starts with a blank line
        if( Message[0] != '\n' )
        {
            // Get current time
            SYSTEMTIME LocalTime;
            GetLocalTime (&LocalTime);
    
            // Store the time string
            char Time [64];
            sprintf ( Time,                 // String where to write
                      "%02d:%02d:%02d\n",   // Format (don't forget '\n' character!)
                      LocalTime.wHour,      // Time numbers to use
                      LocalTime.wMinute, 
                      LocalTime.wSecond );
            
            // Write the time string
            m_theLog.write( Time, strlen( Time ) );

            // Write the space between time and message
            m_theLog.write( "  ", strlen( "  " ) );

            // Write the message
            m_theLog.write( Message, strlen( Message ) );
        }
        // If the message doesn't start with a blank line
        else
        {
            // Write the message without the time
            m_theLog.write( Message, strlen( Message ) );
        }

        m_theLog.flush();
    }
    // If the log is not open
    else
    {
        // Couldn't write to Log!
        return 0;
    }

    return 1;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Write a line into the Log
long CLog::WriteLine( const char *pMessage, ... )
{
    // Format the given string using the given arguments ("..." parameter)
    char Message [2048];
    va_list argList;
    va_start( argList, pMessage );
	vsprintf(Message, pMessage, argList);
    va_end (argList);

    // If the log is open
    if( m_bOpen )
    {
        // If the message starts with a blank line
        if( Message[0] != '\n' )
        {
            // Get current time
            SYSTEMTIME LocalTime;
            GetLocalTime (&LocalTime);
    
            // Store the time string
            char Time [64];
            sprintf ( Time,                 // String where to write
                      "%02d:%02d:%02d",     // Format (don't forget '\n' character!)
                      LocalTime.wHour,      // Time numbers to use
                      LocalTime.wMinute, 
                      LocalTime.wSecond);
            
            // Write the time string
            m_theLog.write( Time, strlen( Time ) );

            // Write the space between time and message
            m_theLog.write( "  ", strlen( "  " ) );

            // Write the message
            m_theLog.write( Message, strlen( Message ) );
        }
        // If the message doesn't start with a blank line
        else
        {
            // Write the message without the time
            m_theLog.write( Message, strlen( Message ) );
        }

        // Write a blank line
        m_theLog.write( "\n", strlen( "\n" ) );

        m_theLog.flush();
    }
    // If the log is not open
    else
    {
        // Couldn't write to Log!
        return 0;
    }

    return 1;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************