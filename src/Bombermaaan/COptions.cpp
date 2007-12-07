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


/////////////////////////
// COptions.cpp

#include "stdafx.h"
#include "COptions.h"
#include "CInput.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define TIMESTART_MINUTES   1
#define TIMESTART_SECONDS   0

#define TIMEUP_MINUTES      0
#define TIMEUP_SECONDS      35

#define CONFIGURATION_KEYBOARD_1      0
#define CONFIGURATION_KEYBOARD_2      1
#define CONFIGURATION_JOYSTICK_1      2

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

COptions::COptions (void)
{
    m_LevelsData = NULL;
    m_LevelsName = NULL;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

COptions::~COptions (void)
{
    Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

COptions& COptions::operator = (COptions& Copy)
{
    m_TimeStartMinutes = Copy.m_TimeStartMinutes;
    m_TimeStartSeconds = Copy.m_TimeStartSeconds;
    m_TimeUpMinutes = Copy.m_TimeUpMinutes;
    m_TimeUpSeconds = Copy.m_TimeUpSeconds;

    int i, j, k;

    for (i = 0 ; i < MAX_PLAYERS ; i++)
    {
        m_BomberType[i] = Copy.m_BomberType[i];
        m_PlayerInput[i] = Copy.m_PlayerInput[i];
    }

    m_PlayerCount = Copy.m_PlayerCount;
    m_BattleCount = Copy.m_BattleCount;
    m_DisplayMode = Copy.m_DisplayMode;

    for (i = 0 ; i < MAX_PLAYER_INPUT ; i++)
        for (j = 0 ; j < NUM_CONTROLS ; j++)
            m_Control[i][j] = Copy.m_Control[i][j];
    
    m_Level = Copy.m_Level;
    m_NumberOfLevels = Copy.m_NumberOfLevels;
    
    AllocateLevels(m_NumberOfLevels);

    for (i = 0 ; i < m_NumberOfLevels ; i++)
    {
        m_LevelsName[i] = new char [strlen(Copy.m_LevelsName[i]) + 1];
        strcpy(m_LevelsName[i], Copy.m_LevelsName[i]);

        for (j = 0 ; j < ARENA_WIDTH ; j++)
            for (k = 0 ; k < ARENA_HEIGHT ; k++)
                m_LevelsData[i][j][k] = Copy.m_LevelsData[i][j][k];
    }

	return *this;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool COptions::Create (void)
{
    // Load configuration file
    if (!LoadConfiguration())
        return false;

    // Load game levels data and names
    if (!LoadLevels())
        return false;
    
    // Everything went ok.
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void COptions::Destroy (void)
{
    //-------------------------------
    // Free all level data and names
    //-------------------------------

    if (m_LevelsData != NULL)
    {
        for (int i = 0 ; i < m_NumberOfLevels ; i++)
        {
            for (int j = 0 ; j < ARENA_WIDTH ; j++)
                delete [] m_LevelsData[i][j];

            delete [] m_LevelsData[i];
            
            if (m_LevelsName != NULL)
                delete [] m_LevelsName[i];
        }

        delete [] m_LevelsData;
        m_LevelsData = NULL;
        
        if (m_LevelsName != NULL)
        {
            delete [] m_LevelsName;
            m_LevelsName = NULL;
        }

        m_NumberOfLevels = 0;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void COptions::SaveBeforeExit (void)
{
   // Try to open the configuration file
    FILE* pConfigFile = fopen("Bomberman.cfg", "wb");
    
    // Write configuration to file
    if (pConfigFile != NULL)
    {
        WriteData(pConfigFile);
        fclose(pConfigFile);
    } 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool COptions::LoadConfiguration (void)
{
    // Try to open the configuration file
    FILE* pConfigFile = fopen("Bomberman.cfg", "rb");
    
    // If the configuration file doesn't exist
    if (pConfigFile == NULL)
    {
        //----------------------------------
        // Set default configuration values
        //----------------------------------

        int i;

        m_TimeUpMinutes = TIMEUP_MINUTES;
        m_TimeUpSeconds = TIMEUP_SECONDS;
        m_TimeStartMinutes = TIMESTART_MINUTES;
        m_TimeStartSeconds = TIMESTART_SECONDS;
        m_BomberType[0] = BOMBERTYPE_MAN;
        m_BomberType[1] = BOMBERTYPE_MAN;
        m_BomberType[2] = BOMBERTYPE_OFF;
        m_BomberType[3] = BOMBERTYPE_OFF;
        m_BomberType[4] = BOMBERTYPE_OFF;
        m_Level = 0;

        for (i = 0 ; i < MAX_PLAYERS ; i++)
            m_PlayerInput[i] = 0;
        
        m_BattleCount = 3;
        m_DisplayMode = DISPLAYMODE_WINDOWED;

        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_UP]      = KEYBOARD_NUMPAD8;
        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_DOWN]    = KEYBOARD_NUMPAD5;
        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_LEFT]    = KEYBOARD_NUMPAD4;
        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_RIGHT]   = KEYBOARD_NUMPAD6;
        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_ACTION1] = KEYBOARD_PRIOR;
        m_Control[CONFIGURATION_KEYBOARD_1][CONTROL_ACTION2] = KEYBOARD_HOME;

        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_UP]      = KEYBOARD_R;
        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_DOWN]    = KEYBOARD_F;
        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_LEFT]    = KEYBOARD_D;
        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_RIGHT]   = KEYBOARD_G;
        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_ACTION1] = KEYBOARD_2;
        m_Control[CONFIGURATION_KEYBOARD_2][CONTROL_ACTION2] = KEYBOARD_1;

        for (i = CONFIGURATION_JOYSTICK_1 ; i < MAX_PLAYER_INPUT ; i++)
        {
            m_Control[i][CONTROL_UP]      = JOYSTICK_UP;
            m_Control[i][CONTROL_DOWN]    = JOYSTICK_DOWN;
            m_Control[i][CONTROL_LEFT]    = JOYSTICK_LEFT;
            m_Control[i][CONTROL_RIGHT]   = JOYSTICK_RIGHT;
            m_Control[i][CONTROL_ACTION1] = JOYSTICK_BUTTON(0);
            m_Control[i][CONTROL_ACTION2] = JOYSTICK_BUTTON(1);
        }

        m_Level = 0;

        // Create the configuration file
        pConfigFile = fopen("Bomberman.cfg", "wb");
        
        // If creation failed
        if (pConfigFile == NULL)
        {
            // Log failure
            theLog.WriteLine ("Options         => !!! Could not create Bomberman.cfg.");

            return false;
        }
        
        // Write the configuration file
        WriteData(pConfigFile);
    }
    // If the configuration file exists
    else
    {
        // Read the configuration file.
        ReadData(pConfigFile);
    }

    // The configuration file is not needed anymore
    fclose(pConfigFile);

    // Success
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void COptions::ReadData (FILE* pConfigFile)
{
    // Read each configuration value in the file
    fread(&m_TimeUpMinutes, sizeof(int), 1, pConfigFile);
    fread(&m_TimeUpSeconds, sizeof(int), 1, pConfigFile);
    fread(&m_TimeStartMinutes, sizeof(int), 1, pConfigFile);
    fread(&m_TimeStartSeconds, sizeof(int), 1, pConfigFile);
    fread(&m_DisplayMode, sizeof(EDisplayMode), 1, pConfigFile);
    fread(&m_BattleCount, sizeof(int), 1, pConfigFile);
    fread(m_BomberType, sizeof(EBomberType), MAX_PLAYERS, pConfigFile);
    fread(m_PlayerInput, sizeof(int), MAX_PLAYERS, pConfigFile);
    fread(m_Control, sizeof(int), MAX_PLAYER_INPUT * NUM_CONTROLS, pConfigFile);
    fread(&m_Level, sizeof(int), 1, pConfigFile);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void COptions::WriteData (FILE* pConfigFile)
{
    // Write each configuration value in the file
    fwrite(&m_TimeUpMinutes, sizeof(int), 1, pConfigFile);
    fwrite(&m_TimeUpSeconds, sizeof(int), 1, pConfigFile);
    fwrite(&m_TimeStartMinutes, sizeof(int), 1, pConfigFile);
    fwrite(&m_TimeStartSeconds, sizeof(int), 1, pConfigFile);
    fwrite(&m_DisplayMode, sizeof(EDisplayMode), 1, pConfigFile);
    fwrite(&m_BattleCount, sizeof(int), 1, pConfigFile);
    fwrite(m_BomberType, sizeof(EBomberType), MAX_PLAYERS, pConfigFile);
    fwrite(m_PlayerInput, sizeof(int), MAX_PLAYERS, pConfigFile);
    fwrite(m_Control, sizeof(int), MAX_PLAYER_INPUT * NUM_CONTROLS, pConfigFile);
    fwrite(&m_Level, sizeof(int), 1, pConfigFile);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void COptions::AllocateLevels (int NumberOfLevels)
{
    ASSERT(m_LevelsData == NULL);
    ASSERT(m_LevelsName == NULL);
    
    m_LevelsData = new EBlockType** [NumberOfLevels];

    for (int i = 0 ; i < NumberOfLevels ; i++)
    {
        m_LevelsData[i] = new EBlockType* [ARENA_WIDTH];

        for (int j = 0 ; j < ARENA_WIDTH ; j++)
            m_LevelsData[i][j] = new EBlockType [ARENA_HEIGHT];
    }    

    m_LevelsName = new char* [NumberOfLevels];
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool COptions::LoadLevels (void)
{
    FILE* File;
    long FindHandle;
    _finddata_t FindData;
    int CurrentLevel = 0;
            
    //-------------------------------------------
    // Determine number of level files available
    //-------------------------------------------
    
    m_NumberOfLevels = 0;
    FindHandle = _findfirst("Levels\\*.txt", &FindData);
    
    if (FindHandle != -1)
    {
        do 
        {
            m_NumberOfLevels++; 
        }
        while (_findnext(FindHandle, &FindData) != -1);
    }

    _findclose(FindHandle);

    //---------------------
    // Check for a problem
    //---------------------

    // If there is no level
    if (m_NumberOfLevels == 0)
    {
        // Log failure
        theLog.WriteLine ("Options         => !!! There should be at least 1 level.");

        return false;
    }

    // If the level number we read in the cfg file is invalid compared to the number of existing levels
    if (m_Level >= m_NumberOfLevels)
    {
        // Select the first level
        m_Level = 0;
    }

    //-----------------------------------
    // Allocate data and name for levels
    //-----------------------------------

    AllocateLevels(m_NumberOfLevels);
    
    //------------------------------------------------------
    // Rebrowse all levels but actually load them this time
    //------------------------------------------------------
    
    bool StopReadingFile;    
    FindHandle = _findfirst("Levels\\*.txt", &FindData);
    
    if (FindHandle != -1)
    {
        do
        {
            // Make the level filename to load
            char LevelFileName[128];
            sprintf(LevelFileName, "Levels\\%s", FindData.name);

            // Allocate and store level name
            m_LevelsName[CurrentLevel] = new char [strlen(FindData.name) + 1];
            strcpy(m_LevelsName[CurrentLevel], FindData.name);

            // Open the existing level file for reading
            File = fopen(LevelFileName, "rt");

            // If it failed
            if (File == NULL)
            {
                // Stop loading levels
                break;
            }

            StopReadingFile = false;

            // For each line of characters to read
            for (int y = 0 ; y < ARENA_HEIGHT ; y++)
            {
                // Buffer where we'll store one line of characters. We'll read the two EOL characters as well.
                char Line[ARENA_WIDTH + 1];
            
                // Read one line of characters (including the EOL chars)
                int ReadBytes = fread(Line, sizeof(char), (ARENA_WIDTH + 1), File);

                // Check if all the characters were read and that the two last bytes are EOL characters
                if (ReadBytes < ARENA_WIDTH + 1 || Line[ARENA_WIDTH] != 10)
                {
                    // Log there is a problem
                    theLog.WriteLine ("Options         => !!! Level file %s is incorrect (%d, %d, %d).", LevelFileName, ReadBytes, Line[ARENA_WIDTH], Line[ARENA_WIDTH + 1]);
                
                    // Close the level file
                    fclose(File);

                    // Stop loading levels
                    StopReadingFile = true;
                    break;
                }

                // For each character representing a block in this line
                for (int x = 0 ; x < ARENA_WIDTH ; x++)
                {
                    // According to the character value, store the corresponding block type in the current position and level
                    switch(Line[x])
                    {
                        case '*' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_HARDWALL;    break;
                        case '-' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_RANDOM;      break;
                        case ' ' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_FREE;        break;
                        case '1' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_WHITEBOMBER; break;
                        case '2' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_BLACKBOMBER; break;
                        case '3' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_REDBOMBER;   break;
                        case '4' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_BLUEBOMBER;  break;
                        case '5' : m_LevelsData[CurrentLevel][x][y] = BLOCKTYPE_GREENBOMBER; break;
                        default  : 
                        {
                            // Log there is a problem
                            theLog.WriteLine ("Options         => !!! Level file %s is incorrect (unknown character %c).", LevelFileName, Line[x]);
                        
                            // Close the level file
                            fclose(File);

                            // Stop loading levels
                            StopReadingFile = true;
                            break;
                        }
                    }
                }

                // If there was a problem
                if (StopReadingFile)
                {
                    // Stop reading this level file
                    break;
                }
            }

            // Close the level file
            fclose(File);
        
            // If there wasn't any problem
            if (!StopReadingFile)
            {
                theLog.WriteLine ("Options         => Level file %s was successfully loaded.", LevelFileName);
            }
            // If there was a problem
            else
            {
                // Stop loading levels
                break;
            }

            CurrentLevel++;
        }
        while (_findnext(FindHandle, &FindData) != -1);
    }

    _findclose(FindHandle);

    // If we had to stop then there is a problem.
    if (StopReadingFile)
        return false;

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************