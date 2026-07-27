#include <stdio.h>
#include <stdlib.h>

#include "core/engine.h"
#include "core/log.h"

int main (){
    if (log_init("logs")) {
        fprintf(stderr, "logger failed to initialize\n");
        return EXIT_FAILURE;
    }
    log_set_console_output(true);  

    Engine engine = {};
    if (engine_init(&engine, "Wizards & Warriors")){
        LOG_ERROR("%s", "Failed to initialize engine");
        return EXIT_FAILURE;
    }
    
    engine_run(&engine);

    engine_shutdown(&engine);
        
    return EXIT_SUCCESS;
}
