/**
 * Copyright © 2009-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * StMoviePlayer program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StMoviePlayer program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __StMoviePlayerStrings_h_
#define __StMoviePlayerStrings_h_

namespace StMoviePlayerStrings {

    // strings' ids in the language dictionary
    enum {
        SWAP_LR_OFF = 1002,
        SWAP_LR_ON = 1003,
        BTN_SRC_FORMAT = 1004,
        FILE_VIDEO_OPEN = 1015,
        VIDEO_PLAYPAUSE = 1020,
        VIDEO_LIST = 1021,
        VIDEO_LIST_PREV = 1022,
        VIDEO_LIST_NEXT = 1023,
        FULLSCREEN = 1029,

        // Root -> Media menu
        MENU_MEDIA = 1100,
        MENU_MEDIA_OPEN_MOVIE = 1101,
        MENU_MEDIA_SAVE_SNAPSHOT_AS = 1102,
        MENU_MEDIA_SRC_FORMAT = 1103,
        MENU_MEDIA_AL_DEVICE  = 1104,
        MENU_MEDIA_SHUFFLE    = 1105,
        MENU_MEDIA_RECENT     = 1106,
        MENU_MEDIA_QUIT = 1109,

        // Root -> Media menu -> Open File menu
        MENU_MEDIA_OPEN_MOVIE_1 = 1110,
        MENU_MEDIA_OPEN_MOVIE_2 = 1111,

        // Root -> Media menu -> Source Format menu
        MENU_SRC_FORMAT_AUTO = 1130,
        MENU_SRC_FORMAT_MONO = 1131,
        MENU_SRC_FORMAT_CROSS_EYED = 1132,
        MENU_SRC_FORMAT_PARALLEL = 1133,
        MENU_SRC_FORMAT_OVERUNDER_RL = 1134,
        MENU_SRC_FORMAT_OVERUNDER_LR = 1135,
        MENU_SRC_FORMAT_INTERLACED = 1136,
        MENU_SRC_FORMAT_ANA_RC = 1137,
        MENU_SRC_FORMAT_ANA_RB = 1138,
        MENU_SRC_FORMAT_ANA_YB = 1139,
        MENU_SRC_FORMAT_PAGEFLIP = 1140,
        MENU_SRC_FORMAT_TILED_4X = 1141,

        // Root -> Media menu -> Recent files menu
        MENU_MEDIA_RECENT_CLEAR = 1160,

        // Root -> View menu
        MENU_VIEW = 1200,
        MENU_VIEW_DISPLAY_MODE  = 1201,
        MENU_VIEW_FULLSCREEN    = 1202,
        MENU_VIEW_RESET         = 1203,
        MENU_VIEW_SWAP_LR       = 1204,
        MENU_VIEW_DISPLAY_RATIO = 1205,
        MENU_VIEW_TEXFILTER     = 1206,
        MENU_VIEW_GAMMA         = 1207,

        MENU_VIEW_DISPLAY_MODE_STEREO       = 1210,
        MENU_VIEW_DISPLAY_MODE_LEFT         = 1211,
        MENU_VIEW_DISPLAY_MODE_RIGHT        = 1212,
        MENU_VIEW_DISPLAY_MODE_PARALLEL     = 1213,
        MENU_VIEW_DISPLAY_MODE_CROSSYED     = 1214,
        MENU_VIEW_DISPLAY_MODE_OVERUNDER_LR = 1215,
        MENU_VIEW_DISPLAY_MODE_OVERUNDER_RL = 1216,

        MENU_VIEW_TEXFILTER_NEAREST = 1260,
        MENU_VIEW_TEXFILTER_LINEAR  = 1261,
        MENU_VIEW_TEXFILTER_BLEND   = 1262,

        // Root -> Audio menu
        MENU_AUDIO = 1300,
        //MENU_AUDIO_NONE = 1301,

        // Root -> Subtitles menu
        MENU_SUBTITLES = 1350,

        // Root -> Output menu
        MENU_CHANGE_DEVICE  = 1400, // Root -> Output -> Change Device menu
        MENU_ABOUT_RENDERER = 1401,
        MENU_FPS            = 1402,

        // Root -> Output -> FPS Control menu
        MENU_FPS_VSYNC      = 1420,
        MENU_FPS_METER      = 1421,
        MENU_FPS_BOUND      = 1422,

        // Root -> Help menu
        MENU_HELP = 1500,
        MENU_HELP_ABOUT   = 1501,
        MENU_HELP_UPDATES = 1502,
        MENU_HELP_LICENSE = 1503,
        MENU_HELP_LANGS   = 1504,
        MENU_HELP_BLOCKSLP= 1505,
        MENU_HELP_USERTIPS= 1506,

        // Root -> Help -> Check for updates menu
        MENU_HELP_UPDATES_NOW   = 1520,
        MENU_HELP_UPDATES_DAY   = 1521,
        MENU_HELP_UPDATES_WEEK  = 1522,
        MENU_HELP_UPDATES_YEAR  = 1523,
        MENU_HELP_UPDATES_NEVER = 1524,

        // Root -> Help -> Block sleeping
        MENU_HELP_BLOCKSLP_NEVER    = 1550,
        MENU_HELP_BLOCKSLP_ALWAYS   = 1551,
        MENU_HELP_BLOCKSLP_PLAYBACK = 1552,
        MENU_HELP_BLOCKSLP_FULLSCR  = 1553,

        // Open/Save dialogs
        DIALOG_OPEN_FILE       = 2000,
        DIALOG_OPEN_LEFT       = 2001,
        DIALOG_OPEN_RIGHT      = 2002,

        DIALOG_SAVE_SNAPSHOT   = 2010,
        DIALOG_NOTHING_TO_SAVE = 2011,
        DIALOG_NO_SNAPSHOT     = 2012,

        // About dialog
        ABOUT_DPLUGIN_NAME     = 3000,
        ABOUT_VERSION          = 3001,
        ABOUT_DESCRIPTION      = 3002,
        UPDATES_NOTIFY         = 3003,

    };
};

#endif //__StMoviePlayerStrings_h_
