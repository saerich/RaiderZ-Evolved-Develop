#pragma once

namespace rs3 {

/// �ý��� ����
class RS_API RSystemInfo
{
	bool GetFileList(const char* path, const char* filter, vector<string>& outvecFiles);

public:
	RSystemInfo();
	~RSystemInfo() {}

	void			CheckAndDeleteCache();
};









} // namespace rs3