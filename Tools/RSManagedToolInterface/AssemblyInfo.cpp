#include "stdafx.h"

using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;

//
// ������� ���� �Ϲ� ������ ���� Ư�� ������ ���� ����˴ϴ�. 
// ������� ���õ� ������ �����Ϸ���
// �� Ư�� ���� �����Ͻʽÿ�.
//
[assembly:AssemblyTitleAttribute("")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("")];
[assembly:AssemblyCopyrightAttribute("")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];		

//
// ������� ���� ������ ���� �� ���� ������ �����˴ϴ�.
//
//      �� ����
//      �� ���� 
//      ���� ��ȣ
//      ���� ��ȣ
//
// ��� ���� �����ϰų�
// �Ʒ��� ���� '*'�� ����Ͽ� ���� ��ȣ �� ���� ��ȣ�� �ڵ����� �����ǵ��� �� �� �ֽ��ϴ�.

[assembly:AssemblyVersionAttribute("1.0.*")];

//
// ������� �����Ϸ��� ����� Ű�� �����ؾ� �մϴ�. 
// ����� ���� ���� �ڼ��� ������ Microsoft .NET Framework ������ �����Ͻʽÿ�.
//
// ���� Ư���� ����ϸ� � Ű�� ����Ͽ� �������� ������ �� �ֽ��ϴ�. 
//
// ����: 
//   (*) Ű�� �������� ������ ������� ������ �� �����ϴ�.
//   (*) KeyName��
//       ����� ��ǻ�Ϳ��� CSP(��ȣȭ ���� ������)�� ��ġ�Ǿ� �ִ� Ű�� �����ϰ�
//       KeyFile�� Ű�� ���Ե� ������ �����մϴ�.
//   (*) KeyFile�� KeyName ���� ��� �����ϸ�
//       ������ ���� ���μ����� �����˴ϴ�.
//       (1) CSP �ȿ� KeyName�� ������ �ش� Ű�� ���˴ϴ�.
//       (2) KeyName�� ����, KeyFile�� ������ 
//           KeyFile �ȿ� �ִ� Ű�� CSP�� ��ġ�� �� ���˴ϴ�.
//   (*) sn.exe(������ �̸� ��ƿ��Ƽ)�� ����ϸ� KeyFile�� ���� �� �ֽ��ϴ�.
//        KeyFile�� �����ϴ� ���
//        KeyFile�� ��ġ�� ������Ʈ ���͸��� �������� �ϴ� ��� ��ġ�̾�� �մϴ�.
//   (*) ���� ����� ��� �ɼ��Դϴ�.
//       �� �ɼǿ� ���� �ڼ��� ������ Microsoft .NET Framework ������ �����Ͻʽÿ�.
//
[assembly:AssemblyDelaySignAttribute(false)];
[assembly:AssemblyKeyFileAttribute("")];
[assembly:AssemblyKeyNameAttribute("")];

