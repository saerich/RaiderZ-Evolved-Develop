using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Diagnostics;
using RSManaged;

namespace CharacterTool
{
	/// <summary>
	/// RenderPanel
	/// </summary>
	public class RenderPanel : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private static uint NumInstances = 0;

		public RenderPanel()
		{
			// �� ȣ���� Windows.Forms Form �����̳ʿ� �ʿ��մϴ�.
			InitializeComponent();

			Debug.Assert(NumInstances == 0);
			NumInstances++;

			// TODO: InitializeComponent�� ȣ���� ���� �ʱ�ȭ �۾��� �߰��մϴ�.

		}

		/// <summary> 
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				NumInstances--;
				Debug.Assert(NumInstances == 0, "There should only ever be one un-disposed" + 
					" version of this class in memory at a time.");

				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region ���� ��� �����̳ʿ��� ������ �ڵ�
		/// <summary> 
		/// �����̳� ������ �ʿ��� �޼����Դϴ�. 
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// RenderPanel
			// 
			this.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.Name = "RenderPanel";
			//this.Location = new System.Drawing.Point(0, 0);
			this.Size = new System.Drawing.Size(400, 400);
			this.Load += new System.EventHandler(this.RenderPanel_Load);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.RenderPanel_MouseMove);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RenderPanel_MouseDown);
			this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.RenderPanel_MouseWheel);

		}
		#endregion


		protected override void OnResize(System.EventArgs e)
		{
			base.OnResize(e);

			Invalidate();
		}

		protected override void OnPaint(PaintEventArgs e)
		{   
			if (this.DesignMode) return;

			if (!MFramework.InstanceIsValid())
			{
				base.OnPaint(e);
			}
			else
			{
				Global.renderer.Render();
			}
//			else if (MFramework.Instance.Renderer.ActiveCamera == null ||
//				!MFramework.Instance.Renderer.ActiveCamera.Active)
//			{
//				base.OnPaint(e);
//			}
//			else if (!MFramework.Instance.Renderer.Active)
//			{
//				base.OnPaint(e);
//			}
//			else
//			{
//				MFramework.Instance.Update();
//			}
		}

		protected override void OnPaintBackground(PaintEventArgs pevent)
		{
			if (!MFramework.InstanceIsValid())
			{
				base.OnPaintBackground(pevent);
			}
//			else if (MFramework.Instance.Renderer.ActiveCamera == null ||
//				!MFramework.Instance.Renderer.ActiveCamera.Active)
//			{
//				base.OnPaintBackground(pevent);
//			}
//			else if (!MFramework.Instance.Renderer.Active)
//			{
//				base.OnPaintBackground(pevent);
//			}
		}

		public void InitRenderer()
		{
			if (!MFramework.InstanceIsValid()) return;

			IntPtr handle = Handle;

			//MFramework.Instance.Device.SetDevice(
			MFramework.Instance.Device.Init(handle,
				640, 480, false, RSManaged.RFORMAT.RFMT_ARGB32);
			Global.renderer.Init();			
		}

		private void RenderPanel_Load(object sender, System.EventArgs e)
		{
			this.Anchor = (AnchorStyles.Bottom | AnchorStyles.Top | 
						   AnchorStyles.Left | AnchorStyles.Right );

			this.Location = new System.Drawing.Point(0, 0);
			this.Size = this.Parent.Size;
		}

		private void RenderPanel_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
		
		}

		private int old_mouse_x = -1;
		private int old_mouse_y = -1;

		private void RenderPanel_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			int dx = e.X - old_mouse_x;
			int dy = e.Y - old_mouse_y;

			if (e.Button == MouseButtons.Left)
			{
				MCameraSceneNode camera = MFramework.Instance.SceneManager.GetActiveCamera();

				if (camera.Valid)
				{
					MPolarCameraSceneNode cam = (camera as MPolarCameraSceneNode);
					RSMPolarCoord coord = cam.Coord;

					coord.fTheta -= (dx / 100.0f);
					coord.fPhi += (dy / 200.0f);
					cam.Coord = coord;
				}
			}
			else if (e.Button == MouseButtons.Right)
			{
				MCameraSceneNode camera = MFramework.Instance.SceneManager.GetActiveCamera();

				if (camera.Valid)
				{
					MPolarCameraSceneNode cam = (camera as MPolarCameraSceneNode);
					Vector3 at = cam.LookAt;
					Vector3 up = new Vector3(0,0,1);

					Vector3 right = cam.Direction.CrossProduct(up);

					right.Normalize();
					right = right * (-dx);

					at += right;
					at.z += dy;
					
					cam.LookAt = at;
				}
			}

			old_mouse_x = e.X;
			old_mouse_y = e.Y;
		}

		private void RenderPanel_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			float d = e.Delta * 0.2f;

			MCameraSceneNode camera = MFramework.Instance.SceneManager.GetActiveCamera();

			if (camera.Valid)
			{
				MPolarCameraSceneNode cam = (camera as MPolarCameraSceneNode);
				RSMPolarCoord coord = cam.Coord;

				coord.fRadius += d;
				if (coord.fRadius < 0.01f) coord.fRadius = 0.01f;

				cam.Coord = coord;
			}
		}

	}
}
