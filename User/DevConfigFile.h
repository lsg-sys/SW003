#ifndef __DEVCONFIGFILE_H
#define __DEVCONFIGFILE_H

#include "utility/format.h"

class DevConfigFile
{
public:
    static DevConfigFile& getInstance() {
        static DevConfigFile instance;
        return instance;
    }

    DevConfigFile(const DevConfigFile&) = delete;
    DevConfigFile& operator=(const DevConfigFile&) = delete;

    int init();
    int SaveConfig();
    int LoadConfig();
    int GetConfig(SmartWatchesConfig_t *config);
    int SetConfig(SmartWatchesConfig_t *config);

private:
    DevConfigFile() {
        config.date_time.s.year  = 2025;
        config.date_time.s.month = 2;
        config.date_time.s.day   = 3;
        config.date_time.s.hour  = 9;
        config.date_time.s.min   = 30;
        config.date_time.s.sec   = 40;

        config.brightness        = 50; /* 0-100 */
        config.sleep_index       = 3;   /* 3: 30 seconds */

        config.language          = 1;   /* 0: Simplified Chinese, 1: English */

        config.theme_dark        = 0;   /* 0: Light, 1: Dark */
        config.theme_color       = 0x698d1f;

        config.clock_style       = 0;
    }

    SmartWatchesConfig_t config;
};

#endif /* __DEVCONFIGFILE_H */