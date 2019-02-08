#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <sddl.h>

DWORD id = 0x639bc8;

bool err = false;

void Fix(const char* keyPath, bool extended) {
	HKEY key;

	long res = RegOpenKeyEx(HKEY_CLASSES_ROOT, keyPath, 0, KEY_ALL_ACCESS, &key);

	if (res != ERROR_SUCCESS) {
		printf("Failed to open key! Admin privileges required");
		err = true;
		return;
	}

	RegDeleteKeyValue(key, "cmd", "HideBasedOnVelocityId");
	RegSetKeyValue(key, "cmd", "ShowBasedOnVelocityId", REG_DWORD, &id, sizeof(DWORD));

	if (extended) RegSetKeyValue(key, "cmd", "Extended", REG_SZ, 0, 0);
	else RegDeleteKeyValue(key, "cmd", "Extended");

	RegDeleteKeyValue(key, "Powershell", "ShowBasedOnVelocityId");
	RegSetKeyValue(key, "Powershell", "HideBasedOnVelocityId", REG_DWORD, &id, sizeof(DWORD));

	RegCloseKey(key);
}

int main(int argc, char** argv) {

	bool extend = true;

	if (argc == 2) {
		extend = memcmp(argv[1], "--no-extend", 11) != 0;
	}

	Fix("\\Directory\\Background\\shell", extend);
	Fix("\\Directory\\shell", extend);
	Fix("\\Drive\\shell", extend);

	if (!err) printf("Success!\n");
	else printf("Failed!\n");

	return 0;
}