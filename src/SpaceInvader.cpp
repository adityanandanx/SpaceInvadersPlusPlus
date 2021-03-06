/**
 * @file SpaceInvader.cpp
 * @date 1-Mar-2021
 */



#include "orx.h"
#include <iostream>

orxOBJECT* player;
orxOBJECT* gun;
orxOBJECT* bug1;
orxOBJECT* bug2;
orxOBJECT* bugs[50];
orxVIEWPORT* viewPort;
orxCAMERA* camera;

orxFLOAT WW, WH;

void orxFASTCALL Update(const orxCLOCK_INFO *_pstClockInfo, void *_pContext)
{
    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
}



orxSTATUS orxFASTCALL Init()
{
    // Create the viewport
    viewPort = orxViewport_CreateFromConfig("MainViewport");
    camera = orxViewport_GetCamera(viewPort);
    orxDisplay_GetScreenSize(&WW, &WH);

    // Create the scene
    orxObject_CreateFromConfig("Scene");
    player = orxObject_CreateFromConfig("Player");
    gun = orxObject_GetChild(player);
    orxObject_Enable(gun, false);
    bug1 = orxObject_CreateFromConfig("Bug");
    bug2 = orxObject_CreateFromConfig("Bug");
    { // Positioning
        orxVECTOR pos = {-80, -80};
        orxObject_SetPosition(bug2, &pos);
        for (int j=0; j <= 5; j++)
        {
            for (int i=0; i <= 5; i++)
            {
                bugs[i] = orxObject_CreateFromConfig("Bug");
                pos.fX = -80 + 20*i;
                orxObject_SetPosition(bugs[i], &pos);
            }
            pos.fY += 20;
        }

    }

    //bug3 = orxObject_CreateFromConfig("Bug");
    //bug4 = orxObject_CreateFromConfig("Bug");

    // Register the Update function to the core clock
    orxClock_Register(orxClock_Get(orxCLOCK_KZ_CORE), Update, orxNULL, orxMODULE_ID_MAIN, orxCLOCK_PRIORITY_NORMAL);

    // Done!
    return orxSTATUS_SUCCESS;
}


orxVECTOR PlayerPosition;
orxVECTOR force, forcePoint;
float forceMagnitude = .1f;
float maxSpeed = 100.0f;
orxFLOAT playerAngleInRadians = 0.0f;
orxVECTOR mousePos;

orxSTATUS orxFASTCALL Run()
{
    // Setting the Velocity, Getting the position and setting the rotation


    orxObject_ApplyImpulse(player, &force, &forcePoint);        // setting player's speed
    orxObject_GetPosition(player, &PlayerPosition);     // getting the player position
    // mouse stuff
    orxMouse_GetPosition(&mousePos);                    // getting the mouse position
    mousePos.fX = mousePos.fX - WW/2;                    // mouse is attached to the window and not to the game
    mousePos.fY = WH/2 - mousePos.fY;                    // mouse is attached to the window and not to the game
    // now the angle part
    playerAngleInRadians = orxMath_ATan(mousePos.fX, mousePos.fY);
    // std::cout << mousePos.fX << ", " << mousePos.fY << std::endl;
    std::cout << force.fX << ", " << force.fY << "\n";
    orxObject_SetRotation(player, playerAngleInRadians);

    // Movement
    if (orxInput_IsActive("goLeft"))
    {
        force.fX = -forceMagnitude;
    }

    else if (orxInput_IsActive("goRight"))
    {
        force.fX = forceMagnitude;
    }

    else if (orxInput_IsActive("goUp"))
    {
        force.fY = -forceMagnitude;
    }

    else if (orxInput_IsActive("goDown"))
    {
        force.fY = forceMagnitude;
    }
    else
    {
        force.fX = 0.0f;
        force.fY = 0.0f;
    }

    // Following the player
    orxCamera_SetPosition(camera, &PlayerPosition);

    // Limiting the velocity
    //if (PlayerVelocity.fX > maxSpeed) {PlayerVelocity.fX = maxSpeed;}
    //if (PlayerVelocity.fX < -maxSpeed) {PlayerVelocity.fX = -maxSpeed;}

    //if (PlayerVelocity.fY > maxSpeed) {PlayerVelocity.fY = maxSpeed;}
    //if (PlayerVelocity.fY < -maxSpeed) {PlayerVelocity.fY = -maxSpeed;}


    if (orxInput_IsActive("fire") || orxMouse_IsButtonPressed(orxMOUSE_BUTTON_LEFT))
    {
        orxObject_Enable(gun, true);
    }
    else
    {
        orxObject_Enable(gun, false);
    }

    // Return orxSTATUS_FAILURE to instruct orx to quit
    return orxSTATUS_SUCCESS;
}



void orxFASTCALL Exit()
{
    // Let Orx clean all our mess automatically. :)
}



orxSTATUS orxFASTCALL Bootstrap()
{
    // Add a config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}



int main(int argc, char **argv)
{
    // Set the bootstrap function to provide at least one resource storage before loading any config files
    orxConfig_SetBootstrap(Bootstrap);

    // Execute our game
    orx_Execute(argc, argv, Init, Run, Exit);

    // Done!
    return EXIT_SUCCESS;
}
