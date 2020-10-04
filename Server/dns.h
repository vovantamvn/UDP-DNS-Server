#pragma once
#include <stdio.h>
#include <string.h>
#include "ipaddressinfo.h"

const char* get_result(char*);

const char* get_result_if_domain(char*);

const char* get_result_if_ipaddr(char*);

bool check_ip_valid(char*);