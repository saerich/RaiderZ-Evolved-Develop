using System;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Diagnostics;
using RSManaged;

namespace CharacterTool
{
	/// <summary>
	/// CToolHelper�� ���� ��� �����Դϴ�.
	/// </summary>
	public class CToolHelper
	{
		static public bool OpenFileDlg(System.Windows.Forms.Form kForm)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = "Mesh Files(*.elu)|*.elu" +
						 "|RS3 Model Files (*.xml)|*.xml";

			if (dlg.ShowDialog(kForm) == DialogResult.OK)
			{
				string strFileName = dlg.FileName;

				if (File.Exists(strFileName))
				{
					string strExt = System.IO.Path.GetExtension(strFileName).ToLower();
					if (strExt == ".elu")
					{
						OpenMeshFile(strFileName);
					}
					else if ((strExt == ".xml") && (dlg.FilterIndex == 2))
					{
						CToolHelper.OpenModelFile(strFileName);
					}
				}
			}

			return true;
		}

		static public bool OpenModelFile(string strFileName)
		{
			if (strFileName == "") return false;

			OutputText.Print("�� �ε�");

			return true;
		}

		static public bool OpenMeshFile(string strFileName)
		{
			Global.bEmptyMesh = true;

			Global.actorMgr.DelAll();
			Global.meshMgr.DelAll();

			MMesh mesh = Global.meshMgr.Add(strFileName, "");
			if (!mesh.Valid) 
			{
				OutputText.Print("�޽��� ���� ���߽��ϴ�.");
				return false;
			}

			int vid = Global.actorMgr.Add(mesh);
			if (vid == -1)
			{
				OutputText.Print("�޽��� ���� ���߽��ϴ�.");
				return false;
			}
////
////			MActor actor = Global.actorMgr.GetFast(vid);
////
//			OutputText.Print("�޽��� �о����ϴ�.");

			Global.bEmptyMesh = false;

			return true;
		}
	}
}
