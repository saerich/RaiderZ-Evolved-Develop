#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>
#using <System.Data.dll>


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Reflection;
using CustomPropGrid::CustomPropertyGrid;
using CustomPropGrid::Settings;
using CustomPropGrid::Setting;

#include <map>
#include <string>
#include "..\Reflection\BaseObject.h"


namespace ReflectivePropertyFormNameSpace
{
	// c++ Reflection�� �ٷ� CustomPropGrid�� �����ų �� ����
	// CustomPropGrid�� ���� ��Ű���� C#�� ������Ƽ�� ������Ѿ� �Ѵ�.
	// �׷��� �Ʒ� Ŭ������ c++ Reflection�� C#�� ������Ƽ�� �����ִ� Ŭ�����̴�
	template<typename T>
	public ref class ReflectionProperty  : public System::Object 
	{
	public:
		CTypedProperty<T>*		m_pTypedProperty;
		CBaseObject*			m_pBaseObject;
	public:
		property	T	Property
		{
			T	get()
			{
				return m_pTypedProperty->GetValue(m_pBaseObject);;
			}
			void set(T value)
			{
				m_pTypedProperty->SetValue(m_pBaseObject,value);
			}
		}
	};

	//char* ���� 'CustomPropGrid'���� ���� ����
	//�׷��� char*���� System::String���� ��ȯ�ϱ� ����, �߰� �ܰ踦 �� ��ġ�� ����,
	//char*���� ���ø�Ư��ȭ�� ������ �ٸ�������
	template<>
	public ref class ReflectionProperty<char*> : public System::Object 
	{
	public:
		CTypedProperty<char*>*		m_pTypedProperty;
		CBaseObject*				m_pBaseObject;

	public:
		property	System::String^	Property
		{
			System::String^	get()
			{
				char* szChar = m_pTypedProperty->GetValue(m_pBaseObject);
				System::String^ str = gcnew System::String(szChar);
				return str;
			}
			void set(System::String^ str)
			{
				char *szStr = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
				m_pTypedProperty->SetValue(m_pBaseObject,szStr);
			}
		}
	};

	
	template<>
	public ref class ReflectionProperty<DWORD> : public System::Object 
	{
	public:
		CTypedProperty<DWORD>*		m_pTypedProperty;
		CBaseObject*				m_pBaseObject;

	public:
		property	System::Drawing::Color^	Property
		{
			System::Drawing::Color^	get()
			{
				DWORD dwColor = m_pTypedProperty->GetValue(m_pBaseObject);
				System::Drawing::Color color;
				color.FromArgb(dwColor);

				return color;
			}

			void set(System::Drawing::Color^ color)
			{
				DWORD dwColor = System::Drawing::ColorTranslator::ToWin32(*color);
				m_pTypedProperty->SetValue(m_pBaseObject,dwColor);
			}
		}
	};

	public ref class ReflectivePropertyGrid : public CustomPropGrid::CustomPropertyGrid
	{
	protected:
		Hashtable^					m_accessors;
		CustomPropGrid::Settings^	m_settings;
		EventHandler^				m_eventHandler;
		CBaseObject*				m_pBaseObject;

	public:
		ReflectivePropertyGrid();
		~ReflectivePropertyGrid();

	public:
		void InitializeComponent();
		void CompleteToInitializeComponent();

		void			setBaseObject(void*	pBaseObject)			{	m_pBaseObject = static_cast<CBaseObject*>(pBaseObject);	}
		void			setBaseObject(CBaseObject* pBaseObject)		{	m_pBaseObject = pBaseObject;	}
		CBaseObject*	getBaseObject(void)							{	return m_pBaseObject;			}


		System::Void updateReflect(System::Object^ sender, System::EventArgs^  evt);

		void addAllReflection();
		void addAllReflection(CBaseObject* pBaseObject);

		ref struct Accesser : Object
		{
		public:
			Accesser(CBaseObject* pBaseObject)
			{
				m_pBaseObject = pBaseObject;
			}
			CBaseObject* m_pBaseObject;
		};

		template<typename T>
		void addReflection(CTypedProperty<T>* pTypedProperty,CBaseObject* pBaseObject)
		{
			ReflectionProperty<T>^ reflectiveProperty = gcnew ReflectionProperty<T>;
			reflectiveProperty->m_pBaseObject	= pBaseObject;
			reflectiveProperty->m_pTypedProperty = pTypedProperty;

			System::String^ name = gcnew System::String(pTypedProperty->GetName());

			m_accessors[name]	=	gcnew Accesser(pBaseObject);
			m_settings[name]	=	gcnew Setting(reflectiveProperty->Property,gcnew System::String(pTypedProperty->GetDescription()),
				gcnew System::String(pTypedProperty->GetCategory()),m_eventHandler,(Setting::PROPERTY_EDITOR)pTypedProperty->GetPropertyEditor());
		}
	};

	public ref class ReflectivePropertyForm : public System::Windows::Forms::Form
	{
	protected:
		char*					m_szText;
		ReflectivePropertyGrid^	propertyGrid1;

	public:
		ReflectivePropertyForm();
		~ReflectivePropertyForm();

	public:
		void InitializeComponent();
		void CompleteToInitializeComponent();

		void	setText(const char* szText);
		char*	getText(void)	{	return m_szText;	}

		void			setBaseObject(void*	pBaseObject)			{	propertyGrid1->setBaseObject( pBaseObject );	}
		void			setBaseObject(CBaseObject* pBaseObject)		{	propertyGrid1->setBaseObject( pBaseObject );	}
		CBaseObject*	getBaseObject(void)							{	return propertyGrid1->getBaseObject();			}
	
		void addAllReflection();
	};
}