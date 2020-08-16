using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace CharacterTool
{
	public class OutputForm : WeifenLuo.WinFormsUI.Content
	{
		private System.Windows.Forms.ComboBox comboBox;
		public System.Windows.Forms.RichTextBox tbOutput;
		private System.ComponentModel.IContainer components = null;

		public OutputForm()
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
			this.comboBox = new System.Windows.Forms.ComboBox();
			this.tbOutput = new System.Windows.Forms.RichTextBox();
			this.SuspendLayout();
			// 
			// comboBox
			// 
			this.comboBox.DisplayMember = "0";
			this.comboBox.Dock = System.Windows.Forms.DockStyle.Top;
			this.comboBox.Items.AddRange(new object[] {
														  "���",
														  "���",
														  "�����"});
			this.comboBox.Location = new System.Drawing.Point(0, 3);
			this.comboBox.Name = "comboBox";
			this.comboBox.Size = new System.Drawing.Size(472, 20);
			this.comboBox.TabIndex = 0;
			// 
			// tbOutput
			// 
			this.tbOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.tbOutput.Location = new System.Drawing.Point(0, 28);
			this.tbOutput.Name = "tbOutput";
			this.tbOutput.Size = new System.Drawing.Size(472, 164);
			this.tbOutput.TabIndex = 1;
			this.tbOutput.Text = "";
			// 
			// OutputForm
			// 
			this.AllowedStates = ((WeifenLuo.WinFormsUI.ContentStates)(((((WeifenLuo.WinFormsUI.ContentStates.Float | WeifenLuo.WinFormsUI.ContentStates.DockLeft) 
				| WeifenLuo.WinFormsUI.ContentStates.DockRight) 
				| WeifenLuo.WinFormsUI.ContentStates.DockTop) 
				| WeifenLuo.WinFormsUI.ContentStates.DockBottom)));
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(472, 197);
			this.Controls.Add(this.tbOutput);
			this.Controls.Add(this.comboBox);
			this.DockPadding.Bottom = 3;
			this.DockPadding.Top = 3;
			this.HideOnClose = true;
			this.Name = "OutputForm";
			this.ShowHint = WeifenLuo.WinFormsUI.DockState.DockBottom;
			this.Text = "���";
			this.ResumeLayout(false);

		}
		#endregion


		
	}
}

