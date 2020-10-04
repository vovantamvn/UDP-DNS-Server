#include "dns.h"

struct ipaddressinfo IPADDR_INFO[5] = {
	{"10.125.132.155", "10.154.135.154", "google.com", "google.com"},
	{"10.125.152.115", "10.154.135.194", "facebook.com", "fb.com"},
	{"10.125.132.226", "10.154.135.174", "wikidia.com", "wiki.com"},
	{"10.125.136.54", "10.154.135.48", "thegioididong.com", "ttdd.com"},
	{"10.125.136.15", "10.154.135.15", "dienmayxanh.com", "dmax.com"},
};

const char* get_result(char* s) {
	bool is_ipaddr = check_ip_valid(s);

	if (is_ipaddr) {
		return get_result_if_ipaddr(s);
	}

	return get_result_if_domain(s);
}

const char* get_result_if_domain(char* s) {
	for (int i = 0; i < 5; i++) {
		ipaddressinfo IP = IPADDR_INFO[i];

		if (strcmp(s, IP.offical_name)) {
			char result[256];
			strcpy_s(result, "Offical IP: ");
			strcat_s(result, IP.offical_ip);
			strcat_s(result, "\nAlias IP: ");
			strcat_s(result, IP.alias_ip);

			return result;
		}
	}

	return "Not found information";
}

const char* get_result_if_ipaddr(char* s) {
	for (int i = 0; i < 5; i++) {
		ipaddressinfo IP = IPADDR_INFO[i];

		bool is_equal = true;
		int length = strlen(s);

		if (length != strlen(IP.offical_ip)) {
			continue;
		}

		for (int j = 0; j < length; j++) {
			if (s[j] != IP.offical_ip[j]) {
				is_equal = false;
				break;
			}
		}

		if (is_equal) {
			char result[256];
			strcpy_s(result, "Offical name: ");
			strcat_s(result, IP.offical_name);
			strcat_s(result, "\nAlias name: ");
			strcat_s(result, IP.offical_name);

			return result;
		}
	}

	return "Not found information";
}

bool check_ip_valid(char* s) {
	if (s == NULL) {
		return false;
	}

	int length = strlen(s);
	int number_of_dot = 0;

	for (int i = 0; i < length; i++) {
		if (s[i] == '.') {
			number_of_dot++;
			continue;
		}

		if (s[i] < '0' || s[i] > '9') {
			return false;
		}
	}

	return number_of_dot == 3;
}