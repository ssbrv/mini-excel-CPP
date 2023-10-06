#ifndef __CONST_H__
#define __CONST_H__

#include "../../libs/json.hpp"
#include <string>

extern std::string CONFIG_FILE_NAME;
extern std::string FAILED_TO_OPEN_CONFIG_FILE_ERR;

extern std::string WELCOME_MESSAGE;

/* operator names */

extern std::string ADDITION_CONCATENATION_OPERATOR_NAME;
extern std::string SUBTRACTION_OPERATOR_NAME;
extern std::string MULTIPLICATION_OPERATOR_NAME;
extern std::string DIVISION_OPERATOR_NAME;
extern std::string SIN_OPERATOR_NAME;
extern std::string COS_OPERATOR_NAME;
extern std::string LOG10_OPERATOR_NAME;
extern std::string LN_OPERATOR_NAME;
extern std::string LOG2_OPERATOR_NAME;
extern std::string AVG_OPERATOR_NAME;
extern std::string REPETITION_OPERATOR_NAME;
extern std::string PADL_OPERATOR_NAME;
extern std::string PADR_OPERATOR_NAME;

/* error messages */

extern std::string NEGATIVE_ARGUMENT_ERR;
extern std::string ZERO_ARGUMENT_ERR;
extern std::string OVERFLOW_ERR;
extern std::string INVALID_ARITY_ERR;

/* command names */

extern std::string HELP_COMMAND_NAME;
extern std::string OPLIST_COMMAND_NAME;
extern std::string MAXSIZE_COMMAND_NAME;
extern std::string CURRSIZE_COMMAND_NAME;
extern std::string CREATE_COMMAND_NAME;
extern std::string IMPORT_COMMAND_NAME;
extern std::string ASSIGN_COMMAND_NAME;
extern std::string FORMULA_COMMAND_NAME;
extern std::string PRINT_COMMAND_NAME;
extern std::string EXPORT_COMMAND_NAME;
extern std::string EXIT_COMMAND_NAME;

/* command descriptions */

extern std::string HELP_COMMAND_DESCRIPTION;
extern std::string OPLIST_COMMAND_DESCRIPTION;
extern std::string MAXSIZE_COMMAND_DESCRIPTION;
extern std::string CURRSIZE_COMMAND_DESCRIPTION;
extern std::string CREATE_COMMAND_DESCRIPTION;
extern std::string IMPORT_COMMAND_DESCRIPTION;
extern std::string ASSIGN_COMMAND_DESCRIPTION;
extern std::string FORMULA_COMMAND_DESCRIPTION;
extern std::string PRINT_COMMAND_DESCRIPTION;
extern std::string EXPORT_COMMAND_DESCRIPTION;
extern std::string EXIT_COMMAND_DESCRIPTION;

/* notes */

extern std::string CELL_INDEX_NOTE;
extern std::string FILE_NAME_NOTE;
extern std::string CELL_RANGE_NOTE;

/* command output */

extern std::string HELP_COMMAND_OUTPUT;
extern std::string OPLIST_COMMAND_OUTPUT;
extern std::string MAXSIZE_COMMAND_OUTPUT;
extern std::string CURRSIZE_COMMAND_OUTPUT;
extern std::string CREATE_COMMAND_OUTPUT;
extern std::string IMPORT_COMMAND_OUTPUT;
extern std::string ASSIGN_COMMAND_OUTPUT;
extern std::string FORMULA_COMMAND_OUTPUT;
extern std::string PRINT_COMMAND_OUTPUT;
extern std::string EXPORT_COMMAND_OUTPUT;
extern std::string EXIT_COMMAND_OUTPUT;

/* constant names */

extern std::string PI_CONSTANT_NAME;
extern std::string E_CONSTANT_NAME;
extern std::string USERNAME_CONSTANT_NAME;

/* constant values */

extern std::string PI_CONSTANT_VALUE;
extern std::string E_CONSTANT_VALUE;
extern std::string USERNAME_CONSTANT_VALUE;

extern size_t NUMBER_OR_COLUMNS_MAX;
extern size_t NUMBER_OR_ROWS_MAX;
extern size_t SCREEN_WIDTH;

void loadConfig(nlohmann::json &config);
void initializeConstValues();

#endif // __CONST_H__