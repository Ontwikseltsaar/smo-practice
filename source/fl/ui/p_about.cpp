#include "fl/ui/pages.h"

void fl::ui::about::update(PracticeUI &ui) {
	ui.printf("Welcome to the Practice Mod!\n\n");
#if SMOVER == 100
	ui.printf("Press left stick to hide/unhide the menu.\n");
#endif
#if SMOVER == 130
	ui.printf("Press L + left D-pad to hide/unhide the menu.\n");
#endif
	ui.printf("Press L + right D-pad to disable/enable the menu.\n\n");
	ui.printf("Created by Fruityloops#8500 and contributors.\n");
	ui.printf("Big thanks to CraftyBoss, bryce_____ and Mars2030!\n");
}