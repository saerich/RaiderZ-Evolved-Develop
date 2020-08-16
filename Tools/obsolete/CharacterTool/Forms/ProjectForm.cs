using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace CharacterTool
{
	public class ProjectForm : WeifenLuo.WinFormsUI.Content
	{
		private System.Windows.Forms.ToolBar toolBar1;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.TreeView tvProject;
		private System.ComponentModel.IContainer components = null;

		public ProjectForm()
		{
			// �� ȣ���� Windows Form �����̳ʿ� �ʿ��մϴ�.
			InitializeComponent();

			// TODO: InitializeComponent�� ȣ���� ���� �ʱ�ȭ �۾��� �߰��մϴ�.
		}

		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region �����̳ʿ��� ������ �ڵ�
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.toolBar1 = new System.Windows.Forms.ToolBar();
			this.tvProject = new System.Windows.Forms.TreeView();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.SuspendLayout();
			// 
			// toolBar1
			// 
			this.toolBar1.ButtonSize = new System.Drawing.Size(24, 24);
			this.toolBar1.DropDownArrows = true;
			this.toolBar1.Location = new System.Drawing.Point(0, 25);
			this.toolBar1.Name = "toolBar1";
			this.toolBar1.ShowToolTips = true;
			this.toolBar1.Size = new System.Drawing.Size(184, 30);
			this.toolBar1.TabIndex = 0;
			// 
			// tvProject
			// 
			this.tvProject.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvProject.ImageIndex = -1;
			this.tvProject.Location = new System.Drawing.Point(0, 55);
			this.tvProject.Name = "tvProject";
			this.tvProject.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
																				  new System.Windows.Forms.TreeNode("CharacterModel", new System.Windows.Forms.TreeNode[] {
																																											  new System.Windows.Forms.TreeNode("Mesh"),
																																											  new System.Windows.Forms.TreeNode("Animation", new System.Windows.Forms.TreeNode[] {
																																																																	 new System.Windows.Forms.TreeNode("idle"),
																																																																	 new System.Windows.Forms.TreeNode("run"),
																																																																	 new System.Windows.Forms.TreeNode("run_back"),
																																																																	 new System.Windows.Forms.TreeNode("run_left"),
																																																																	 new System.Windows.Forms.TreeNode("run_right"),
																																																																	 new System.Windows.Forms.TreeNode("jump")}),
																																											  new System.Windows.Forms.TreeNode("Motion")})});
			this.tvProject.SelectedImageIndex = -1;
			this.tvProject.Size = new System.Drawing.Size(184, 228);
			this.tvProject.TabIndex = 1;
			this.tvProject.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvProject_AfterSelect);
			// 
			// imageList1
			// 
			this.imageList1.ImageSize = new System.Drawing.Size(16, 16);
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// ProjectForm
			// 
			this.AllowedStates = ((WeifenLuo.WinFormsUI.ContentStates)(((((WeifenLuo.WinFormsUI.ContentStates.Float | WeifenLuo.WinFormsUI.ContentStates.DockLeft) 
				| WeifenLuo.WinFormsUI.ContentStates.DockRight) 
				| WeifenLuo.WinFormsUI.ContentStates.DockTop) 
				| WeifenLuo.WinFormsUI.ContentStates.DockBottom)));
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(184, 285);
			this.Controls.Add(this.tvProject);
			this.Controls.Add(this.toolBar1);
			this.DockPadding.Bottom = 2;
			this.DockPadding.Top = 25;
			this.Name = "ProjectForm";
			this.ShowHint = WeifenLuo.WinFormsUI.DockState.DockLeft;
			this.Text = "������Ʈ";
			this.ResumeLayout(false);

		}
		#endregion

		private void tvProject_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
		
		}
	}
}

