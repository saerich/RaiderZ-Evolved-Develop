#pragma once

#using <System.dll>
#using <System.Xml.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Xmlseral.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::Xml::Schema;
using namespace System::ComponentModel;
using namespace System::Globalization;
using namespace System::Drawing;
using namespace System::Drawing::Design;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::Design;

namespace XmlSerializable
{
	//------------------------------------------------------------------------
	public ref class SEGMENT
	{
	public: SEGMENT(){}

	};

	//------------------------------------------------------------------------
	//public ref class BoolTypeConverter: public DropDownStringConverter 
	//{  

	//public:
	//	virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
	//	{
	//		ArrayList^ aBoolType = gcnew ArrayList;
	//		aBoolType->Add("True");
	//		aBoolType->Add("False");

	//		return gcnew StandardValuesCollection(aBoolType);
	//	} 
	//};

	//------------------------------------------------------------------------
	public ref class TALENTCAPSULE: public CAPSULE 
	{
	public: TALENTCAPSULE(){}
	private: 
		float			_delay;
		bool			_bDoHitCheck;
		bool			_bDoHitFirst;
	public:
		[DescriptionAttribute("����������"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("Delay"),
			EditorAttribute(MyValueEditor3::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ����������
		{
			float		get() {return _delay;}
			void		set(float value) {_delay= value;}
		}

		[DescriptionAttribute("������ĸ���� �Ÿ���꿡 ���� ���� �Ǵ�"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("DoHitCheck")]
			//TypeConverter(BoolTypeConverter::typeid)]
		property bool DoHitCheck
		{
			bool		get() {return _bDoHitCheck;}
			void		set(bool value) {_bDoHitCheck = value;}
		}

		[DescriptionAttribute("�켱 ����"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("DoHitFirst")]
		//TypeConverter(BoolTypeConverter::typeid)]
		property bool DoHitFirst
		{
			bool		get() {return _bDoHitFirst;}
			void		set(bool value) {_bDoHitFirst = value;}
		}
	};
}
