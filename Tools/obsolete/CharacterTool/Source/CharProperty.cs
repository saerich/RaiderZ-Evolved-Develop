using System;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;


namespace CharacterTool
{
	/// <summary>
	/// CharProperty�� ���� ��� �����Դϴ�.
	/// </summary>
	public class CharProperty
	{
		public CharProperty() { }

		private string		_�̸�;
		private int			_Age;
		private string		_Ani;
		private Color		_����;

		[Category("�⺻ ����"), Description("�̸��� ��������.")]

		public string �̸�
		{
			get { return _�̸�; }
			set { _�̸� = value; }
		}

		[Category("�⺻ ����"), Description("���̸� ��������.")]

		public int ����
		{
			get { return _Age; }
			set { _Age = value; }
		}

		[Category("�ִϸ��̼�"), Description("���ϸ��̼� ���.")]
		public string �ִϸ��̼�
		{
			get { return _Ani; }
			set { _Ani = value; }
		}

		[Category("�ִϸ��̼�"), Description("���� �Է��ϼ���.")]
		public Color ����
		{
			get { return _����; }
			set { _���� = value; }
		}

	}
}
