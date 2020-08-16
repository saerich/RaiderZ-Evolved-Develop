﻿namespace HeatMap
{
    partial class MainForm
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
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.tbXmlPath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbFieldID = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tbFileImage = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnOpenFieldImage = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.udMinX = new System.Windows.Forms.NumericUpDown();
            this.udMinY = new System.Windows.Forms.NumericUpDown();
            this.udMaxX = new System.Windows.Forms.NumericUpDown();
            this.udMaxY = new System.Windows.Forms.NumericUpDown();
            this.btnSaveImage = new System.Windows.Forms.Button();
            this.btnOpenSrcXml = new System.Windows.Forms.Button();
            this.udRadius = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udRadius)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(521, 121);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(451, 451);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // btnGenerate
            // 
            this.btnGenerate.Location = new System.Drawing.Point(630, 48);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(110, 23);
            this.btnGenerate.TabIndex = 1;
            this.btnGenerate.Text = "HeatMap 만들기";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // tbXmlPath
            // 
            this.tbXmlPath.Location = new System.Drawing.Point(100, 12);
            this.tbXmlPath.Name = "tbXmlPath";
            this.tbXmlPath.ReadOnly = true;
            this.tbXmlPath.Size = new System.Drawing.Size(324, 21);
            this.tbXmlPath.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "원본 xml";
            // 
            // tbFieldID
            // 
            this.tbFieldID.Location = new System.Drawing.Point(100, 85);
            this.tbFieldID.Name = "tbFieldID";
            this.tbFieldID.Size = new System.Drawing.Size(324, 21);
            this.tbFieldID.TabIndex = 7;
            this.tbFieldID.Text = "0";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(19, 88);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "필드 ID";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 124);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "min_x";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(19, 163);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(39, 12);
            this.label5.TabIndex = 10;
            this.label5.Text = "min_y";
            // 
            // tbFileImage
            // 
            this.tbFileImage.Location = new System.Drawing.Point(100, 48);
            this.tbFileImage.Name = "tbFileImage";
            this.tbFileImage.ReadOnly = true;
            this.tbFileImage.Size = new System.Drawing.Size(324, 21);
            this.tbFileImage.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "필드 이미지";
            // 
            // btnOpenFieldImage
            // 
            this.btnOpenFieldImage.Location = new System.Drawing.Point(451, 46);
            this.btnOpenFieldImage.Name = "btnOpenFieldImage";
            this.btnOpenFieldImage.Size = new System.Drawing.Size(124, 23);
            this.btnOpenFieldImage.TabIndex = 14;
            this.btnOpenFieldImage.Text = "필드 이미지 열기";
            this.btnOpenFieldImage.UseVisualStyleBackColor = true;
            this.btnOpenFieldImage.Click += new System.EventHandler(this.btnOpenFieldImage_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(19, 240);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(43, 12);
            this.label6.TabIndex = 17;
            this.label6.Text = "max_y";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(19, 201);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(43, 12);
            this.label7.TabIndex = 15;
            this.label7.Text = "max_x";
            // 
            // udMinX
            // 
            this.udMinX.Location = new System.Drawing.Point(100, 121);
            this.udMinX.Maximum = new decimal(new int[] {
            2000000,
            0,
            0,
            0});
            this.udMinX.Minimum = new decimal(new int[] {
            2000000,
            0,
            0,
            -2147483648});
            this.udMinX.Name = "udMinX";
            this.udMinX.Size = new System.Drawing.Size(120, 21);
            this.udMinX.TabIndex = 18;
            // 
            // udMinY
            // 
            this.udMinY.Location = new System.Drawing.Point(100, 161);
            this.udMinY.Maximum = new decimal(new int[] {
            2000000,
            0,
            0,
            0});
            this.udMinY.Minimum = new decimal(new int[] {
            2000000,
            0,
            0,
            -2147483648});
            this.udMinY.Name = "udMinY";
            this.udMinY.Size = new System.Drawing.Size(120, 21);
            this.udMinY.TabIndex = 19;
            // 
            // udMaxX
            // 
            this.udMaxX.Location = new System.Drawing.Point(100, 201);
            this.udMaxX.Maximum = new decimal(new int[] {
            2000000,
            0,
            0,
            0});
            this.udMaxX.Minimum = new decimal(new int[] {
            2000000,
            0,
            0,
            -2147483648});
            this.udMaxX.Name = "udMaxX";
            this.udMaxX.Size = new System.Drawing.Size(120, 21);
            this.udMaxX.TabIndex = 20;
            this.udMaxX.Value = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.udMaxX.ValueChanged += new System.EventHandler(this.udMaxX_ValueChanged);
            // 
            // udMaxY
            // 
            this.udMaxY.Location = new System.Drawing.Point(100, 240);
            this.udMaxY.Maximum = new decimal(new int[] {
            2000000,
            0,
            0,
            0});
            this.udMaxY.Minimum = new decimal(new int[] {
            2000000,
            0,
            0,
            -2147483648});
            this.udMaxY.Name = "udMaxY";
            this.udMaxY.Size = new System.Drawing.Size(120, 21);
            this.udMaxY.TabIndex = 21;
            this.udMaxY.Value = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            // 
            // btnSaveImage
            // 
            this.btnSaveImage.Location = new System.Drawing.Point(746, 48);
            this.btnSaveImage.Name = "btnSaveImage";
            this.btnSaveImage.Size = new System.Drawing.Size(112, 23);
            this.btnSaveImage.TabIndex = 22;
            this.btnSaveImage.Text = "저장";
            this.btnSaveImage.UseVisualStyleBackColor = true;
            this.btnSaveImage.Click += new System.EventHandler(this.btnSaveImage_Click);
            // 
            // btnOpenSrcXml
            // 
            this.btnOpenSrcXml.Location = new System.Drawing.Point(451, 13);
            this.btnOpenSrcXml.Name = "btnOpenSrcXml";
            this.btnOpenSrcXml.Size = new System.Drawing.Size(124, 23);
            this.btnOpenSrcXml.TabIndex = 23;
            this.btnOpenSrcXml.Text = "원본 XML 열기..";
            this.btnOpenSrcXml.UseVisualStyleBackColor = true;
            this.btnOpenSrcXml.Click += new System.EventHandler(this.btnOpenSrcXml_Click);
            // 
            // udRadius
            // 
            this.udRadius.Location = new System.Drawing.Point(100, 283);
            this.udRadius.Maximum = new decimal(new int[] {
            2000000,
            0,
            0,
            0});
            this.udRadius.Minimum = new decimal(new int[] {
            2000000,
            0,
            0,
            -2147483648});
            this.udRadius.Name = "udRadius";
            this.udRadius.Size = new System.Drawing.Size(120, 21);
            this.udRadius.TabIndex = 24;
            this.udRadius.Value = new decimal(new int[] {
            25,
            0,
            0,
            0});
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(20, 285);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(40, 12);
            this.label8.TabIndex = 25;
            this.label8.Text = "radius";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1072, 621);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.udRadius);
            this.Controls.Add(this.btnOpenSrcXml);
            this.Controls.Add(this.btnSaveImage);
            this.Controls.Add(this.udMaxY);
            this.Controls.Add(this.udMaxX);
            this.Controls.Add(this.udMinY);
            this.Controls.Add(this.udMinX);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.btnOpenFieldImage);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tbFieldID);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbFileImage);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbXmlPath);
            this.Controls.Add(this.btnGenerate);
            this.Controls.Add(this.pictureBox1);
            this.Name = "MainForm";
            this.Text = "Raiderz HeatMap";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udRadius)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.TextBox tbXmlPath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbFieldID;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbFileImage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnOpenFieldImage;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown udMinX;
        private System.Windows.Forms.NumericUpDown udMinY;
        private System.Windows.Forms.NumericUpDown udMaxX;
        private System.Windows.Forms.NumericUpDown udMaxY;
        private System.Windows.Forms.Button btnSaveImage;
        private System.Windows.Forms.Button btnOpenSrcXml;
        private System.Windows.Forms.NumericUpDown udRadius;
        private System.Windows.Forms.Label label8;
    }
}

