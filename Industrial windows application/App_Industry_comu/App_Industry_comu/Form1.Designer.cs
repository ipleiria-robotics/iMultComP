
namespace App_Industry_comu
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.Btn_modbus_win = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.but_board = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft YaHei UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(33, 254);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(518, 27);
            this.label1.TabIndex = 0;
            this.label1.Text = "Modbus Protocol Debug in Windows Environment";
            // 
            // Btn_modbus_win
            // 
            this.Btn_modbus_win.Location = new System.Drawing.Point(585, 252);
            this.Btn_modbus_win.Margin = new System.Windows.Forms.Padding(4);
            this.Btn_modbus_win.Name = "Btn_modbus_win";
            this.Btn_modbus_win.Size = new System.Drawing.Size(171, 36);
            this.Btn_modbus_win.TabIndex = 1;
            this.Btn_modbus_win.Text = "Modbus PC Windows";
            this.Btn_modbus_win.UseVisualStyleBackColor = true;
            this.Btn_modbus_win.Click += new System.EventHandler(this.Btn_modbus_win_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("MV Boli", 27.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(104, 32);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(828, 62);
            this.label2.TabIndex = 2;
            this.label2.Text = "Industrial communication protocols";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft YaHei UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(238, 157);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(313, 27);
            this.label3.TabIndex = 3;
            this.label3.Text = "Comunication Board Am335x ";
            // 
            // but_board
            // 
            this.but_board.Location = new System.Drawing.Point(585, 157);
            this.but_board.Margin = new System.Windows.Forms.Padding(4);
            this.but_board.Name = "but_board";
            this.but_board.Size = new System.Drawing.Size(171, 36);
            this.but_board.TabIndex = 4;
            this.but_board.Text = "Board Am335x";
            this.but_board.UseVisualStyleBackColor = true;
            this.but_board.Click += new System.EventHandler(this.but_board_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.AliceBlue;
            this.ClientSize = new System.Drawing.Size(1067, 554);
            this.Controls.Add(this.but_board);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.Btn_modbus_win);
            this.Controls.Add(this.label1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button Btn_modbus_win;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button but_board;
    }
}

