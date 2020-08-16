#pragma once

/// ���� �������� UI�� �ε��ϴ� ������ �Ѵ�.
class XGameFrameUILoader
{
private:
	map<wstring, wstring>			m_FrameDescriptorList;
	void AddFrameList(wstring strFrameName, wstring strFileName);
	bool LoadFrameDescriptor();
public:
	XGameFrameUILoader(){}
	~XGameFrameUILoader(){}
	bool Load(const wchar_t* szGameFrameName=NULL, bool bReload=false);
};