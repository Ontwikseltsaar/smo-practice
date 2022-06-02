#include "fl/ui/pages.h"
#include "game/GameData/GameDataFunction.h"

void fl::ui::mooninfo::update(PracticeUI &ui) {
	StageScene *stageScene = ui.getStageScene();
	ui.toggle("Hide Original Moon Counter", ui.hideShineCounter);

#if (SMOVER == 100)
	s32 currShine = GameDataFunction::getCurrentShineNum(*stageScene->mHolder);
	s32 totalShine = GameDataFunction::getTotalShineNum(*stageScene->mHolder, -1);
	s32 unlockShine = GameDataFunction::findUnlockShineNumByWorldId(0, *stageScene->mHolder, GameDataFunction::getCurrentWorldId(*stageScene->mHolder));
	ui.printf("Current Moons: %d\n", currShine);
	ui.printf("Total Moons: %d\n", totalShine);
	ui.printf("Required Moons: %d", unlockShine);
#endif
}