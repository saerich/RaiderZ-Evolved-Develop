using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace CharacterTool
{
	/// <summary>
	/// OptionForm�� ���� ��� �����Դϴ�.
	/// </summary>
	public class OptionForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tpGeneral;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Button btnReset;
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public OptionForm()
		{
			//
			// Windows Form �����̳� ������ �ʿ��մϴ�.
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent�� ȣ���� ���� ������ �ڵ带 �߰��մϴ�.
			//
		}

		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form �����̳ʿ��� ������ �ڵ�
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		private void InitializeComponent()
		{
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tpGeneral = new System.Windows.Forms.TabPage();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.btnReset = new System.Windows.Forms.Button();
			this.tabControl1.SuspendLayout();
			this.tpGeneral.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tpGeneral);
			this.tabControl1.Location = new System.Drawing.Point(8, 9);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(504, 336);
			this.tabControl1.TabIndex = 0;
			// 
			// tpGeneral
			// 
			this.tpGeneral.Controls.Add(this.groupBox1);
			this.tpGeneral.Location = new System.Drawing.Point(4, 21);
			this.tpGeneral.Name = "tpGeneral";
			this.tpGeneral.Size = new System.Drawing.Size(496, 311);
			this.tpGeneral.TabIndex = 0;
			this.tpGeneral.Text = "�Ϲ�";
			// 
			// groupBox1
			// 
			this.groupBox1.Location = new System.Drawing.Point(16, 24);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(456, 80);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "����";
			// 
			// btnOK
			// 
			this.btnOK.Location = new System.Drawing.Point(437, 357);
			this.btnOK.Name = "btnOK";
			this.btnOK.TabIndex = 1;
			this.btnOK.Text = "Ȯ��";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.Location = new System.Drawing.Point(351, 357);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.TabIndex = 2;
			this.btnCancel.Text = "���";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// btnReset
			// 
			this.btnReset.Location = new System.Drawing.Point(265, 357);
			this.btnReset.Name = "btnReset";
			this.btnReset.TabIndex = 3;
			this.btnReset.Text = "�ʱ�ȭ";
			// 
			// OptionForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(520, 389);
			this.Controls.Add(this.btnReset);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.tabControl1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "OptionForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "�ɼ�";
			this.tabControl1.ResumeLayout(false);
			this.tpGeneral.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void btnOK_Click(object sender, System.EventArgs e)
		{
			this.DialogResult = DialogResult.OK;
			this.Close();
		}

		private void btnCancel_Click(object sender, System.EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}
	}
}
