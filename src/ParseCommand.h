#ifndef PARSECOMMAND_H
#define PARSECOMMAND_H

#include "Command.h"

/**
 * @brief Function parses CpSync command arguments, passed from main. 
 *        ! not idiotproof. 
 * 
 * @param argc count of arguments in argv array
 * @param argv command arguments
 * @return struct CpSyncCmdData 
 */
struct CpSyncCmdData ParseCommand(int32_t argc, const char* const* argv);

#endif