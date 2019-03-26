#pragma once

enum EngineState {
	STATE_LOCKSCREEN,
	STATE_DASHBOARD,
	STATE_SETTINGS
};

enum SelectType{
	SELECT_OUTLINE,
	SELECT_SIZEDIFF
};

enum Command {
    CMD_Home = 12,
    CMD_Sleepmode = 16,
    CMD_Shutdown = 22,
    CMD_Reboot = 24,
    
};