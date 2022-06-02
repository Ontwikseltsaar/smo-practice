#include "al/util.hpp"
#include "fl/ui/page.h"
#include "fl/ui/pages.h"
#include "fl/util.h"
#include "nn/init.h"
#include "rs/util.hpp"

#include <fl/game.h>
#include <fl/server.h>
#include <fl/ui/ui.h>

void fl::ui::PracticeUI::savePosition(PlayerActorHakoniwa& player)
{
    savedTrans = *al::getTrans(&player);
    savedQuat = *al::getQuat(&player);
    saved = true;
}

void fl::ui::PracticeUI::loadPosition(PlayerActorHakoniwa& player)
{
    al::LiveActor* move = &player;
    al::LiveActor* hack = player.mHackKeeper->currentHackActor;

    if (hack)
        move = hack;

    if (disablePuppet && !hack)
        al::offCollide(&player);
    else if (!hack)
        player.startDemoPuppetable();
    else
        al::offCollide(hack);

    al::setTrans(move, savedTrans);
    if (!hack)
        al::updatePoseQuat(move, savedQuat);
    al::setVelocityZero(move);

    if (disablePuppet && !hack)
        al::onCollide(&player);
    else if (!hack)
        player.endDemoPuppetable();
    else
        al::onCollide(hack);
}

void fl::ui::PracticeUI::update(StageScene* stageScene)
{
    Game::instance().setStageScene(stageScene);
    this->mStageScene = stageScene;
    isInGame = true;

    PlayerActorHakoniwa* player = rs::getPlayerActor(stageScene);

    bool holdL = al::isPadHoldL(CONTROLLER_AUTO);

#if SMOVER == 100
    fl::TasHolder::instance().update();
#endif

    if (holdL && al::isPadTriggerRight(CONTROLLER_AUTO)) {
        inputEnabled = !inputEnabled;
        nextFrameNoRightInput = true;
        return;
    } else if
#if SMOVER == 130
        (holdL && al::isPadTriggerLeft(CONTROLLER_AUTO))
#endif
#if SMOVER == 100
        (al::isPadTriggerPressLeftStick(CONTROLLER_AUTO))
#endif
        {
            showMenu = !showMenu;
#if SMOVER == 130
            nextFrameNoLeftInput = true;
#endif
        }

    if (!showMenu || (!inputEnabled && !holdL)) {
        if (teleportEnabled) {
            if (al::isPadTriggerLeft(CONTROLLER_AUTO))
                savePosition(*player);

            if (al::isPadTriggerRight(CONTROLLER_AUTO) && saved)
                loadPosition(*player);
        }
    }

    if (hideShineCounter)
        al::hidePane(stageScene->stageSceneLayout->shineCounter, "TxtShine");
    else
        al::showPane(stageScene->stageSceneLayout->shineCounter, "TxtShine");

#if SMOVER == 130
    if (showMenu)
        menu();
    else
        al::setPaneStringFormat(stageScene->stageSceneLayout->coinCounter, "TxtDebug", " ");
#endif
}

#if SMOVER == 100
void fl::ui::PracticeUI::menu(sead::TextWriter& p)
#endif
#if SMOVER == 130
    void fl::ui::PracticeUI::menu()
#endif
{
    s32 sprintfRv = 0;
    printPos = 0;
    if (!mStageScene)
        return;
    if (showMenu) {
        if (inputEnabled) {
            if (al::isPadTriggerDown(CONTROLLER_AUTO) && curLine + 1 < curMaxLine)
                curLine++;
            if (al::isPadTriggerUp(CONTROLLER_AUTO) && curLine >= 1)
                curLine--;
        }

        if (inputEnabled && curLine == 0) {
            if (al::isPadTriggerRight(CONTROLLER_AUTO) && !nextFrameNoRightInput) {
                if ((*(u8*)&curPage) == (u8)Page::Debug)
                    (*(u8*)&curPage) = (u8)Page::About;
                else
                    (*(u8*)&curPage)++;
            }
            if (al::isPadTriggerLeft(CONTROLLER_AUTO) && !nextFrameNoLeftInput) {
                if ((*(u8*)&curPage) == (u8)Page::About)
                    (*(u8*)&curPage) = (u8)Page::Debug;
                else
                    (*(u8*)&curPage)--;
            }
        }

        auto& page = mPages[*(u8*)&curPage];
        printf("%s%s (%d/%d) %s\n", curLine == 0 ? ">" : "", page.name, (int)curPage + 1, sizeof(mPages) / sizeof(mPages[0]), inputEnabled ? "" : "[Menu Disabled]");
        curMaxLine = 1;
        page.update(*this);
    } else {
        textBuffer[0] = ' ';
        textBuffer[1] = '\0';
    }
#if SMOVER == 130
    al::showPane(getStageScene()->stageSceneLayout->coinCounter, "TxtDebug");
    al::setPaneStringFormat(getStageScene()->stageSceneLayout->coinCounter, "TxtDebug", textBuffer);

    nextFrameNoLeftInput = false;
#elif SMOVER == 100
    p.printf(textBuffer);
#endif
    nextFrameNoRightInput = false;
}

void fl::ui::PracticeUI::toggle(const char* name, bool& value)
{
    cursor(curMaxLine);
    printf("%s: %s\n", name, value ? "Enabled" : "Disabled");
    if (inputEnabled && !nextFrameNoLeftInput && !nextFrameNoRightInput && curLine == curMaxLine && (al::isPadTriggerLeft(CONTROLLER_AUTO) || al::isPadTriggerRight(CONTROLLER_AUTO)))
        value = !value;
    curMaxLine++;
}