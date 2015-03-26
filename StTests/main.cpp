/**
 * Copyright © 2011-2015 Kirill Gavrilov <kirill@sview.ru>
 *
 * StTests program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StTests program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <StStrings/stConsole.h>
#include <StThreads/StProcess.h>
#include <StFile/StFolder.h>

#include "StTestMutex.h"
#include "StTestGlBand.h"
#include "StTestEmbed.h"
#include "StTestImageLib.h"
#include "StTestGlStress.h"

int main(int , char** ) { // force console output
#if defined(_WIN32)
    setlocale(LC_ALL, ".OCP"); // we set default locale for console output
#endif

    // setup environment variables
    const StString ST_ENV_NAME_STCORE_PATH =
    #if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
        "StCore64";
    #else
        "StCore32";
    #endif
    const StString aProcessPath = StProcess::getProcessFolder();
    StProcess::setEnv(ST_ENV_NAME_STCORE_PATH, aProcessPath);
    if(StFolder::isFolder(aProcessPath + "textures")) {
        StProcess::setEnv("StShare", aProcessPath);
    } else if(StFolder::isFolder(aProcessPath + ".." + SYS_FS_SPLITTER + "textures")) {
        StProcess::setEnv("StShare", aProcessPath + ".." + SYS_FS_SPLITTER);
    }

    st::cout << stostream_text("This application performs some synthetic tests\n");

    StArrayList<StString> anArgs = StProcess::getArguments();
    const StString ST_TEST_MUTICES = "mutex";
    const StString ST_TEST_GLBAND  = "glband";
    const StString ST_TEST_GLHANG  = "glhang";
    const StString ST_TEST_EMBED   = "embed";
    const StString ST_TEST_IMAGE   = "image";
    const StString ST_TEST_ALL     = "all";
    size_t aFound = 0;
    for(size_t anArgId = 0; anArgId < anArgs.size(); ++anArgId) {
        const StString& aParam = anArgs[anArgId];
        if(aParam == ST_TEST_MUTICES) {
            // mutex speed test
            StTestMutex aMutices;
            aMutices.perform();
            ++aFound;
        } else if(aParam == ST_TEST_GLBAND) {
            // gl <-> cpu trasfer speed test
            StTestGlBand aGlBand;
            aGlBand.perform();
            ++aFound;
        } else if(aParam == ST_TEST_GLHANG) {
            // gl stress test
            StTestGlStress aGlHang;
            aGlHang.perform();
            ++aFound;
        } else if(aParam == ST_TEST_EMBED) {
            // StWindow embed to native window
            StTestEmbed anEmbed;
            anEmbed.perform();
            ++aFound;
        } else if(aParam == ST_TEST_IMAGE) {
            // image libraries performance tests
            if(++anArgId >= anArgs.size()) {
                st::cout << stostream_text("Broken syntax - image file awaited!\n");
                break;
            }

            StTestImageLib anImage(anArgs[anArgId]);
            anImage.perform();
            ++aFound;
        } else if(aParam == ST_TEST_ALL) {
            // mutex speed test
            StTestMutex aMutices;
            aMutices.perform();

            // gl <-> cpu trasfer speed test
            StTestGlBand aGlBand;
            aGlBand.perform();

            // StWindow embed to native window
            StTestEmbed anEmbed;
            anEmbed.perform();

            ++aFound;
            break;
        }
    }

    // show help
    if(aFound == 0) {
        st::cout << stostream_text("No test selected. Options:\n")
                 << stostream_text("  all    - execute all available tests\n")
                 << stostream_text("  mutex  - mutex speed test\n")
                 << stostream_text("  glband - gl <-> cpu trasfer speed test\n")
                 << stostream_text("  glhang - gl stress test\n")
                 << stostream_text("  embed  - test window embedding\n")
                 << stostream_text("  image fileName - test image libraries\n");
    }

    st::cout << stostream_text("Press any key to exit...") << st::SYS_PAUSE_EMPTY;
    return 0;
}
