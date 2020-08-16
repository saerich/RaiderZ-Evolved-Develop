namespace SoulHunt
{
    partial class CompareFileForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.lblFileName = new System.Windows.Forms.Label();
            this.btnCopyServer = new System.Windows.Forms.Button();
            this.btnCopyClient = new System.Windows.Forms.Button();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnRestart = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Location = new System.Drawing.Point(12, 38);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(540, 96);
            this.textBox1.TabIndex = 0;
            this.textBox1.TabStop = false;
            this.textBox1.Text = "위의 파일이 일치하지 않습니다!!\r\n\r\n이 파일은 서버와 클라이언트에 각각 저장되며 안전을 위하여 반드시 서로 일치하기를 권장합니다.\r\n\r\n다음의" +
                " 동작에서 하나를 선택해주십시오. 현재 SVN 업데이트 중이라면 \"바로 재시작\"을 권장합니다.\r\n만일 검토후 다시 프로그램을 시작하고 싶다면 \"" +
                "바로 종료\"를 선택해주시기 바랍니다.";
            // 
            // lblFileName
            // 
            this.lblFileName.AutoSize = true;
            this.lblFileName.Font = new System.Drawing.Font("굴림", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblFileName.Location = new System.Drawing.Point(12, 9);
            this.lblFileName.Name = "lblFileName";
            this.lblFileName.Size = new System.Drawing.Size(66, 14);
            this.lblFileName.TabIndex = 0;
            this.lblFileName.Text = "npc.xml";
            // 
            // btnCopyServer
            // 
            this.btnCopyServer.DialogResult = System.Windows.Forms.DialogResult.Yes;
            this.btnCopyServer.Location = new System.Drawing.Point(12, 140);
            this.btnCopyServer.Name = "btnCopyServer";
            this.btnCopyServer.Size = new System.Drawing.Size(137, 32);
            this.btnCopyServer.TabIndex = 0;
            this.btnCopyServer.Text = "서버 파일을 복사(&S)";
            this.btnCopyServer.UseVisualStyleBackColor = true;
            // 
            // btnCopyClient
            // 
            this.btnCopyClient.DialogResult = System.Windows.Forms.DialogResult.No;
            this.btnCopyClient.Location = new System.Drawing.Point(155, 140);
            this.btnCopyClient.Name = "btnCopyClient";
            this.btnCopyClient.Size = new System.Drawing.Size(172, 32);
            this.btnCopyClient.TabIndex = 1;
            this.btnCopyClient.Text = "클라이언트 파일을 복사(&C)";
            this.btnCopyClient.UseVisualStyleBackColor = true;
            // 
            // btnExit
            // 
            this.btnExit.DialogResult = System.Windows.Forms.DialogResult.Abort;
            this.btnExit.Location = new System.Drawing.Point(464, 140);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(99, 32);
            this.btnExit.TabIndex = 2;
            this.btnExit.Text = "바로 종료(&X)";
            this.btnExit.UseVisualStyleBackColor = true;
            // 
            // btnRestart
            // 
            this.btnRestart.DialogResult = System.Windows.Forms.DialogResult.Retry;
            this.btnRestart.Location = new System.Drawing.Point(344, 140);
            this.btnRestart.Name = "btnRestart";
            this.btnRestart.Size = new System.Drawing.Size(114, 32);
            this.btnRestart.TabIndex = 3;
            this.btnRestart.Text = "바로 재시작(&R)";
            this.btnRestart.UseVisualStyleBackColor = true;
            // 
            // CompareFileForm
            // 
            this.AcceptButton = this.btnCopyServer;
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnExit;
            this.ClientSize = new System.Drawing.Size(575, 186);
            this.ControlBox = false;
            this.Controls.Add(this.btnRestart);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.btnCopyClient);
            this.Controls.Add(this.btnCopyServer);
            this.Controls.Add(this.lblFileName);
            this.Controls.Add(this.textBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CompareFileForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "파일비교";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnExit;
        public System.Windows.Forms.Label lblFileName;
        public System.Windows.Forms.Button btnCopyServer;
        public System.Windows.Forms.Button btnCopyClient;
        private System.Windows.Forms.Button btnRestart;

    }
}