using System;

namespace CharacterTool
{
	/// <summary>
	/// InputManager�� ���� ��� �����Դϴ�.
	/// </summary>
	public class InputManager
	{
		#region ��� ����
		private static InputManager m_pkThis = null;
		#endregion

		public InputManager()
		{

		
		}

		public static InputManager Instance
		{
			get
			{
				if (m_pkThis == null)
				{
					m_pkThis = new InputManager();
				}
				return m_pkThis;
			}
		}

	}
}
