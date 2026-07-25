#include <stdio.h>

#include "core/engine.h"
#include "core/log.h"

int main (){
    printf("Hello, World!\n"); 

    if (!log_init("logs")) {
        fprintf(stderr, "logger failed to initialize\n");
    }

    log_set_console_output(false);

    Engine engine = {};
    engine_init(&engine, "Wizards & Woriors");
    
    engine_run(&engine);

    engine_shutdown(&engine);
        
    return 0;
}
