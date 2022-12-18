// git clone https://github.com/nothings/stb.git
// sudo cp stb/stb_image.h /usr/local/include/
#include "util/logger.h"
#include "application.h"


int main(void)
{
    logger::log::fatal("A %s level message", "fatal");
    logger::log::error("A %s level message", "error");
    logger::log::warn("A %s level message", "warn");
    logger::log::info("A %s level message", "info");
    logger::log::debug("A %s level message", "debug");
    logger::log::trace("A %s level message", "trace");

    Application app;
    app.init();
    app.run();
    app.end();
}