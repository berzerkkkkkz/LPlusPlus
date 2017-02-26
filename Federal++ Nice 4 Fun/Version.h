#pragma once
#include "PluginSDK.h"
#include "Color.h"
#include <string>

inline bool VersionChecker()
{
	std::string szData;
	if (GPluginSDK->ReadFileFromURL("https://github.com/zFederaL/LPlusPlus/blob/master/Version/Nice4Fun.txt", szData))
	{
		auto n = szData.find("LocalVersion = ");

		if (n != szData.npos)
		{
			n += 15;
			szData = szData.substr(n, szData.size() - n);

		}

		auto k = szData.find(";");

		if (k != szData.npos)
			szData = szData.substr(0, k);

		auto dwGitVersion = atoi(szData.c_str());

		return (dwGitVersion <= 1);
	}

	return true;
}

inline void RunVersionChecker()
{
	if (VersionChecker() == true)
	{
		GRender->NotificationEx(Color::LightBlue().Get(), 2, true, true, "You have latest Version");
	}
	else
	{
		GRender->NotificationEx(Color::LightBlue().Get(), 2, true, true, "You have an Old Version Go Download Latest Plugin");
	}
}



