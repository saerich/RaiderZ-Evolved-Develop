#pragma once


namespace rs3 {


class RS_API RRenderingCallbackInterface
{
public:
	RRenderingCallbackInterface(void);
	virtual ~RRenderingCallbackInterface(void);

	virtual void DeviceSetting()	{} ///< �� �н��� ������ ��ġ�� ����̽� ������ ���ش�. G �������� �������� ȣ���.
	virtual void PreOrderRender()	{}
	virtual void PostOrderRender()	{}
};


}
