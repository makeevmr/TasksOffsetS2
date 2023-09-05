#ifndef OFFSET_TASKS_S2_EXCEPTIONS_H
#define OFFSET_TASKS_S2_EXCEPTIONS_H

#include "request.h"

bool valid_command(const char *command);

bool valid_date_format(const char *date);

bool valid_data_day(const char *data, char *number);

bool valid_data_month(const char *data, char *number);

bool valid_data_year(const char *data, char *number);

bool valid_time_format(const char *time);

bool valid_time(const char *time);

bool valid_request_type(const char *type);

#endif //OFFSET_TASKS_S2_EXCEPTIONS_H
