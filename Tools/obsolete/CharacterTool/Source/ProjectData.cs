using System;

namespace CharacterTool
{
	/// <summary>
	/// ProjectData�� ���� ��� �����Դϴ�.
	/// </summary>
	public class ProjectData
	{
		#region Instance
		private static ProjectData ms_pkThis = null;
		public static ProjectData Instance
		{
			get	{ return ms_pkThis;	}
		}

		public static void Init()
		{
			ms_pkThis = new ProjectData();
		}

		public static void Final()
		{
			ms_pkThis = null;
		}
		#endregion

		public ProjectData()
		{
		}


		#region ����Ÿ
		

		#endregion
	}
}
